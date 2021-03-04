#include "render/qfx_texture2D.hpp"

namespace QuakeFX
{
	QfxTexture2D::QfxTexture2D(
		glm::ivec2 dim,
		ImageFormats internalFormat,
		PixelFormats format,
		PixelTypes pixType,
		GLint lod,
		TexInterpFuncs minInterp,
		TexInterpFuncs magInterp,
		TexWrapPatterns wrapS,
		TexWrapPatterns wrapT,
		GLvoid* imgData
	) :
		QfxTextureBase(0, TexTargs::TwoD),
		dim(dim),
		internalFormat(internalFormat),
		format(format),
		pixType(pixType),
		lod(lod),
		minInterp(minInterp),
		magInterp(magInterp),
		wrapS(wrapS),
		wrapT(wrapT),
		imgData(imgData)
	{
		glGenTextures(1, &id);

		if (dim.x > 0 || dim.y > 0)
		{ 
			Bind();

			SetParameter(TexParams::MinFilter, minInterp);
			SetParameter(TexParams::MagFilter, magInterp);
			SetParameter(TexParams::WrapS, wrapS);
			SetParameter(TexParams::WrapT, wrapS);

			glTexImage2D((GLenum)target, lod, (GLint)internalFormat, dim.x, dim.y, 0, (GLenum)format, (GLenum)pixType, imgData);
			Unbind();
		}
	}

	QfxTexture2D::QfxTexture2D(
		glm::ivec2 dim,
		ImageFormats internalFormat,
		PixelFormats format,
		PixelTypes pixType,
		GLint lod,
		TexInterpFuncs interpFunc,
		TexWrapPatterns wrapFunc,
		GLvoid* imgData
	) :
		QfxTexture2D(
			dim,
			internalFormat,
			format,
			pixType,
			lod,
			interpFunc, interpFunc,
			wrapFunc, wrapFunc,
			imgData
		)
	{ }

	QfxTexture2D::QfxTexture2D(QfxTexture2D&& other) noexcept :
		QfxTextureBase(std::move(other)),
		dim(other.dim),
		internalFormat(other.internalFormat),
		format(other.format),
		pixType(other.pixType),
		lod(other.lod),
		minInterp(other.minInterp),
		magInterp(other.magInterp),
		wrapS(other.wrapS),
		wrapT(other.wrapT),
		imgData(other.imgData)
	{ }

	QfxTexture2D& QfxTexture2D::operator=(QfxTexture2D&& rhs) noexcept
	{
		id = rhs.id;
		texUnit = rhs.texUnit;
		target = rhs.target;
		binding = rhs.binding;
		dim = rhs.dim;

		rhs.id = 0;

		return *this;
	}

	/// <summary>
	/// Resizes the texture using the given dimensions
	/// </summary>
	void QfxTexture2D::SetSize(glm::ivec2 size)
	{
		Validate();

		if (dim.x != size.x || dim.y != size.y)
		{
			dim = size;

			SetParameter(TexParams::MinFilter, minInterp);
			SetParameter(TexParams::MagFilter, magInterp);
			SetParameter(TexParams::WrapS, wrapS);
			SetParameter(TexParams::WrapT, wrapS);

			glTexImage2D((GLenum)target, lod, (GLint)internalFormat, dim.x, dim.y, 0, (GLenum)format, (GLenum)pixType, imgData);
		}
	}
}
