#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "hook_manager.hpp"
#include "qfx_runtime.hpp"
#include "qfx_opengl.hpp"
#include "qfx_dynamic_collections.hpp"

#define TRY_LOAD_GL( func ) if (!TryLoadGL(#func, ##func)) return false;
#define TRY_LOAD_WGL( func ) if (!TryLoadWgl(#func, ##func)) return false;

using std::stringstream;
using std::string;
using std::copy;
using std::endl;

// WGL Extensions
PFNWGLCHOOSEPIXELFORMATPROC wglChoosePixelFormat = nullptr;
PFNWGLDESCRIBEPIXELFORMATPROC wglDescribePixelFormat = nullptr;
PFNWGLGETPIXELFORMATPROC wglGetPixelFormat = nullptr;
PFNWGLSETPIXELFORMATPROC wglSetPixelFormat = nullptr;
PFNWGLSWAPBUFFERSPROC wglSwapBuffers = nullptr;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;

namespace QuakeFX
{
	bool TryLoadLibrary(wchar_t* path, HMODULE& module)
	{
		QfxRuntime::LogInfo("Loading \"%s\": ", path);
		module = LoadLibrary(path);

		if (module == nullptr)
		{
			QfxRuntime::LogError("Failed to load library");
			return false;
		}

		return true;
	}

	bool QfxRuntime::InitWGL()
	{
		wchar_t libPath[1024];
		GetSystemDirectory(libPath, sizeof(libPath) / 2);
		wcscat_s(libPath, L"\\opengl32.dll");

		if (TryLoadLibrary(libPath, openGlDll))
			QfxRuntime::LogInfo("Library loaded. Loading functions...");
		else
		{
			QfxRuntime::LogError("Failed to load libraries. Aborting.");
			return false;
		}
		
		TRY_LOAD_GL(wglChoosePixelFormat);
		TRY_LOAD_GL(wglDescribePixelFormat);
		TRY_LOAD_GL(wglGetPixelFormat);
		TRY_LOAD_GL(wglSetPixelFormat);
		TRY_LOAD_GL(wglSwapBuffers);

		QfxRuntime::LogInfo("Functions loaded");

		return true;
	}

	void QfxRuntime::CloseGL()
	{
		if (openGlDll != nullptr)
		{
			FreeLibrary(openGlDll);
			openGlDll = nullptr;
		}
	}

	const char* const GetGlErrorString(GLenum errorID)
	{
		switch (errorID)
		{
		case GL_NO_ERROR:
			return "No error.";
		case GL_INVALID_ENUM:
			return "Invalid enum.";
		case GL_INVALID_VALUE:
			return "Invalid value.";
		case GL_INVALID_OPERATION:
			return "Invalid operation.";
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			return "Invalid framebuffer operation.";
		case GL_OUT_OF_MEMORY:
			return "Out of memory.";
		case GL_STACK_UNDERFLOW:
			return "Stack underflow.";
		case GL_STACK_OVERFLOW:
			return "Stack overflow.";
		default:
			return "Error ID not recognised.";
		}
	}

	bool LogGlCall(const char* funcName, const char* srcFile, int line)
	{
		GLenum status = glGetError();
		std::stringstream sStream;
		string messages;
		int count = 0;

		messages.reserve(200);

		while (status != GL_NO_ERROR)
		{
			messages += '\t' + GetGlErrorString(status) + '\n';
			status = glGetError();
			count++;
		}

		if (count > 0)
		{
			sStream
				<< "[OpenGL Error] " << count << " errors occured at: " << endl
				<< "\t" << funcName << endl
				<< "\tin " << srcFile << " at line " << line << endl << endl
				<< "\tErrors:\n" << messages;

			QfxRuntime::LogError(sStream.str().c_str());
		}

		return count == 0;
	}
}
