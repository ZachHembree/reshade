#ifndef QFX_GLBUFFER_H
#define QFX_GLBUFFER_H

#include "opengl/qfx_opengl.hpp"
#include "qfx_dynamic_collections.hpp"

namespace QuakeFX
{
	enum class BufferContextBinding
	{
		Invalid = -1,

		/// <summary>
		/// Vertex attribute buffer binding
		/// </summary>
		ArrayBuffer = GL_ARRAY_BUFFER_BINDING,

		/// <summary>
		/// Indirect compute dispatch commands
		/// </summary>
		DispatchIndirect = GL_DISPATCH_INDIRECT_BUFFER_BINDING,

		/// <summary>
		/// Vertex array indices
		/// </summary>
		ElementArray = GL_ELEMENT_ARRAY_BUFFER_BINDING,

		/// <summary>
		/// Pixel read target
		/// </summary>
		PixelPack = GL_PIXEL_PACK_BUFFER_BINDING,

		/// <summary>
		/// Texture data source
		/// </summary>
		PixelUnpack = GL_PIXEL_UNPACK_BUFFER_BINDING,

		/// <summary>
		/// Read-write storage for shaders
		/// </summary>
		ShaderStorage = GL_SHADER_STORAGE_BUFFER_BINDING,

		TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER_BINDING,

		Uniform = GL_UNIFORM_BUFFER_BINDING,
	};

	enum class BufferBindingTargets
	{
		Invalid = -1,

		/// <summary>
		/// Vertex attribute buffer
		/// </summary>
		ArrayBuffer = GL_ARRAY_BUFFER,

		/// <summary>
		/// Atomic counter storage
		/// </summary>
		AtomicCounter = GL_ATOMIC_COUNTER_BUFFER,

		/// <summary>
		/// Copy source
		/// </summary>
		CopyRead = GL_COPY_READ_BUFFER,

		/// <summary>
		/// Copy destination
		/// </summary>
		CopyWrite = GL_COPY_WRITE_BUFFER,

		/// <summary>
		/// Indirect compute dispatch commands
		/// </summary>
		DispatchIndirect = GL_DISPATCH_INDIRECT_BUFFER,

		/// <summary>
		/// Indirect command arguments
		/// </summary>
		DrawIndirect = GL_DRAW_INDIRECT_BUFFER,

		/// <summary>
		/// Vertex array indices
		/// </summary>
		ElementArray = GL_ELEMENT_ARRAY_BUFFER,

		/// <summary>
		/// Pixel read target
		/// </summary>
		PixelPack = GL_PIXEL_PACK_BUFFER,

		/// <summary>
		/// Texture data source
		/// </summary>
		PixelUnpack = GL_PIXEL_UNPACK_BUFFER,

		/// <summary>
		/// Query result buffer
		/// </summary>
		QueryBuffer = GL_QUERY_BUFFER,

		/// <summary>
		/// Read-write storage for shaders
		/// </summary>
		ShaderStorage = GL_SHADER_STORAGE_BUFFER,

		TextureBuffer = GL_TEXTURE_BUFFER,
		TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER,
		Uniform = GL_UNIFORM_BUFFER
	};

	/// <summary>
	/// Used to specify a buffer's usage pattern. Usage patterns are used
	/// to optimize for a given kind of behavior, not constrain it.
	/// </summary>
	enum class BufferUsagePatterns
	{
		Invalid = -1,

		/// <summary>
		/// STREAM: The data store contents will be modified once and used at most a few times.
		/// DRAW: The data store contents are modified by the application, and used as the source
		/// for GL drawing and image specification commands.
		/// </summary>
		StreamDraw = GL_STREAM_DRAW,

		/// <summary>
		/// STREAM: The data store contents will be modified once and used at most a few times.
		/// READ: The data store contents are modified by reading data from the GL, and used to 
		/// return that data when queried by the application.
		/// </summary>
		StreamRead = GL_STREAM_READ,

		/// <summary>
		/// STREAM: The data store contents will be modified once and used at most a few times.
		/// COPY: The data store contents are modified by reading data from the GL, and used as 
		/// the source for GL drawing and image specification commands.
		/// </summary>
		StreamCopy = GL_STREAM_COPY,

		/// <summary>
		/// STATIC: The data store contents will be modified once and used many times.
		/// DRAW: The data store contents are modified by the application, and used as the source
		/// for GL drawing and image specification commands.
		/// </summary>
		StaticDraw = GL_STATIC_DRAW,

		/// <summary>
		/// STATIC: The data store contents will be modified once and used many times.
		/// READ: The data store contents are modified by reading data from the GL, and used to 
		/// return that data when queried by the application.
		/// </summary>
		StaticRead = GL_STATIC_READ,

		/// <summary>
		/// STATIC: The data store contents will be modified once and used many times.
		/// COPY: The data store contents are modified by reading data from the GL, and used as 
		/// the source for GL drawing and image specification commands.
		/// </summary>
		StaticCopy = GL_STATIC_COPY,

