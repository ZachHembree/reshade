#ifndef QFX_TEXTURE_H
#define QFX_TEXTURE_H

#include <glm/glm.hpp>
#include "opengl/qfx_opengl.hpp"

namespace QuakeFX
{
	enum class TexTargs
	{
		Invalid = -1,
		OneD = GL_TEXTURE_1D,
		TwoD = GL_TEXTURE_2D,
		ThreeD = GL_TEXTURE_3D,
		OneDArray = GL_TEXTURE_1D_ARRAY,
		TwoDArray = GL_TEXTURE_2D_ARRAY,
		Rectangle = GL_TEXTURE_RECTANGLE,
		CubeMap = GL_TEXTURE_CUBE_MAP,
		CubeMapArray = GL_TEXTURE_CUBE_MAP_ARRAY,
		Buffer = GL_TEXTURE_BUFFER,
		TwoDMultisample = GL_TEXTURE_2D_MULTISAMPLE,
		TwoDMultisampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
	};

	enum class TexBindings
	{
		Invalid = -1,
		OneD = GL_TEXTURE_BINDING_1D,
		TwoD = GL_TEXTURE_BINDING_2D,
		ThreeD = GL_TEXTURE_BINDING_3D,
		OneDArray = GL_TEXTURE_BINDING_1D_ARRAY,
		TwoDArray = GL_TEXTURE_BINDING_2D_ARRAY,
		Rectangle = GL_TEXTURE_BINDING_RECTANGLE,
		CubeMap = GL_TEXTURE_BINDING_CUBE_MAP,
		CubeMapArray = GL_TEXTURE_BINDING_CUBE_MAP_ARRAY,
		Buffer = GL_TEXTURE_BINDING_BUFFER,
		TwoDMultisample = GL_TEXTURE_BINDING_2D_MULTISAMPLE,
		TwoDMultisampleArray = GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY
	};

	enum class TexParams
	{
		/// <summary>
		/// Specifies the mode used to read from depth-stencil format textures.
		/// </summary>
		DepthStencilMode = GL_DEPTH_STENCIL_TEXTURE_MODE,

		/// <summary>
		/// Specifies the index of the lowest defined mipmap level. This is an 
		/// integer value. The initial value is 0.
		/// </summary>
		BaseLevel = GL_TEXTURE_BASE_LEVEL,

		/// <summary>
		/// Specifies the comparison operator used when GL_TEXTURE_COMPARE_MODE 
		/// is set to GL_COMPARE_REF_TO_TEXTURE. 
		/// </summary>
		CompareFunc = GL_TEXTURE_COMPARE_FUNC,

		/// <summary>
		/// Specifies the texture comparison mode for currently bound depth 
		/// textures. 
		/// </summary>
		CompareMode = GL_TEXTURE_COMPARE_MODE,

		/// <summary>
		/// Specifies a fixed bias value that is to be added to the 
		/// level-of-detail parameter for the texture before texture sampling. 
		/// </summary>
		LodBias = GL_TEXTURE_LOD_BIAS,

		/// <summary>
		/// Texture minifying function used whenever the level-of-detail 
		/// function used when sampling from the texture determines that the 
		/// texture should be minified.
		/// </summary>
		MinFilter = GL_TEXTURE_MIN_FILTER,

		/// <summary>
		/// Texture magnification function used whenever the level-of-detail 
		/// function used when sampling from the texture determines that the 
		/// texture should be magified.
		/// </summary>
		MagFilter = GL_TEXTURE_MAG_FILTER,

		/// <summary>
		/// Sets the minimum level-of-detail parameter. This floating-point value 
		/// limits the selection of highest resolution mipmap (lowest mipmap level).
		/// The initial value is -1000.
		/// </summary>
		MinLod = GL_TEXTURE_MIN_LOD,

		/// <summary>
		/// Sets the maximum level-of-detail parameter. This floating-point value 
		/// limits the selection of the lowest resolution mipmap (highest mipmap level). 
		/// The initial value is 1000.
		/// </summary>
		MaxLod = GL_TEXTURE_MAX_LOD,

		/// <summary>
		/// Sets the index of the highest defined mipmap level. This is an integer value. 
		/// The initial value is 1000.
		/// </summary>
		MaxLevel = GL_TEXTURE_MAX_LEVEL,

		/// <summary>
		/// Sets the wrap parameter for texture coordinate S/U.
		/// </summary>
		WrapS = GL_TEXTURE_WRAP_S,

		/// <summary>
		/// Sets the wrap parameter for texture coordinate T/V.
		/// </summary>
		WrapT = GL_TEXTURE_WRAP_T,

