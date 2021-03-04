#include <hook_manager.hpp>
#include <mutex>
#include <utility>
#include "qfx_dynamic_collections.hpp"
#include "qfx_main.hpp"
#include "qfx_runtime.hpp"
#include "qfx_hook_manager.hpp"

using namespace reshade;

HMODULE WINAPI HookLoadLibraryA(LPCSTR lpFileName);
HMODULE WINAPI HookLoadLibraryExA(LPCSTR lpFileName, HANDLE hFile, DWORD dwFlags);
HMODULE WINAPI HookLoadLibraryW(LPCWSTR lpFileName);
HMODULE WINAPI HookLoadLibraryExW(LPCWSTR lpFileName, HANDLE hFile, DWORD dwFlags);

namespace QuakeFX
{
	typedef bool (*hookInstallPtr)(const char* name, void* target, void* replacement, bool queue_enable);

	using mutex = std::mutex;

	template <class _Mutex>
	using unique_lock = std::unique_lock<_Mutex>;

	template <class _Mutex>
	using lock_guard = std::lock_guard<_Mutex>;

	template<typename T>
	using vector = std::vector<T>;

	template<typename S, typename T>
	using pair = std::pair<S,T>;

	static mutex initMutex;

	HMODULE QfxHookManager::moduleHandle;
	vector<pair<const char*, void*>> QfxHookManager::libLoadHooks;
	void* QfxHookManager::LibLoadCallback = false;
	bool QfxHookManager::registered = false;
	bool QfxHookManager::initialized = false;
	bool QfxHookManager::glHooksInstalled = false;

	vector<RepTargPair> GetQfxReshadeHookPairs(vector<ModuleExport>& reshadeExports, const char* prefix);
	vector<ModuleExport> GetEnumeratedModuleExports(HMODULE handle);

	void QfxHookManager::Init(HMODULE hModule)
	{
		if (!registered)
		{
			moduleHandle = hModule;

#ifndef RESHADE_TEST_APPLICATION
			// Install callback hooks to listen for Reshade hook install
			hooks::install("QfxLoadLibraryA", HookLoadLibraryA, QfxLoadLibraryA, true);
			hooks::install("QfxLoadLibraryExA", HookLoadLibraryExA, QfxLoadLibraryExA, true);
			hooks::install("QfxLoadLibraryW", HookLoadLibraryW, QfxLoadLibraryW, true);
			hooks::install("QfxLoadLibraryExW", HookLoadLibraryExW, QfxLoadLibraryExW, true);

			hook::apply_queued_actions();
#else
			OnLibLoad();
#endif

			registered = true;
		}
	}

	/// <summary>
	/// Registers a callback that is invoked when it's safe to initialize the runtime.
	/// </summary>
	void QfxHookManager::RegisterInitCallback(void* callbackFunc)
	{
		if (LibLoadCallback == nullptr)
		{
			LibLoadCallback = callbackFunc;
		}
	}

	/// <summary>
	/// Install hooks for Reshade's OpenGL hooks.
	/// </summary>
	bool QfxHookManager::TryInstallGlHooks()
	{
		if (initialized && !glHooksInstalled)
		{
			vector<ModuleExport> reshadeExports(GetEnumeratedModuleExports(moduleHandle));
			vector<RepTargPair> reshadeHookPairs = GetQfxReshadeHookPairs(reshadeExports, "qfx_");

			glHooksInstalled = true;

			if (reshadeHookPairs.size() > 0)
			{ 
				QfxRuntime::LogInfo("Found %i OpenGL Hooks. Installing...", reshadeHookPairs.size());

				for (const RepTargPair& hookPair : reshadeHookPairs)
				{
					const ModuleExport replacement = *hookPair.replacement;

					if (hookPair.target != nullptr)
					{
						const ModuleExport target = *hookPair.target;
						hooks::install(replacement.name, target.address, replacement.address, true);
					}
					else if (replacement.name != nullptr)
					{ 
						QfxRuntime::LogError("Could not find hook target for %s", replacement.name);
						glHooksInstalled = false;
						break;
					}
				}
			}
			else
			{ 
				QfxRuntime::LogError("No hook targets found.");
				glHooksInstalled = false;
			}
		}
		
		hook::apply_queued_actions();
		return glHooksInstalled;
	}

	/// <summary>
	/// Retrieves a vector of QFX Opengl replacement functions paired with their Reshade targets
	/// </summary>
	vector<RepTargPair> GetQfxReshadeHookPairs(vector<ModuleExport>& reshadeExports, const char* prefix)
	{
		vector<RepTargPair> repTargPairs;
		repTargPairs.reserve(20);

		QfxRuntime::LogInfo("Loaded %i exported functions from the local module.", reshadeExports.size());

		// Get the symbols that start with "qfx_"
		for (const ModuleExport& symbol : reshadeExports)
		{
			if (symbol.name != nullptr && symbol.address != nullptr)
			{
				if (strstr(symbol.name, prefix) != nullptr)
				{
					RepTargPair& pair = repTargPairs.emplace_back();
					pair.replacement = &symbol;
					pair.target = nullptr;
				}
			}
		}

		// Match them with their Reshade targets
		for (RepTargPair& pair : repTargPairs)
		{
			for (const ModuleExport& symbol : reshadeExports)
			{
				if (&symbol != pair.replacement && symbol.name != nullptr && symbol.address != nullptr)
				{
					if (strcmp((pair.replacement->name) + 4, symbol.name) == 0)
					{
						pair.target = &symbol;
						break;
					}
				}
			}
		}

		return repTargPairs;
	}

