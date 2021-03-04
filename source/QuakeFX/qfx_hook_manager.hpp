#ifndef QFX_HOOKS_H
#define QFX_HOOKS_H
#include <windows.h>
#include <vector>

namespace std
{
	template<typename K, typename V>
	struct pair;

	class mutex;
}

namespace QuakeFX
{
	typedef void (*CallbackFunc)();

	struct ModuleExport
	{
		void* address;
		const char* name;
		unsigned short ordinal;

		ModuleExport() :
			address(nullptr),
			name(nullptr),
			ordinal(0)
		{ }
	};

	struct RepTargPair
	{
		const ModuleExport* replacement;
		const ModuleExport* target;

		RepTargPair() :
			replacement(nullptr),
			target(nullptr)
		{ }
	};

	class QfxHookManager
	{
	public:
		static void Init(HMODULE hModule);

		/// <summary>
		/// Registers a callback that is invoked when it's safe to initialize the runtime.
		/// </summary>
		static void RegisterInitCallback(void* callbackFunc);

		/// <summary>
		/// Install QFX GL hooks for Reshade's OpenGL hooks.
		/// </summary>
		static bool QfxHookManager::TryInstallGlHooks();

		static HMODULE GetRegisteredHandle() { return moduleHandle; }
		static bool GetCallbackRegistered() { return registered; }
		static bool GetInitialized() { return initialized; }
		static bool GetGlHooksInstalled() { return glHooksInstalled; }

	private:
		static HMODULE moduleHandle;
		static std::vector<std::pair<const char*, void*>> libLoadHooks;
		static void* LibLoadCallback;
		static bool registered;
		static bool initialized;
		static bool glHooksInstalled;

		QfxHookManager() = delete;
		QfxHookManager(const QfxHookManager& other) = delete;
		void operator=(const QfxHookManager& rhs) = delete;

		/// <summary>
		/// Callback function invoked when library load functions are called to load something other than the
		/// current module.
		/// </summary>
		static void OnLibLoad();

		static HMODULE WINAPI QfxLoadLibraryA(LPCSTR lpFileName);
		static HMODULE WINAPI QfxLoadLibraryExA(LPCSTR lpFileName, HANDLE hFile, DWORD dwFlags);
		static HMODULE WINAPI QfxLoadLibraryW(LPCWSTR lpFileName);
		static HMODULE WINAPI QfxLoadLibraryExW(LPCWSTR lpFileName, HANDLE hFile, DWORD dwFlags);
	};
}

#endif // QFX_HOOKS_H
