#include "render/qfx_render_buffer.hpp"

using namespace glm;

namespace QuakeFX
{
	QfxRenderBuffer::QfxRenderBuffer(
		ImageFormats internalFormat,
		ivec2 dim,
		int samples
	) :
		id(0),
		internalFormat(internalFormat),
		dim(dim),
		samples(0)
	{
		glGenRenderbuffers(1, &id);

		if (dim.x > 0 || dim.y > 0)
		{
			Bind();
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, (GLenum)internalFormat, dim.x, dim.y);
		}
	}

	QfxRenderBuffer::QfxRenderBuffer(QfxRenderBuffer&& other) noexcept :
		id(other.id),
		internalFormat(other.internalFormat),
		dim(other.dim),
		samples(other.samples)
	{
		other.id = 0;
	}

	QfxRenderBuffer::~QfxRenderBuffer()
	{
		if (id != 0)
		{
			glDeleteRenderbuffers(1, &id);
		}
	}

	QfxRenderBuffer& QfxRenderBuffer::operator=(QfxRenderBuffer&& rhs) noexcept
	{
		id = rhs.id;
		internalFormat = rhs.internalFormat;
		dim = rhs.dim;
		samples = rhs.samples;

		rhs.id = 0;
		
		return *this;
	}

	/// <summary>
	/// Returns true if the render buffer is bound
	/// </summary>
	bool QfxRenderBuffer::IsBound() const
	{
		return id != 0 && GetCurrentRenderbuffer() == id;
	}

	/// <summary>
	/// Returns the name associated with the render buffer
	/// </summary>
	GLuint QfxRenderBuffer::GetID()
	{
		return id;
	}

	/// <summary>
	/// Returns the dimensions of the render buffer
	/// </summary>
	/// <returns></returns>
	ivec2 QfxRenderBuffer::GetDim() const
	{
		return dim;
	}

	/// <summary>
	/// Returns the number of samples used by the render buffer.
	/// </summary>
	int QfxRenderBuffer::GetNumSamples() const
	{
		return samples;
	}

	/// <summary>
	/// Returns the internal pixel format of the render buffer.
	/// </summary>
	ImageFormats QfxRenderBuffer::GetFormat() const
	{
		return internalFormat;
	}

	/// <summary>
	/// Binds the render buffer to the current context
	/// </summary>	
	void QfxRenderBuffer::Bind()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, id);
	}

	/// <summary>
	/// Unbinds the render buffer from the current context
	/// </summary>
	void QfxRenderBuffer::Unbind()
	{
		Validate();
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	/// <summary>
	/// Returns the name of the render buffer currently bound
	/// </summary>
	GLuint QfxRenderBuffer::GetCurrentRenderbuffer()
	{
		GLint name;
		glGetIntegerv(GL_RENDERBUFFER_BINDING, &name);

		return (GLuint)name;
	}

	/// <summary>
	/// Binds the render buffer associated with the given name to the current context
	/// </summary>
	void QfxRenderBuffer::BindRenderBuffer(GLuint id)
	{
		glBindRenderbuffer(GL_RENDERBUFFER, id);
	}

	/// <summary>
	/// Unbinds the 
	/// </summary>
	void QfxRenderBuffer::UnbindRenderBuffer(GLuint id)
	{
		Validate(id);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	/// <summary>
	/// Resizes the render buffer to the given dimensions
	/// </summary>
	void QfxRenderBuffer::SetSize(ivec2 newSize, int newSamples)
	{
		Validate();

		if (newSize != dim || (newSamples != -1 && newSamples != samples))
		{
			samples = newSamples != -1 ? newSamples : samples;
			dim = newSize;

			// Because the Khronos wiki said not doing this could cause completeness problems
			glDeleteRenderbuffers(1, &id);
			glGenRenderbuffers(1, &id);
			Bind();

			glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, (GLenum)internalFormat, dim.x, dim.y);
		}
	}

	void QfxRenderBuffer::Validate() const
	{
#ifndef NDEBUG
		if (GetCurrentRenderbuffer() != id)
			REPORTERROR("Renderbuffer objects must be bound before they can be oeprated on.");
#endif // !NDEBUG
	}

	void QfxRenderBuffer::Validate(GLuint id)
	{
#ifndef NDEBUG
		if (GetCurrentRenderbuffer() != id)
			REPORTERROR("Renderbuffer objects must be bound before they can be oeprated on.");
#endif // !NDEBUG
	}
}