#ifndef QFX_VERTEX_ARRAYOBJ_H
#define QFX_VERTEX_ARRAYOBJ_H

#include "opengl/qfx_opengl.hpp"
#include "qfx_dynamic_collections.hpp"

namespace QuakeFX
{
	class QfxVertexLayout;

	class QfxVertexArrayObj
	{
	public:
		/// <summary>
		/// Initializes a new vertex array object. Unbound by default.
		/// </summary>
		QfxVertexArrayObj();

		QfxVertexArrayObj(const QfxVertexArrayObj& other) = delete;

		QfxVertexArrayObj(QfxVertexArrayObj&& other) noexcept;

		~QfxVertexArrayObj();

		QfxVertexArrayObj& operator=(const QfxVertexArrayObj& rhs) = delete;

		QfxVertexArrayObj& operator=(QfxVertexArrayObj&& rhs) noexcept;

		/// <summary>
		/// Returns the name of the vertex array object.
		/// </summary>
		uint GetID() const;

		/// <summary>
		/// Returns true if the vertex array object is bound to the current context.
		/// </summary>
		bool IsBound() const;

		/// <summary>
		/// Binds the VAO to the current context.
		/// </summary>
		void Bind();

		/// <summary>
		/// Unbinds the array from the current context.
		/// </summary>
		void Unbind();

		/// <summary>
		/// Sets the vertex layout for the VAO.
		/// </summary>
		void SetLayout(const QfxVertexLayout& layout);

		/// <summary>
		/// Binds the vertex array object associated with the given id to the current rendering context.
		/// </summary>
		/// <param name="id"></param>
		static void BindVAO(GLuint id);

		/// <summary>
		/// Unbinds the vertex array object currently bound from the rendering context.
		/// </summary>
		static void UnbindCurrentVAO();

		/// <summary>
		/// Retrives the name of the vertex array object currently bound
		/// </summary>
		static GLuint GetCurrentVAO();

	private:
		uint id;

		void Validate() const;
	};
}

#endif // !QFX_VERTEX_ARRAYOBJ_H