	/// <summary>
	/// Retrieves a vector of functions exported by the given module.
	/// </summary>
	vector<ModuleExport> GetEnumeratedModuleExports(HMODULE handle)
	{
		const auto imgBasePtr = reinterpret_cast<const BYTE*>(handle);
		const auto dosHeaderPtr = reinterpret_cast<const IMAGE_DOS_HEADER*>(imgBasePtr);
		const auto imgHeaderPtr = reinterpret_cast<const IMAGE_NT_HEADERS*>(imgBasePtr + (dosHeaderPtr->e_lfanew));

		const IMAGE_NT_HEADERS& imgHeader = *imgHeaderPtr;
		const auto imgDataDir = imgHeader.OptionalHeader.DataDirectory;

		if (imgHeader.Signature != IMAGE_NT_SIGNATURE || imgDataDir[IMAGE_DIRECTORY_ENTRY_EXPORT].Size == 0)
			return {}; // The handle does not point to a valid module

		const auto exportDirPtr = reinterpret_cast<const IMAGE_EXPORT_DIRECTORY*>(
			(imgBasePtr + imgDataDir[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress) );

		const IMAGE_EXPORT_DIRECTORY& exportDir = *exportDirPtr;
		const auto exportBase = static_cast<WORD>(exportDir.Base);

		if (exportDir.NumberOfFunctions == 0)
			return {}; // This module does not contain any exported functions

		vector<ModuleExport> exports;
		exports.reserve(exportDir.NumberOfNames);

		for (size_t i = 0; i < exports.capacity(); i++)
		{
			ModuleExport& symbol = exports.emplace_back();
			const auto ordinals = reinterpret_cast<const WORD*>( imgBasePtr + exportDir.AddressOfNameOrdinals );
			const auto names = reinterpret_cast<const DWORD*>( imgBasePtr + exportDir.AddressOfNames );
			const auto addresses = reinterpret_cast<const DWORD*>( imgBasePtr + exportDir.AddressOfFunctions );

			symbol.ordinal = exportBase + ordinals[i];
			symbol.name = reinterpret_cast<const char*>(imgBasePtr + names[i]);
			symbol.address = const_cast<void*>(
				reinterpret_cast<const void*>( imgBasePtr + addresses[symbol.ordinal - exportBase] ));
		}

		return exports;
	}

	/// <summary>
	/// Callback function invoked when library load functions are called to load something other than the
	/// current module.
	/// </summary>
	void QfxHookManager::OnLibLoad()
	{
		if (unique_lock<mutex> lock(initMutex, std::try_to_lock); lock.owns_lock())
		{
			initialized = true;

			if (LibLoadCallback != nullptr)
				reinterpret_cast<CallbackFunc>(LibLoadCallback)();
		}
	}

	HMODULE WINAPI QfxHookManager::QfxLoadLibraryA(LPCSTR lpFileName)
	{
		auto trampoline = hooks::call(&QfxLoadLibraryA);
		const HMODULE handle = trampoline(lpFileName);

		if (!initialized && handle != nullptr && handle != moduleHandle)
			OnLibLoad();

		return handle;
	}

	HMODULE WINAPI QfxHookManager::QfxLoadLibraryExA(LPCSTR lpFileName, HANDLE hFile, DWORD dwFlags)
	{
		auto trampoline = hooks::call(&QfxLoadLibraryExA);
		const HMODULE handle = trampoline(lpFileName, hFile, dwFlags);
		bool exFlags = (dwFlags & (LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE | LOAD_LIBRARY_AS_IMAGE_RESOURCE)) == 0;

		if (!initialized && handle != nullptr && handle != moduleHandle && exFlags)
			OnLibLoad();

		return handle;
	}

	HMODULE WINAPI QfxHookManager::QfxLoadLibraryW(LPCWSTR lpFileName)
	{
		auto trampoline = hooks::call(&QfxLoadLibraryW);
		const HMODULE handle = trampoline(lpFileName);

		if (!initialized && handle != nullptr && handle != moduleHandle)
			OnLibLoad();

		return handle;
	}

	HMODULE WINAPI QfxHookManager::QfxLoadLibraryExW(LPCWSTR lpFileName, HANDLE hFile, DWORD dwFlags)
	{
		auto trampoline = hooks::call(&QfxLoadLibraryExW);
		const HMODULE handle = trampoline(lpFileName, hFile, dwFlags);
		bool exFlags = (dwFlags & (LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE | LOAD_LIBRARY_AS_IMAGE_RESOURCE)) == 0;

		if (!initialized && handle != nullptr && handle != moduleHandle && exFlags)
			OnLibLoad();

		return handle;
	}
}
