#include <hook_manager.hpp>
#include "qfx_runtime.hpp"
#include "render/qfx_renderer.hpp"

#define IS_3D_PROJECTION_MATRIX(x)	(x[2*4+3] < 0)
#define MATRIXMODE RENDERER->GetLastMatrixMode()
#define RENDERER QfxRenderer::GetInstance()

using namespace reshade;
using namespace QuakeFX;

HOOK_EXPORT void WINAPI qfx_glMatrixMode(GLenum mode)
{
	if (RENDERER->GetIsInitialized())
	{ 
		RENDERER->OnMatrixModeChanged(mode);
	}

	auto trampoline = hooks::call(qfx_glMatrixMode);
	trampoline(mode);
}

HOOK_EXPORT void WINAPI qfx_glLoadMatrixd(const GLdouble *m)
{
	auto trampoline = hooks::call(qfx_glLoadMatrixd);
	trampoline(m);

	if (RENDERER->GetIsInitialized() && MATRIXMODE == GL_PROJECTION)
	{
		RENDERER->OnProjection(!IS_3D_PROJECTION_MATRIX(m));
	}
}

HOOK_EXPORT void WINAPI qfx_glLoadMatrixf(const GLfloat *m)
{
	auto trampoline = hooks::call(qfx_glLoadMatrixf);
	trampoline(m);

	if (RENDERER->GetIsInitialized() && MATRIXMODE == GL_PROJECTION)
	{
		RENDERER->OnProjection(!IS_3D_PROJECTION_MATRIX(m));
	}
}

HOOK_EXPORT void WINAPI qfx_glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
	auto trampoline = hooks::call(qfx_glFrustum);
	trampoline(left, right, bottom, top, zNear, zFar);

	if (RENDERER->GetIsInitialized() && MATRIXMODE == GL_PROJECTION)
	{
		RENDERER->OnProjection(false);
	}
}

HOOK_EXPORT void WINAPI qfx_glOrtho( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar )
{
	auto trampoline = hooks::call(qfx_glOrtho);
	trampoline(left, right, bottom, top, zNear, zFar);

	if (RENDERER->GetIsInitialized() && MATRIXMODE == GL_PROJECTION)
	{
		RENDERER->OnProjection(true);
	}
}

HOOK_EXPORT void WINAPI qfx_glBlendFunc(GLenum sfactor​, GLenum dfactor​)
{
	if (RENDERER->GetIsInitialized())
	{
		RENDERER->OnSetBlendFunc(sfactor​, dfactor​);
	}
	else
	{
		auto trampoline = hooks::call(qfx_glBlendFunc);
		trampoline(sfactor​, dfactor​);
	}
}

HOOK_EXPORT void WINAPI qfx_glBindTexture(GLenum target, GLuint texture)
{
	auto trampoline = hooks::call(qfx_glBindTexture);
	trampoline(target, texture);

	if (RENDERER->GetIsInitialized() && target == GL_TEXTURE_2D)
	{
		RENDERER->OnBindTex2D();
	}
}

HOOK_EXPORT BOOL WINAPI qfx_wglMakeCurrent( HDC hdc, HGLRC hglrc )
{
	auto trampoline = hooks::call(qfx_wglMakeCurrent);
	QfxRuntime::Init();

	if (!trampoline(hdc, hglrc))
		return FALSE;
	else if (QfxRuntime::GetIsInitialized() && RENDERER != nullptr)
		RENDERER->OnMakeCurrent( hdc, hglrc );

	return TRUE;
}

HOOK_EXPORT BOOL WINAPI qfx_wglDeleteContext( HGLRC hglrc )
{
	if (RENDERER->GetIsInitialized())
		RENDERER->OnDeleteContext(hglrc);

	auto trampoline = hooks::call(qfx_wglDeleteContext);
	return trampoline(hglrc);
}

HOOK_EXPORT BOOL WINAPI qfx_wglSwapBuffers(HDC hdc)
{
	if (!RENDERER->GetIsInitialized())
	{
		auto reshadeSwap = hooks::call(qfx_wglSwapBuffers);
		return reshadeSwap(hdc);
	}
	else
	{
		return RENDERER->OnSwapBuffers(hdc);
	}
}

HOOK_EXPORT BOOL WINAPI qfxGL_wglSwapBuffers(HDC hdc)
{
	if (!RENDERER->GetIsInitialized())
	{
		auto baseSwap = hooks::call(qfxGL_wglSwapBuffers);
		return baseSwap(hdc);
	}
	else
	{
		return GL_FALSE;
	}
}
