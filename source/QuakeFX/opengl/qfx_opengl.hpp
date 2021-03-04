#ifndef QFX_OPENGL_H
#define QFX_OPENGL_H

#include <opengl/opengl.hpp>
#include <opengl/runtime_gl.hpp>
#include <GL/GLU.h>
#include <wingdi.h>
#include <glm/glm.hpp>
#include <GL/glext.h>
#include <GL/wglext.h>
#include <exception>

#if NDEBUG
#define GLCALL(x) {x;}
#define REPORTERROR(error) {}
#elif RESHADE_TEST_APPLICATION
#define ASSERT(x) if (!x) __debugbreak()
#define GLCALL(x) { while (glGetError() != GL_NO_ERROR); x; ASSERT(LogGlCall(#x, __FILE__, __LINE__)); }
#define REPORTERROR(x) { LogGlCall(#x, __FILE__, __LINE__); __debugbreak(); }

#else
#include <cstdio>
#define GLCALL(call) \
	{ \
		glGetError(); \
		call; \
		GLenum __e = glGetError(); \
		if (__e != GL_NO_ERROR) \
		{ \
			char __m[1024]; \
			sprintf_s(__m, "OpenGL error %x in %s at line %d: %s", __e, __FILE__, __LINE__, #call); \
			MessageBoxA(nullptr, __m, 0, MB_ICONERROR); \
		} \
	}
#define REPORTERROR(error) \
	{ \
		char __m[1024]; \
		sprintf_s(__m, "OpenGL error: \"%s\" in %s at line %d.", error, __FILE__, __LINE__); \
		MessageBoxA(nullptr, __m, 0, MB_ICONERROR); \
	}
#endif

namespace QuakeFX
{
	bool LogGlCall(const char* funcName, const char* srcFile, int line);
	const char* const GetGlErrorString(GLenum errorID);

}

typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char ubyte;

#define GL_MODELVIEW                      0x1700
#define GL_PROJECTION                     0x1701
#define GL_TEXTURE                        0x1702

typedef int (WINAPI* PFNWGLCHOOSEPIXELFORMATPROC)(HDC, const PIXELFORMATDESCRIPTOR*);
typedef int (WINAPI* PFNWGLDESCRIBEPIXELFORMATPROC) (HDC, int, UINT, LPPIXELFORMATDESCRIPTOR);
typedef int (WINAPI* PFNWGLGETPIXELFORMATPROC)(HDC);
typedef BOOL(WINAPI* PFNWGLSETPIXELFORMATPROC)(HDC, int, const PIXELFORMATDESCRIPTOR*);
typedef BOOL(WINAPI* PFNWGLSWAPBUFFERSPROC)(HDC);
typedef BOOL(WINAPI* PFNWGLCHOOSEPIXELFORMATARBPROC)(HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);

extern PFNWGLCHOOSEPIXELFORMATPROC	wglChoosePixelFormat;
extern PFNWGLDESCRIBEPIXELFORMATPROC wglDescribePixelFormat;
extern PFNWGLGETPIXELFORMATPROC wglGetPixelFormat;
extern PFNWGLSETPIXELFORMATPROC wglSetPixelFormat;
extern PFNWGLSWAPBUFFERSPROC wglSwapBuffers;
extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;

#endif //QFX_OPENGL_H
