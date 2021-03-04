#include <hook_manager.hpp>
#include <vector>
#include <string>
#include <imgui.h>
#include "render/qfx_renderer.hpp"
#include "qfx_default_shader.hpp"
#include "qfx_hook_manager.hpp"

#define CFG QfxRuntime::GetConfig()

using namespace reshade;
using namespace glm;

typedef ImDrawData* (*ImDrawDataPtr)();

HOOK_EXPORT BOOL WINAPI qfxGL_wglSwapBuffers(HDC hdc);
HOOK_EXPORT BOOL WINAPI qfx_wglSwapBuffers(HDC hdc);
HOOK_EXPORT void WINAPI qfx_glEnable(GLenum cap);
HOOK_EXPORT void WINAPI qfx_glBlendFunc(GLenum sfactor​, GLenum dfactor​);

namespace QuakeFX
{
	QfxRenderer* QfxRenderer::instance = nullptr;

	bool QfxRenderer::isInitialized;
	bool QfxRenderer::isContextInitialized = false;

	CallbackPtr QfxRenderer::InitCallbackFunc;
	CallbackPtr QfxRenderer::CloseCallback;

	bool QfxRenderer::GetIsInitialized()
	{
		return isInitialized && instance != nullptr && g_current_runtime != nullptr && g_current_runtime->_compatibility_context;
	}

	uint QfxRenderer::GetMaxMsaa()
	{
		if (instance != nullptr)
			return instance->maxMsaa;
		else
			return 0;
	}

	uint QfxRenderer::GetMaxAnisotropy()
	{
		if (instance != nullptr)
			return instance->maxAnisotropy;
		else
			return 0;
	}

	void QfxRenderer::Init(CallbackPtr initCallback, CallbackPtr closeCallback)
	{
		if (!isInitialized && initCallback != nullptr && closeCallback != nullptr)
		{
			InitCallbackFunc = initCallback;
			CloseCallback = closeCallback;

			hooks::install("qfxGL_wglSwapBuffers", wglSwapBuffers, qfxGL_wglSwapBuffers, false);
			hooks::install("HookImgui", ImGui::GetDrawData, HookImgui, false);

			isInitialized = true;
		}
	}

	void QfxRenderer::Close()
	{
		isInitialized = false;
		isContextInitialized = false;
		InitCallbackFunc = nullptr;
		CloseCallback = nullptr;

		delete instance;
	}

	QfxRenderer* QfxRenderer::GetInstance()
	{
		if (isInitialized && instance == nullptr && g_current_runtime != nullptr && g_current_runtime->_compatibility_context)
		{
			instance = new QfxRenderer();
		}

		return instance;
	}

	QfxRenderer::QfxRenderer() :
		skipQfxRender(false),
		curDevContext(NULL),
		curRenderContext(NULL),
		viewport(ivec4(0,0,0,0)),
		vpSize(ivec2(0,0)),
		colorBuf(QfxRenderBuffer(ImageFormats::RGBA8)),
		depthStencilBuf(QfxRenderBuffer(ImageFormats::Depth24Stencil8)),
		curFrameOffset(0),
		lastFrameCount(0),
		frameCount(0),
		viewportFrame(0),
		lastMatrixMode(GL_MODELVIEW),
		maxAnisotropy(0),
		maxMsaa(0)
	{ }

	QfxRenderer :: ~QfxRenderer()
	{
		instance = nullptr;
	}