		/// <summary>
		/// Sets the wrap parameter for texture coordinate R.
		/// </summary>
		WrapR = GL_TEXTURE_WRAP_R,

		// glTexParameter*v
		/// <summary>
		/// The data in params specifies four values that define the border values 
		/// that should be used for border texels.
		/// </summary>
		BorderColor = GL_TEXTURE_BORDER_COLOR,
	};

	enum class TexInterpFuncs
	{
		/// <summary>
		/// Returns the value of the texture element that is nearest to the 
		/// specified texture coordinates.
		/// </summary>
		Nearest = GL_NEAREST,

		/// <summary>
		/// Returns the weighted average of the four texture elements that are 
		/// closest to the specified texture coordinates.
		/// </summary>
		Linear = GL_LINEAR,

		/// <summary>
		/// Chooses the mipmap that most closely matches the size of the pixel 
		/// being textured and uses the GL_NEAREST.
		/// </summary>
		NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,

		/// <summary>
		/// Chooses the mipmap that most closely matches the size of the pixel 
		/// being textured and uses the GL_LINEAR.
		/// </summary>
		LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,

		/// <summary>
		/// Chooses the two mipmaps that most closely match the size of the pixel 
		/// being textured and interpolates between the two.
		/// </summary>
		NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,

		/// <summary>
		/// Chooses the two mipmaps that most closely match the size of the pixel 
		/// being textured and uses GL_LINEAR to calculate the value at the same 
		/// location on both textures and interpolates between them to find the final value.
		/// </summary>
		LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
	};

	enum class TexWrapPatterns
	{
		/// <summary>
		/// Causes texture sampling on the given axis to be clamped between 
		/// [1 - 1/2N, 1/2N] where N is the size of the texture on the axis.
		/// </summary>
		ClampToEdge = GL_CLAMP_TO_EDGE,

		/// <summary>
		/// Similar to ClampToEdge, but instead of clamping the sampling range, whenever
		/// a value sampled is out of bounds, the border color is returned instead.
		/// </summary>
		ClampToBorder = GL_CLAMP_TO_BORDER,

		/// <summary>
		/// Only the fractional part of the texture coordinate is used, creating a repeating
		/// pattern. Default wrapping behavior.
		/// </summary>
		Repeat = GL_REPEAT,

		/// <summary>
		/// Like Repeat, this creates a repeating pattern, but instead of repeating the unaltered
		/// texture, the direction of the texture on the axis is reversed in an alternating pattern.
		/// </summary>
		MirroredRepeat = GL_MIRRORED_REPEAT
	};

	class QfxTextureBase
	{
	public:
		bool IsActive() const;

		/// <summary>
		/// Returns true if the texture is currently bound to a texture slot
		/// </summary>
		bool IsBound() const;

		GLuint GetID();

		/// <summary>
		/// Binds the texture to the given slot
		/// </summary>
		void Bind(GLenum slot = 0);

		/// <summary>
		/// Binds the texture associated with the given id to the give slot.
		/// </summary>
		static void BindTexture(TexTargs target, GLuint id, GLenum slot);

		/// <summary>
		/// Unbinds the texture at given target
		/// </summary>
		static void UnbindTexture(TexTargs target, GLuint id, GLenum slot);

		/// <summary>
		/// Unbinds the 
		/// </summary>
		void Unbind();

		/// <summary>
		/// Sets the interp function for the given param
		/// </summary>
		void SetParameter(TexParams param, TexInterpFuncs value);

		/// <summary>
		/// Sets the wrap pattern for the given param
		/// </summary>
		void SetParameter(TexParams param, TexWrapPatterns value);

		/// <summary>
		/// Retrieves the texture binding associated with the given target
		/// </summary>
		static TexBindings GetTextureBinding(TexTargs target);

		/// <summary>
		/// Returns the index of the active texture unit.
		/// </summary>
		static GLenum GetActiveTextureUnit();

		/// <summary>
		/// Returns the name of the texture bound to the target
		/// </summary>
		static GLuint GetCurrentTexture(TexBindings binding);

		QfxTextureBase& operator=(QfxTextureBase&& rhs) noexcept;

	protected:
		uint id;
		GLenum texUnit;
		TexTargs target;
		TexBindings binding;

		QfxTextureBase(uint texUnit, TexTargs target);
		QfxTextureBase(QfxTextureBase&& other) noexcept;
		QfxTextureBase() = delete;
		~QfxTextureBase();

		void Validate() const;

		static void Validate(TexBindings binding, GLuint id, GLenum texUnit);
	};
}

#endif // !QFX_TEXTURE_H
