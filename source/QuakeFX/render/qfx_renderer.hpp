#ifndef QFX_RENDERER_H
#define QFX_RENDERER_H
#include "qfx_runtime.hpp"
#include "render/qfx_capture_buffer.hpp"

namespace QuakeFX
{
	typedef void(*CallbackPtr)();

	class QfxRenderer
	{
	public:
		static void Init(CallbackPtr callbackFunc, CallbackPtr closeCallback);
		static void Close();

		static QfxRenderer* GetInstance();
		static bool GetIsInitialized();

		static HDC GetCurrentDeviceContext() { return instance != nullptr ? instance->curDevContext : nullptr; }
		static HGLRC GetCurrentRenderContext() { return instance != nullptr ? instance->curRenderContext : nullptr; }

		static uint GetMaxMsaa();
		static uint GetMaxAnisotropy();

		GLenum GetLastMatrixMode() { return instance->lastMatrixMode; }
		void OnMatrixModeChanged(GLenum mode);
		void OnMakeCurrent(HDC hdc, HGLRC hglrc);
		void OnDeleteContext(HGLRC hglrc);
		void OnSetBlendFunc(GLenum sfactor​, GLenum dfactor​);
		BOOL OnSwapBuffers(HDC hdc);
		void OnBindTex2D();
		void OnProjection(GLboolean enable);

	private:
		static QfxRenderer* instance;

		static bool isInitialized;
		static bool isContextInitialized;
		static CallbackPtr InitCallbackFunc;
		static CallbackPtr CloseCallback;

		HDC	curDevContext;
		HGLRC curRenderContext;
		GLenum lastMatrixMode;

		glm::ivec4 viewport;
		glm::ivec2 vpSize;

		CaptureBuffer uiCap;
		QfxFramebufferObj sceneFbo;
		QfxRenderBuffer colorBuf;
		QfxRenderBuffer depthStencilBuf;

		int	curFrameOffset;
		uint lastFrameCount;
		uint frameCount;
		uint viewportFrame;

		bool skipQfxRender;
		uint maxAnisotropy;
		uint maxMsaa;

		QfxRenderer();
		QfxRenderer(const QfxRenderer& other) = delete;
		void operator=(const QfxRenderer&) = delete;
		~QfxRenderer();

		void OnContextInit();
		static ImDrawData* HookImgui();
	};
}
#endif //QFX_RENDERER_H
