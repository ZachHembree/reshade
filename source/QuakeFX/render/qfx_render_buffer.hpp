#ifndef QFX_RENDER_BUFFER_H
#define QFX_RENDER_BUFFER_H
#include "opengl/qfx_opengl.hpp"
#include "render/qfx_image_formats.hpp"

namespace QuakeFX
{
	class QfxRenderBuffer
	{
	public:
		QfxRenderBuffer(
			ImageFormats internalFormat = ImageFormats::RGBA8,
			glm::ivec2 dim = glm::ivec2(0,0),
			int samples = 0
		);

		QfxRenderBuffer(const QfxRenderBuffer& other) = delete;
		QfxRenderBuffer(QfxRenderBuffer&& other) noexcept;
		
		~QfxRenderBuffer();

		QfxRenderBuffer& operator=(QfxRenderBuffer&& rhs) noexcept;
		QfxRenderBuffer& operator=(const QfxRenderBuffer& rhs) = delete;

		/// <summary>
		/// Returns true if the render buffer is bound
		/// </summary>
		bool IsBound() const;

		/// <summary>
		/// Returns the name associated with the render buffer
		/// </summary>
		GLuint GetID();

		/// <summary>
		/// Returns the dimensions of the render buffer
		/// </summary>
		/// <returns></returns>
		glm::ivec2 GetDim() const;

		/// <summary>
		/// Returns the number of samples used by the render buffer.
		/// </summary>
		int GetNumSamples() const;

		/// <summary>
		/// Returns the internal pixel format of the render buffer.
		/// </summary>
		ImageFormats GetFormat() const;

		/// <summary>
		/// Binds the render buffer to the current context
		/// </summary>
		void Bind();

		/// <summary>
		/// Unbinds the render buffer from the current context
		/// </summary>
		void Unbind();

		/// <summary>
		/// Returns the name of the render buffer currently bound
		/// </summary>
		static GLuint GetCurrentRenderbuffer();

		/// <summary>
		/// Binds the render buffer associated with the given name to the current context
		/// </summary>
		static void BindRenderBuffer(GLuint id);

		/// <summary>
		/// Unbinds the 
		/// </summary>
		static void UnbindRenderBuffer(GLuint id);

		/// <summary>
		/// Resizes the render buffer to the given dimensions
		/// </summary>
		void SetSize(glm::ivec2 newSize, int newSamples = -1);

	private:
		GLuint id;
		ImageFormats internalFormat;
		glm::ivec2 dim;
		int samples;

		void Validate() const;

		static void Validate(GLuint id);
	};
}

#endif