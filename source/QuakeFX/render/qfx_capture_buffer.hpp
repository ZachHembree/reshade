#ifndef QFX_CAPTURE_BUFFER_H
#define QFX_CAPTURE_BUFFER_H
#include "render/qfx_texture2D.hpp"
#include "render/qfx_program.hpp"
#include "render/qfx_buffer.hpp"
#include "render/qfx_framebuffer_obj.hpp"
#include "render/qfx_vertex_array_obj.hpp"
#include "render/qfx_vertex_layout.hpp"
#include "render/qfx_default_shader.hpp"

using namespace glm;

namespace QuakeFX
{
	/// <summary>
	/// Type used to capture color buffer data to be later drawn as a textured quad covering the whole screen.
	/// </summary>
	class CaptureBuffer
	{
	public:
		CaptureBuffer();

		CaptureBuffer(const CaptureBuffer& other) = delete;
		CaptureBuffer& operator=(const CaptureBuffer& rhs) = delete;

		/// <summary>
		/// Returns true if the buffer has started capturing color data.
		/// </summary>
		bool GetIsCapturing();

		/// <summary>
		/// Returns true if the buffer has been stopped after successfully capturing color
		/// data.
		/// </summary>
		bool GetIsCaptured();

		/// <summary>
		/// Starts capturing framebuffer color data using the current viewport size.
		/// </summary>
		void StartCapture();

		/// <summary>
		/// Starts capturing framebuffer color data using the given viewport size.
		/// </summary>
		void StartCapture(glm::ivec4 vp);

		/// <summary>
		/// Resumes capturing after being stopped without clearing the buffer or resizing.
		/// </summary>
		void ResumeCapture();

		/// <summary>
		/// Stops color data capture.
		/// </summary>
		void StopCapture();

		/// <summary>
		/// Draws color data to textured quad.
		/// </summary>
		void Draw();

	private:
		QfxFramebufferObj fbo;
		GLuint lastDrawFbo, lastReadFbo;
		QfxTexture2D texture;
		QfxVertexArrayObj vao;
		QfxBuffer vertexBuffer;
		QfxBuffer indexBuffer;
		QfxProgram program;
		UniqueArray<glm::vec2> vertices;
		UniqueArray<glm::ivec3> triangles;
		glm::ivec4 viewport;
		bool capturing;
		bool captured;
	};
}

#endif // !QFX_CAPTURE_BUFFER_H
