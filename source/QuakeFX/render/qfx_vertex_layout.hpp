#ifndef QFX_VERTEX_LAYOUT_H
#define QFX_VERTEX_LAYOUT_H

#include "opengl/qfx_opengl.hpp"
#include "qfx_dynamic_collections.hpp"

namespace QuakeFX
{
	enum class BufferDataTypes
	{
		// Attrib and Attrib I
		Byte = GL_BYTE,
		UnsignedByte = GL_UNSIGNED_BYTE,
		Short = GL_SHORT,
		UnsignedShort = GL_UNSIGNED_SHORT,
		Int = GL_INT,
		UnsignedInt = GL_UNSIGNED_INT,

		// Attrib
		HalfFP = GL_HALF_FLOAT,
		Float = GL_FLOAT,
		Double = GL_DOUBLE // Attrib L
	};

	/// <summary>
	/// Defines a vertex attribute in a vertex layout.
	/// </summary>
	struct QfxVertexElement
	{
		BufferDataTypes type;
		uint count;
		bool normalized;

		/// <summary>
		/// Returns the size of the vertex element specified by the struct.
		/// </summary>
		/// <returns></returns>
		uint GetSize() const;

		static uint GetTypeSize(BufferDataTypes type);
	};

	/// <summary>
	/// Defines the layout of a vertex buffer bound to a VAO.
	/// </summary>
	class QfxVertexLayout
	{
	public:
		/// <summary>
		/// Initializes an empty vertex layout object
		/// </summary>
		QfxVertexLayout();

		/// <summary>
		/// Initializes a new vertex layout using the given initializer list.
		/// </summary>
		QfxVertexLayout(const std::initializer_list<QfxVertexElement>& vertexLayout);

		/// <summary>
		/// Returns a reference to the vertex elements in the layout.
		/// </summary>
		/// <returns></returns>
		const UniqueArray<QfxVertexElement>& GetElements() const;

		/// <summary>
		/// Returns the total size of the vertex specified by the layout.
		/// </summary>
		uint GetStride() const;

	private:
		UniqueArray<QfxVertexElement> elements;
		uint stride;

	};
}

#endif // !QFX_VERTEX_LAYOUT_H