		/// <summary>
		/// DYNAMIC: The data store contents will be modified repeatedly and used many times.
		/// DRAW: The data store contents are modified by the application, and used as the source
		/// for GL drawing and image specification commands.
		/// </summary>
		DynamicDraw = GL_DYNAMIC_DRAW,

		/// <summary>
		/// DYNAMIC: The data store contents will be modified repeatedly and used many times.
		/// READ: The data store contents are modified by reading data from the GL, and used to 
		/// return that data when queried by the application.
		/// </summary>
		DynamicRead = GL_DYNAMIC_READ,

		/// <summary>
		/// DYNAMIC: The data store contents will be modified repeatedly and used many times.
		/// COPY: The data store contents are modified by the application, and used as the source
		/// for GL drawing and image specification commands.
		/// </summary>
		DynamicCopy = GL_DYNAMIC_COPY
	};

	class QfxBuffer
	{
	public:
		/// <summary>
		/// Initializes an empty, invalid buffer wrapper.
		/// </summary>
		QfxBuffer();

		/// <summary>
		/// Initializes a new buffer with the given target and usage pattern.
		/// </summary>
		QfxBuffer(BufferBindingTargets target, BufferUsagePatterns pattern);

		/// <summary>
		/// Copy constructor
		/// </summary>
		QfxBuffer(const QfxBuffer& other) = delete;

		/// <summary>
		/// Move constructor
		/// </summary>
		QfxBuffer(QfxBuffer&& other) noexcept;

		~QfxBuffer();

		/// <summary>
		/// Copy assignment operator. Deleted.
		/// </summary>
		QfxBuffer& operator=(const QfxBuffer& rhs) = delete;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		QfxBuffer& operator=(QfxBuffer&& rhs) noexcept;

		/// <summary>
		/// Returns the unique id/name of the buffer object.
		/// </summary>
		uint GetID() const;

		/// <summary>
		/// Returns true if the buffer is currently bound to its target
		/// in the current context.
		/// </summary>
		bool GetIsBound() const;

		/// <summary>
		/// Returns true if the buffer is currently bound to its target
		/// in the current context.
		/// </summary>
		static bool GetIsBufferBound(GLuint id, BufferContextBinding binding);

		/// <summary>
		/// Returns the binding target for the buffer.
		/// </summary>
		BufferBindingTargets GetBindingTarget() const;

		/// <summary>
		/// Returns the usage pattern for the buffer.
		/// </summary>
		BufferUsagePatterns GetUsagePattern() const;

		/// <summary>
		/// Binds the buffer to the target specified at initialization.
		/// </summary>
		void Bind();

		/// <summary>
		/// Unbinds the buffer from its target.
		/// </summary>
		void Unbind();

		/// <summary>
		/// Creates a new data store for the buffer using the data given.
		/// Any preexisting data stores will be deleted. Buffer must be
		/// bound to target before data is assigned.
		/// </summary>
		template<typename T>
		void SetData(DynamicArrayBase<T>& data)
		{
			SetData(data.GetPtr(), data.GetLength() * sizeof(T));
		}

		/// <summary>
		/// Creates a new data store for the buffer using the data given.
		/// Any preexisting data stores will be deleted. Buffer must be
		/// bound to target before data is assigned.
		/// </summary>
		template<typename T>
		void SetData(std::vector<T>& data)
		{
			SetData(data.data(), data.size() * sizeof(T));
		}

		/// <summary>
		/// Creates a new data store for the buffer using the data given.
		/// Any preexisting data stores will be deleted. Buffer must be
		/// bound to target before data is assigned.
		/// </summary>
		void SetData(void* data, size_t sizeInBytes);

		/// <summary>
		/// Unbinds the buffer from the given target
		/// </summary>
		static void UnbindBuffer(BufferBindingTargets target, GLuint id);

		/// <summary>
		/// Binds the buffer associated with the given name to the given target.
		/// </summary>
		static void BindBuffer(BufferBindingTargets target, GLuint id);

		/// <summary>
		/// Retrieves the name of the buffer using the binding associated with the
		/// given target.
		/// </summary>
		static GLuint GetCurrentBuffer(BufferBindingTargets target);

		/// <summary>
		/// Retrieves the name of the given target in the current context.
		/// </summary>
		static GLuint GetCurrentBuffer(BufferContextBinding binding);

		/// <summary>
		/// Finds and returns the buffer binding associated with the given binding target,
		/// if one exists.
		/// </summary>
		static BufferContextBinding GetBufferContextBinding(BufferBindingTargets target);

	private:
		uint id;
		BufferBindingTargets target;
		BufferContextBinding binding;
		BufferUsagePatterns pattern;

		void Validate(bool bindRequired) const;

		static void ValidateBuffer(GLuint id, BufferContextBinding binding, bool bindRequired);
	};
}

#endif // !QFX_GLBUFFER_H
