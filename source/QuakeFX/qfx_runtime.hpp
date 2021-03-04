#ifndef QFX_RUNTIME_H
#define QFX_RUNTIME_H
#include <string.h>
#include <unordered_set>
#include <addon_manager.hpp>
#include "qfx_opengl.hpp"
#include "qfx_main.hpp"

// Yes, I'm exposing an internal global. Don't @ me.
extern thread_local reshade::opengl::runtime_impl* g_current_runtime;

namespace QuakeFX
{ 
	typedef unsigned int uint;
	typedef unsigned long ulong;
	typedef unsigned char ubyte;
	typedef char byte;

	struct QfxConfig
	{
		int	renderFrame;
		int	anisotropyLevel;
		int	msaaFactor;

		QfxConfig() :
			renderFrame(0),
			anisotropyLevel(16),
			msaaFactor(4)
		{ }
	};

	class QfxRuntime
	{
	public:
		static void Init();

		static bool GetIsInitialized() { return isInitialized; }
		static bool GetInitFailed() { return initFailed; }
		static const QfxConfig& GetConfig() { return instance->cfg; }

		static bool GetGlExtensionPresent(const char* const extName);
		static void LogInfo(const char* formatString, ...);
		static void LogError(const char* formatString, ...);
		static void LogWarning(const char* formatString, ...);
		static void LogDebug(const char* formatString, ...);

	private:
		static QfxRuntime* instance;
		static bool isInitialized;
		static bool initFailed;
		static bool unloading;

		HMODULE openGlDll;
		HMODULE reshadeDll;
		QfxConfig cfg;
		std::unordered_set<std::string> extensions;

		static void LoadConfigCallback(const reshade::ini_file& cfgFile);
		static void SaveConfigCallback(reshade::ini_file& cfgFile);
		static void RenderInit();
		static void DrawImGui(reshade::api::effect_runtime* runtime, void* imguiContext);
		static void Close();

		QfxRuntime(HMODULE reshadeDll);
		QfxRuntime() = delete;
		QfxRuntime(const QfxRuntime& other) = delete;
		void operator=(const QfxRuntime&) = delete;

		bool InitWGL();
		void CloseGL();

		template <typename T>
		bool TryLoadGL(LPCSTR name, T& glFunc)
		{
			void* address = GetProcAddress(openGlDll, name);

			if (address == nullptr)
			{
				QfxRuntime::LogError("Could not find function \"%s\" in opengl32.dll!", name);
				initFailed = true;
				return false;
			}
			else
			{
				QfxRuntime::LogDebug("Loaded \"%s\" at %i.", name, address);
				glFunc = reinterpret_cast<T>(address);
				return true;
			}
		}

		template <typename T>
		bool TryLoadWgl(LPCSTR name, T& glFunc)
		{
			void* address = wglGetProcAddress(name);
			
			if (address == nullptr)
			{
				QfxRuntime::LogError("Could not find function \"%s\" in opengl32.dll!", name);
				initFailed = true;
				return false;
			}
			else
			{
				QfxRuntime::LogDebug("Loaded \"%s\" at %i.", name, address);
				glFunc = reinterpret_cast<T>(address);
				return true;
			}
		}
	};
}
#endif // !QFX_RUNTIME_H

