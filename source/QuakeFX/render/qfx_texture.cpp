#include <string>
#include "render/qfx_texture.hpp"

namespace QuakeFX
{
	QfxTextureBase::QfxTextureBase(uint texUnit, TexTargs target) :
		id(0),
		texUnit(texUnit),
		target(target),
		binding(GetTextureBinding(target))
	{ }

	QfxTextureBase::QfxTextureBase(QfxTextureBase&& other) noexcept :
		id(other.id),
		texUnit(other.texUnit),
		target(other.target),
		binding(other.binding)
	{
		other.id = 0;
	}

	QfxTextureBase::~QfxTextureBase()
	{
		if (id != 0)
			glDeleteTextures(1, &id);
	}

	QfxTextureBase& QfxTextureBase::operator=(QfxTextureBase&& rhs) noexcept
	{
		id = rhs.id;
		texUnit = rhs.texUnit;
		target = rhs.target;
		binding = rhs.binding;

		rhs.id = 0;

		return *this;
	}

	bool QfxTextureBase::IsActive() const
	{
		return IsBound() && GetActiveTextureUnit() == texUnit;
	}

	bool QfxTextureBase::IsBound() const
	{
		return GetCurrentTexture(binding) == id;
	}

	GLuint QfxTextureBase::GetID() { return id; }

	void QfxTextureBase::Bind(GLenum slot)
	{
		texUnit = slot;
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture((GLenum)target, id);
	}

	void QfxTextureBase::BindTexture(TexTargs target, GLuint id, GLenum slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture((GLenum)target, id);
	}

	void QfxTextureBase::UnbindTexture(TexTargs target, GLuint id, GLenum slot)
	{
		Validate(GetTextureBinding(target), id, slot);
		glBindTexture((GLenum)target, 0);
	}

	void QfxTextureBase::Unbind()
	{
		Validate();
		glBindTexture((GLenum)target, 0);
	}

	void QfxTextureBase::SetParameter(TexParams param, TexInterpFuncs value)
	{
		Validate();
		glTexParameteri((GLenum)target, (GLenum)param, (GLint)value);
	}

	void QfxTextureBase::SetParameter(TexParams param, TexWrapPatterns value)
	{
		Validate();
		glTexParameteri((GLenum)target, (GLenum)param, (GLint)value);
	}

	GLenum QfxTextureBase::GetActiveTextureUnit()
	{
		GLint unit;
		glGetIntegerv(GL_ACTIVE_TEXTURE, &unit);

		return (GLenum)(unit - GL_TEXTURE0);
	}

	GLuint QfxTextureBase::GetCurrentTexture(TexBindings binding)
	{
		GLint name;
		glGetIntegerv((GLint)binding, &name);

		return (GLuint)name;
	}

	void QfxTextureBase::Validate() const
	{
#ifndef NDEBUG
		if (!IsActive())
			REPORTERROR("The texture must be active and bound before it can be used.")
#endif // !NDEBUG
	}

	void QfxTextureBase::Validate(TexBindings binding, GLuint id, GLenum texUnit)
	{
#ifndef NDEBUG
		if (GetCurrentTexture(binding) != id || GetActiveTextureUnit() != texUnit)
			REPORTERROR("The texture must be active and bound before it can be used.")
#endif // !NDEBUG
	}

	TexBindings QfxTextureBase::GetTextureBinding(TexTargs target)
	{
		switch (target)
		{
		case TexTargs::OneD:
			return TexBindings::OneD;
		case TexTargs::TwoD:
			return TexBindings::TwoD;
		case TexTargs::ThreeD:
			return TexBindings::ThreeD;
		case TexTargs::OneDArray:
			return TexBindings::OneDArray;
		case TexTargs::TwoDArray:
			return TexBindings::TwoDArray;
		case TexTargs::Rectangle:
			return TexBindings::Rectangle;
		case TexTargs::CubeMap:
			return TexBindings::CubeMap;
		case TexTargs::CubeMapArray:
			return TexBindings::CubeMapArray;
		case TexTargs::Buffer:
			return TexBindings::Buffer;
		case TexTargs::TwoDMultisample:
			return TexBindings::TwoDMultisample;
		case TexTargs::TwoDMultisampleArray:
			return TexBindings::TwoDMultisampleArray;
		default:
			return TexBindings::Invalid;
		}
	}
}
