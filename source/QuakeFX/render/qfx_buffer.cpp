#include <string>
#include "render/qfx_buffer.hpp"

namespace QuakeFX
{
	/// <summary>
	/// Initializes an empty, invalid buffer wrapper.
	/// </summary>
	QfxBuffer::QfxBuffer() :
		id(0),
		target(BufferBindingTargets::Invalid),
		binding(BufferContextBinding::Invalid),
		pattern(BufferUsagePatterns::Invalid)
	{ }

	/// <summary>
	/// Initializes a new buffer with the given target and usage pattern.
	/// </summary>
	QfxBuffer::QfxBuffer(BufferBindingTargets target, BufferUsagePatterns pattern) :
		id(0),
		target(target),
		binding(GetBufferContextBinding(target)),
		pattern(pattern)
	{
		// Get new id
		glGenBuffers(1, &id);
	}

	/// <summary>
	/// Move constructor
	/// </summary>
	QfxBuffer::QfxBuffer(QfxBuffer&& other) noexcept :
		id(other.id),
		target(other.target),
		binding(other.binding),
		pattern(other.pattern)
	{
		other.id = 0;
		other.target = BufferBindingTargets::Invalid;
		other.binding = BufferContextBinding::Invalid;
		other.pattern = BufferUsagePatterns::Invalid;
	}

	/// <summary>
	/// Destroy wrapper and delete buffer.
	/// </summary>
	QfxBuffer::~QfxBuffer()
	{
		if (id != 0)
		{
			glDeleteBuffers(1, &id);
		}
	}

	/// <summary>
	/// Move assignment operator.
	/// </summary>
	QfxBuffer& QfxBuffer::operator=(QfxBuffer&& rhs) noexcept
	{
		id = rhs.id;
		target = rhs.target;
		binding = rhs.binding;
		pattern = rhs.pattern;

		rhs.id = 0;
		rhs.target = BufferBindingTargets::Invalid;
		rhs.binding = BufferContextBinding::Invalid;
		rhs.pattern = BufferUsagePatterns::Invalid;

		return *this;
	}

	bool QfxBuffer::GetIsBound() const
	{
		return GetIsBufferBound(id, binding);
	}

	bool QfxBuffer::GetIsBufferBound(GLuint id, BufferContextBinding binding)
	{
		return id != 0 && GetCurrentBuffer(binding) == id;
	}

	/// <summary>
	/// Returns the unique id/name of the buffer object.
	/// </summary>
	uint QfxBuffer::GetID() const
	{
		return id;
	}

	/// <summary>
	/// Returns the binding target for the buffer.
	/// </summary>
	BufferBindingTargets QfxBuffer::GetBindingTarget() const
	{
		return target;
	}

	/// <summary>
	/// Returns the usage pattern for the buffer.
	/// </summary>
	BufferUsagePatterns QfxBuffer::GetUsagePattern() const
	{
		return pattern;
	}

	/// <summary>
	/// Binds the buffer to the target specified at initialization.
	/// </summary>
	void QfxBuffer::Bind()
	{
		// Bind to target
		Validate(false);
		glBindBuffer((GLenum)target, id);
	}

	void QfxBuffer::Unbind()
	{
		Validate(true);
		glBindBuffer((GLenum)target, 0);
	}

	/// <summary>
	/// Creates a new data store for the buffer using the data given.
	/// Any preexisting data stores will be deleted. Buffer must be
	/// bound to target before data is assigned.
	/// </summary>
	void QfxBuffer::SetData(void* data, size_t sizeInBytes)
	{
		Validate(true);
		glBufferData((GLenum)target, sizeInBytes, data, (int)pattern);
	}

	/// <summary>
	/// Binds the buffer associated with the given name to the given target.
	/// </summary>
	void QfxBuffer::BindBuffer(BufferBindingTargets target, GLuint id)
	{
		ValidateBuffer(id, GetBufferContextBinding(target), false);
		glBindBuffer((GLenum)target, id);
	}

	/// <summary>
	/// Unbinds the buffer from the given target
	/// </summary>
	void QfxBuffer::UnbindBuffer(BufferBindingTargets target, GLuint id)
	{
		ValidateBuffer(id, GetBufferContextBinding(target), true);
		glBindBuffer((GLenum)target, 0);
	}

	/// <summary>
	/// Retrieves the name of the given target in the current context.
	/// </summary>
	GLuint QfxBuffer::GetCurrentBuffer(BufferBindingTargets target)
	{
		int name;
		glGetIntegerv((GLenum)GetBufferContextBinding(target), &name);

		return (GLuint)name;
	}

	/// <summary>
	/// Retrieves the name of the given target in the current context.
	/// </summary>
	GLuint QfxBuffer::GetCurrentBuffer(BufferContextBinding binding)
	{
		int name;
		glGetIntegerv((GLenum)binding, &name);

		return (GLuint)name;
	}

	/// <summary>
	/// Finds and returns the buffer binding associated with the given binding target,
	/// if one exists.
	/// </summary>
	BufferContextBinding QfxBuffer::GetBufferContextBinding(BufferBindingTargets target)
	{
		switch (target)
		{
		case BufferBindingTargets::ArrayBuffer:
			return BufferContextBinding::ArrayBuffer;
		case BufferBindingTargets::DispatchIndirect:
			return BufferContextBinding::DispatchIndirect;
		case BufferBindingTargets::ElementArray:
			return BufferContextBinding::ElementArray;
		case BufferBindingTargets::PixelPack:
			return BufferContextBinding::PixelPack;
		case BufferBindingTargets::PixelUnpack:
			return BufferContextBinding::PixelUnpack;
		case BufferBindingTargets::ShaderStorage:
			return BufferContextBinding::ShaderStorage;
		case BufferBindingTargets::TransformFeedback:
			return BufferContextBinding::TransformFeedback;
		case BufferBindingTargets::Uniform:
			return BufferContextBinding::Uniform;
		default:
			return BufferContextBinding::Invalid;
		}
	}

	void QfxBuffer::Validate(bool bindRequired) const
	{
#ifndef NDEBUG
		if (target == BufferBindingTargets::Invalid || pattern == BufferUsagePatterns::Invalid)
			REPORTERROR("Cannot perform operations on non-existant buffers or buffers with invalid bindings or usage patterns.")
		else
			ValidateBuffer(id, binding, bindRequired);
#endif // !NDEBUG
	}

	void QfxBuffer::ValidateBuffer(GLuint id, BufferContextBinding binding, bool bindRequired)
	{
#ifndef NDEBUG
		if ((bindRequired && id == 0) || binding == BufferContextBinding::Invalid)
			REPORTERROR("Cannot perform operations on non-existant buffers or buffers with invalid bindings or usage patterns.")
		else if (bindRequired && !GetIsBufferBound(id, binding))
			REPORTERROR("The buffer must be bound to the current context to use this function.")
#endif // !NDEBUG
	}
}
