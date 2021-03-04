#include <stdio.h>
#include <fstream>
#include <wtypes.h>
#include <dll_config.hpp>
#include <dll_log.hpp>
#include <mutex>
#include "qfx_runtime.hpp"

using namespace reshade;
using namespace reshade::opengl;

namespace QuakeFX
{
	using wstring = std::wstring;

	template <class _Mutex>
	using unique_lock = std::unique_lock<_Mutex>;

	static std::mutex mutex;

	/// <summary>
	/// Cfg load callback registered to Reshade GL runtime
	/// </summary>
	void QfxRuntime::LoadConfigCallback(const ini_file& cfgFile)
	{
		QfxConfig& newConfig = instance->cfg;
		LogInfo("Loading config...");

		cfgFile.get("QuakeFX", "renderFrame", newConfig.renderFrame);
		cfgFile.get("QuakeFX", "anisotropyLevel", newConfig.anisotropyLevel);
		cfgFile.get("QuakeFX", "msaaFactor", newConfig.msaaFactor);
	}

	/// <summary>
	/// Cfg save callback registered to Reshade GL runtime
	/// </summary>
	void QfxRuntime::SaveConfigCallback(ini_file& cfgFile)
	{
		QfxConfig& newConfig = instance->cfg;
		LogInfo("Saving config...");

		cfgFile.set("QuakeFX", "renderFrame", newConfig.renderFrame);
		cfgFile.set("QuakeFX", "anisotropyLevel", newConfig.anisotropyLevel);
		cfgFile.set("QuakeFX", "msaaFactor", newConfig.msaaFactor);
	}
}
