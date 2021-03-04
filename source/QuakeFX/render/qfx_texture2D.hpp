#ifndef QFX_TEXTURE2D_H
#define QFX_TEXTURE2D_H

#include "render/qfx_image_formats.hpp"
#include "render/qfx_texture.hpp"

namespace QuakeFX
{
	class QfxTexture2D : public QfxTextureBase
	{
	public:
		QfxTexture2D(
			glm::ivec2 dim = glm::ivec2(0,0),
			ImageFormats internalFormat = ImageFormats::RGBA8,
			PixelFormats format = PixelFormats::RGBA,
			PixelTypes pixType = PixelTypes::UByte,
			GLint lod = 0,
			TexInterpFuncs minInterp = TexInterpFuncs::Linear,
			TexInterpFuncs magInterp = TexInterpFuncs::Linear,
			TexWrapPatterns wrapS = TexWrapPatterns::ClampToEdge,
			TexWrapPatterns wrapT = TexWrapPatterns::ClampToEdge,
			GLvoid* imgData = nullptr
		);

		QfxTexture2D(
			glm::ivec2 dim,
			ImageFormats internalFormat,
			PixelFormats format,
			PixelTypes pixType,
			GLint lod,
			TexInterpFuncs interpFunc,
			TexWrapPatterns wrapFunc = TexWrapPatterns::ClampToEdge,
			GLvoid* imgData = nullptr
		);

		QfxTexture2D(const QfxTexture2D& other) = delete;
		QfxTexture2D(QfxTexture2D&& other) noexcept;

		QfxTexture2D& operator=(const QfxTexture2D& rhs) = delete;
		QfxTexture2D& operator=(QfxTexture2D&& rhs) noexcept;

		/// <summary>
		/// Resizes the texture using the given dimensions
		/// </summary>
		void SetSize(glm::ivec2 size);

	private:
		glm::ivec2 dim;
		ImageFormats internalFormat;
		PixelFormats format;
		PixelTypes pixType;
		GLint lod;
		TexInterpFuncs minInterp;
		TexInterpFuncs magInterp;
		TexWrapPatterns wrapS;
		TexWrapPatterns wrapT;
		GLvoid* imgData;
	};
}

#endif // QFX_TEXTURE2D_H
