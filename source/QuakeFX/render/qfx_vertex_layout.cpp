#include <string>
#include "render/qfx_vertex_layout.hpp"

namespace QuakeFX
{
	/// <summary>
	/// Returns the size of the vertex element specified by the struct.
	/// </summary>
	uint QfxVertexElement::GetSize() const
	{
		return count * GetTypeSize(type);
	}

	/// <summary>
	/// Returns the size of the type specified by the enum in bytes.
	/// </summary>
	uint QfxVertexElement::GetTypeSize(BufferDataTypes type)
	{
		switch (type)
		{
		case BufferDataTypes::Byte:
			return 1;
		case BufferDataTypes::UnsignedByte:
			return 1;
		case BufferDataTypes::Short:
			return 2;
		case BufferDataTypes::UnsignedShort:
			return 2;
		case BufferDataTypes::Int:
			return 4;
		case BufferDataTypes::UnsignedInt:
			return 4;
		case BufferDataTypes::HalfFP:
			return 2;
		case BufferDataTypes::Float:
			return 4;
		case BufferDataTypes::Double:
			return 8;
		default:
			REPORTERROR("Unsupported data type")
			return 0;
		}
	}

	QfxVertexLayout::QfxVertexLayout() :
		stride(0)
	{ }

	/// <summary>
	/// Initializes a new vertex layout using the given initializer list.
	/// </summary>
	QfxVertexLayout::QfxVertexLayout(const std::initializer_list<QfxVertexElement>& vertexLayout) :
		elements(UniqueArray<QfxVertexElement>(vertexLayout)),
		stride(0)
	{
		for (size_t n = 0; n < elements.GetLength(); n++)
		{
			stride += elements[n].GetSize();
		}
	}
		/// <summary>
	/// Returns a reference to the vertex elements in the layout.
	/// </summary>
	const UniqueArray<QfxVertexElement>& QfxVertexLayout::GetElements() const
	{
		return elements;
	}

	/// <summary>
	/// Returns the total size of the vertex specified by the layout.
	/// </summary>
	uint QfxVertexLayout::GetStride() const
	{
		return stride;
	}
}
