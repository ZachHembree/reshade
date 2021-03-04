#include <hook_manager.hpp>
#include "qfx_main.hpp"
#include "qfx_runtime.hpp"
#include "qfx_hook_manager.hpp"

using namespace reshade;
using namespace QuakeFX;

void InitCallback();

#ifdef RESHADE_TEST_APPLICATION

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
#undef WinMain

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	QfxRuntime::LogInfo("Test init");
	QfxHookManager::Init(hInstance);

	QfxRuntime::LogInfo("Installing hooks...");
	QfxHookManager::TryInstallGlHooks();

	return ReshadeWinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

#else

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved);
#undef DllMain

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			QfxRuntime::LogInfo("Lib init");
			QfxHookManager::Init(hModule);
			QfxHookManager::RegisterInitCallback(&InitCallback);

			break;
	}

	return ReshadeDllMain(hModule, fdwReason, lpReserved);
}
#endif

void InitCallback()
{
	QfxRuntime::LogInfo("Installing hooks...");
	QfxHookManager::TryInstallGlHooks();
}
