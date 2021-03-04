#include <stdio.h>
#include <string.h>
#include <fstream>
#include <wtypes.h>
#include <mutex>
#include <tgmath.h>
#include <imgui.h>
#include <dll_config.hpp>
#include <dll_log.hpp>
#include "qfx_runtime.hpp"
#include "render/qfx_renderer.hpp"
#include "qfx_dynamic_collections.hpp"
#include "qfx_hook_manager.hpp"

using namespace reshade;
using namespace reshade::opengl;
using namespace reshade::api;

namespace QuakeFX
{
	typedef void (*ImGuiCallback)(const char* title, void(*callback)(reshade::api::effect_runtime* runtime, void* imgui_context));

	using std::unique_lock;
	using std::lock_guard;
	using std::vector;
	using std::string;

	static std::mutex mutex;
	const char* const factorText[] = { "Disabled", "2x", "4x", "8x", "16x", "32x" };

	template<typename T, int sz>
	constexpr int arrSize(T(&)[sz]) { return sz; }

	QfxRuntime* QfxRuntime::instance;
	bool QfxRuntime::isInitialized;
	bool QfxRuntime::initFailed;
	bool QfxRuntime::unloading;

	void WriteToLog(log::level logLevel, const char* text);

	void QfxRuntime::Init()
	{
		if (!isInitialized && !unloading)
		{
			instance = new QfxRuntime(QfxHookManager::GetRegisteredHandle());
			isInitialized = true;

			instance->InitWGL();
			QfxRenderer::Init(RenderInit, Close);
		}
	}

	QfxRuntime::QfxRuntime(HMODULE reshadeDll) :
		reshadeDll(reshadeDll),
		openGlDll(NULL)
	{ }

	void QfxRuntime::RenderInit()
	{
#ifndef RESHADE_TEST_APPLICATION
		g_current_runtime->subscribe_to_load_config(LoadConfigCallback);
		g_current_runtime->subscribe_to_save_config(SaveConfigCallback);
#endif

		static const auto func = reinterpret_cast<ImGuiCallback>(
			GetProcAddress(instance->reshadeDll, "ReShadeRegisterOverlay"));
		func("QuakeFX", DrawImGui);

		int numExtensions;
		glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
		instance->extensions.reserve(numExtensions);

		for (int i = 0; i < numExtensions; i++)
		{
			auto extension = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
			instance->extensions.insert(extension);
		}
	}

	bool QfxRuntime::GetGlExtensionPresent(const char* const extName)
	{
		if (instance != nullptr)
		{
			const auto& extensions = instance->extensions;
			return extensions.find(extName) != extensions.end();
		}
		else
			return false;
	}

	void QfxRuntime::DrawImGui(effect_runtime* runtime, void* imguiContext)
	{
		QfxConfig& cfg = instance->cfg;
		const int maxAnisotropy = (int)std::min(32u, QfxRenderer::GetMaxAnisotropy()),
			anisoFactorCount = (int)std::log2(maxAnisotropy) + 1,
			maxMsaa = (int)std::min(32u, QfxRenderer::GetMaxMsaa()),
			msaaFactorCount = (int)std::log2(maxMsaa) + 1;

		if (ImGui::SliderInt("Render Frame", &cfg.renderFrame, -1, 5))
		{
			g_current_runtime->save_config();
		}

		int msaaSelection = cfg.msaaFactor == 0 ? 0 : (int)std::log2(cfg.msaaFactor);

		if (ImGui::Combo("MSAA", &msaaSelection, factorText, msaaFactorCount))
		{
			cfg.msaaFactor = std::clamp(2 << (msaaSelection - 1), 0, maxMsaa);
			g_current_runtime->save_config();
		}

		int ansioSelection = cfg.anisotropyLevel == 0 ? 0 : (int)std::log2(cfg.anisotropyLevel);

		if (ImGui::Combo("Anisotropic Filtering", &ansioSelection, factorText, anisoFactorCount))
		{
			cfg.anisotropyLevel = std::clamp(2 << (ansioSelection - 1), 0, maxAnisotropy);
			g_current_runtime->save_config();
		}

		PIXELFORMATDESCRIPTOR pfd;
		HDC dc = QfxRenderer::GetCurrentDeviceContext();
		DescribePixelFormat(dc, GetPixelFormat(dc), sizeof(PIXELFORMATDESCRIPTOR), &pfd);

		ImGui::LabelText("", "Color Bits: %i, Depth Bits: %i, Stencil Bits: %i", pfd.cColorBits, pfd.cDepthBits, pfd.cStencilBits);
	}

	void QfxRuntime::Close()
	{
		if (isInitialized)
		{
			QfxRuntime::LogInfo("Exiting...");

			isInitialized = false;
			unloading = true;
			QfxRenderer::Close();
			instance->CloseGL();
			
			delete instance;

			QfxRuntime::LogInfo("Finished exiting.");
		}
	}

	void QfxRuntime::LogInfo(const char* formatString, ...)
	{
		char buff[200];

		va_list args;
		va_start(args, formatString);
		vsnprintf(buff, 200, formatString, args);
		va_end(args);

		WriteToLog(log::level::info, buff);
	}

	void QfxRuntime::LogError(const char* formatString, ...)
	{
		char buff[200];

		va_list args;
		va_start(args, formatString);
		vsnprintf(buff, 200, formatString, args);
		va_end(args);

		WriteToLog(log::level::error, buff);
	}

	void QfxRuntime::LogWarning(const char* formatString, ...)
	{
		char buff[200];

		va_list args;
		va_start(args, formatString);
		vsnprintf(buff, 200, formatString, args);
		va_end(args);

		WriteToLog(log::level::warning, buff);
	}

	void QfxRuntime::LogDebug(const char* formatString, ...)
	{
	#ifndef NDEBUG
		char buff[200];

		va_list args;
		va_start(args, formatString);
		vsnprintf(buff, 200, formatString, args);
		va_end(args);

		WriteToLog(log::level::debug, buff);
	#endif
	}

	void WriteToLog(log::level level, const char* text)
	{
		log::message message(level);
		message << "QuakeFX | " << text;
	}
}
