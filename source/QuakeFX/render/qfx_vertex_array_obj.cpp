#include "render/qfx_vertex_layout.hpp"
#include "render/qfx_vertex_array_obj.hpp"

namespace QuakeFX
{
	void QfxVertexArrayObj::Validate() const
	{
#ifndef NDEBUG
		if (!IsBound())
			REPORTERROR("Vertex array objects must be bound before they can be used.")
#endif // !NDEBUG
	}

	/// <summary>
	/// Initializes a new vertex array object. Unbound by default.
	/// </summary>
	QfxVertexArrayObj::QfxVertexArrayObj()
	{
		glGenVertexArrays(1, &id);
	}

	QfxVertexArrayObj::QfxVertexArrayObj(QfxVertexArrayObj&& other) noexcept:
		id(other.id)
	{
		other.id = 0;
	}

	QfxVertexArrayObj::~QfxVertexArrayObj()
	{
		if (id != 0)
		{
			glDeleteVertexArrays(1, &id);
		}
	}

	QfxVertexArrayObj& QfxVertexArrayObj::operator=(QfxVertexArrayObj&& rhs) noexcept
	{
		id = rhs.id;
		rhs.id = 0;

		return *this;
	}

	/// <summary>
	/// Returns the name of the vertex array object.
	/// </summary>
	uint QfxVertexArrayObj::GetID() const
	{
		return id;
	}

	/// <summary>
	/// Returns true if the vertex array object is bound to the current context.
	/// </summary>
	bool QfxVertexArrayObj::IsBound() const
	{
		return id != 0 && GetCurrentVAO() == id;
	}

	/// <summary>
	/// Binds the VAO to the current context.
	/// </summary>
	void QfxVertexArrayObj::Bind()
	{
		glBindVertexArray(id);
	}

	/// <summary>
	/// Unbinds the array from the current context.
	/// </summary>
	void QfxVertexArrayObj::Unbind()
	{
		Validate();
		glBindVertexArray(0);
	}

	/// <summary>
	/// Sets the vertex layout for the VAO.
	/// </summary>
	void QfxVertexArrayObj::SetLayout(const QfxVertexLayout& layout)
	{
		Validate();

		const UniqueArray<QfxVertexElement>& elements = layout.GetElements();
		uint offset = 0;

		for (size_t n = 0; n < elements.GetLength(); n++)
		{
			glEnableVertexAttribArray(n);
			glVertexAttribPointer(n, elements[n].count, (GLint)elements[n].type,
				(elements[n].normalized ? GL_TRUE : GL_FALSE), layout.GetStride(), (const void*)offset);

			offset += elements[n].GetSize();
		}
	}

	/// <summary>
	/// Binds the vertex array object associated with the given id to the current rendering context.
	/// </summary>
	/// <param name="id"></param>
	void QfxVertexArrayObj::BindVAO(GLuint id)
	{
		glBindVertexArray(id);
	}

	/// <summary>
	/// Unbinds the vertex array object currently bound from the rendering context.
	/// </summary>
	void QfxVertexArrayObj::UnbindCurrentVAO()
	{
		glBindVertexArray(0);
	}

	/// <summary>
	/// Retrives the name of the vertex array object currently bound
	/// </summary>
	GLuint QfxVertexArrayObj::GetCurrentVAO()
	{
		int name;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &name);

		return (uint)name;
	}
}