	void QfxRenderer::OnContextInit()
	{
		if (!isContextInitialized && GetIsInitialized())
		{
			int major, minor;
			glGetIntegerv(GL_MAJOR_VERSION, &major);
			glGetIntegerv(GL_MINOR_VERSION, &minor);
			QfxRuntime::LogInfo("OpenGL Version: %i.%i", major, minor);

			InitCallbackFunc();
			maxMsaa = (uint)QfxFramebufferObj::GetMaxSamples();

			if (maxMsaa == 0)
				QfxRuntime::LogWarning("MSAA not supported");
			else
				QfxRuntime::LogInfo("Max MSAA factor: %i", maxMsaa);

			if (QfxRuntime::GetGlExtensionPresent("GL_EXT_texture_filter_anisotropic"))
			{
				GLint value;
				glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &value);

				maxAnisotropy = (uint)value;
				QfxRuntime::LogInfo("Max anisotropy: %i", maxAnisotropy);
			}

			if (maxAnisotropy == 0)
				QfxRuntime::LogWarning("Anisotropic filtering not supported");

			isContextInitialized = true;
		}
	}

	void QfxRenderer::OnMakeCurrent(HDC devContext, HGLRC renderContext)
	{
		if (devContext != NULL && renderContext != NULL)
		{
			if (devContext != curDevContext || renderContext != curRenderContext)
			{
				curDevContext = devContext;
				curRenderContext = renderContext;

				OnContextInit();
			}
		}
	}

	void QfxRenderer::OnSetBlendFunc(GLenum sfactor​, GLenum dfactor​)
	{
		auto trampoline = hooks::call(qfx_glBlendFunc);

		if (uiCap.GetIsCapturing())
		{
			trampoline(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else
		{
			trampoline(sfactor​, dfactor​);
		}
	}

	void QfxRenderer::OnDeleteContext(HGLRC renderContext)
	{
		if (renderContext != NULL && renderContext == curRenderContext)
		{
			CloseCallback();
		}
	}

	void QfxRenderer::OnMatrixModeChanged(GLenum mode)
	{
		lastMatrixMode = mode;
	}

	void QfxRenderer::OnBindTex2D()
	{
		if (maxAnisotropy > 1)
		{
			int value = std::max(1, std::min((int)maxAnisotropy, CFG.anisotropyLevel));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, value);
		}
	}

	void QfxRenderer::OnProjection(GLboolean isOrthographic)
	{
		if (isOrthographic)
		{
			if (lastFrameCount != frameCount)
			{
				lastFrameCount = frameCount;
				curFrameOffset = 0;
			}
			else
			{
				curFrameOffset++;
			}

			if (curFrameOffset == CFG.renderFrame)
			{
				if (!skipQfxRender && CFG.renderFrame != -1)
				{
					uiCap.StartCapture();
				}
			}
		}
		else
		{
			if (viewportFrame != frameCount)
			{
				viewportFrame = frameCount;
			}
		}
	}

	ImDrawData* QfxRenderer::HookImgui()
	{
		// Workaround for bug present in Reshade 4.9.1 that breaks texture wrapping
		// on AMD cards. Not sure why it's happening. Could be a driver bug for all
		// I know.
		for (int i = 0; i < 32; i++)
			glBindSampler(i, 0);

		auto trampoline = hooks::call(HookImgui);
		ImDrawData* data = trampoline();

		if (instance != nullptr && data != nullptr && data->CmdListsCount != 0 && data->TotalVtxCount != 0)
		{
			instance->uiCap.ResumeCapture();
		}

		return data;
	}

	BOOL QfxRenderer::OnSwapBuffers(HDC hdc)
	{
		// Stop UI capture and blit scene FBO into default framebuffer
		uiCap.StopCapture();

		if (sceneFbo.GetIsBound())
		{
			sceneFbo.BlitTo(ColorBuffers::ColorAttach, 0, ColorBuffers::Back, vpSize, FramebufferMasks::ColorDepthStencil);
		}

		auto reshadeSwap = hooks::call(qfx_wglSwapBuffers);
		reshadeSwap(hdc);

		if (hdc != NULL)
		{
			if (uiCap.GetIsCaptured())
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				uiCap.Draw();
			}

			frameCount++;
		}

		auto baseSwap = hooks::call(qfxGL_wglSwapBuffers);
		BOOL success = baseSwap(hdc);

		// Setup for next frame
		glGetIntegerv(GL_VIEWPORT, &viewport[0]);
		vpSize = ivec2(viewport.z, viewport.w);
		int msaaFactor = std::min((int)maxMsaa, CFG.msaaFactor);

		sceneFbo.Bind(FramebufferTargs::Framebuffer);
		QfxFramebufferObj::SetDrawBuffer(ColorBuffers::ColorAttach);

		colorBuf.Bind();
		colorBuf.SetSize(vpSize, msaaFactor);

		depthStencilBuf.Bind();
		depthStencilBuf.SetSize(vpSize, msaaFactor);
		depthStencilBuf.Unbind();

		sceneFbo.SetFrameBufferRenderBuffer(colorBuf, FramebufferAttachments::Color);
		sceneFbo.SetFrameBufferRenderBuffer(depthStencilBuf, FramebufferAttachments::DepthStencil);

		return success;
	}
}
