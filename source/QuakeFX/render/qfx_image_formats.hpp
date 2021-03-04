#ifndef QFX_COLOR_FORMATS_H
#define QFX_COLOR_FORMATS_H

#include "opengl/qfx_opengl.hpp"

namespace QuakeFX
{
    /// <summary>
    /// Used to specify format of pixel data used by images.
    /// </summary>
    enum class PixelFormats
    {
        BGRA = GL_BGRA,
        BGRA_I = GL_BGRA_INTEGER,
        BGR_I = GL_BGR_INTEGER,
        Depth = GL_DEPTH_COMPONENT,
        DepthStencil = GL_DEPTH_STENCIL,
        RG = GL_RG,
        RGB = GL_RGB,
        RGBA = GL_RGBA,
        RGBA_I = GL_RGBA_INTEGER,
        RGB_I = GL_RGB_INTEGER,
        RG_I = GL_RG_INTEGER,
        Red = GL_RED,
        Red_I = GL_RED_INTEGER,
        Stencil = GL_STENCIL_INDEX,
    };

    /// <summary>
    /// Used to specify the data type of pixel data stored by an image.
    /// </summary>
    enum class PixelTypes
    {
        Byte = GL_BYTE,
        Float = GL_FLOAT,
        Int = GL_INT,
        Short = GL_SHORT,
        UByte = GL_UNSIGNED_BYTE,
        UByte_233R = GL_UNSIGNED_BYTE_2_3_3_REV,
        UByte_332 = GL_UNSIGNED_BYTE_3_3_2,
        UInt = GL_UNSIGNED_INT,
        UInt_10_10_10_2 = GL_UNSIGNED_INT_10_10_10_2,
        UInt_10_10_10_2R = GL_UNSIGNED_INT_2_10_10_10_REV,
        UInt_8888 = GL_UNSIGNED_INT_8_8_8_8,
        UInt_8888R = GL_UNSIGNED_INT_8_8_8_8_REV,
        UShort = GL_UNSIGNED_SHORT,
        UShort_1555R = GL_UNSIGNED_SHORT_1_5_5_5_REV,
        UShort_4444 = GL_UNSIGNED_SHORT_4_4_4_4,
        UShort_4444R = GL_UNSIGNED_SHORT_4_4_4_4_REV,
        UShort_5551 = GL_UNSIGNED_SHORT_5_5_5_1,
        UShort_565 = GL_UNSIGNED_SHORT_5_6_5,
        UShort_565R = GL_UNSIGNED_SHORT_5_6_5_REV,
    };

    /// <summary>
    /// Internal image format enums used to describe the way images in Textures and render buffers
    /// store their data, including color, depth and stencil formats.
    /// </summary>
    enum class ImageFormats
    {
        /// <summary>
        /// Unsigned normalized format. FP values on interval [0.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>		
        R8 = GL_R8,
        /// <summary>
        /// Unsigned normalized format. FP values on interval [0.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>		
        R16 = GL_R16,
        /// <summary>
        /// Unsigned normalized format. FP values on interval [0.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>		
        /// <summary>
        /// Unsigned normalized format. FP values on interval [0.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>		
        RG8 = GL_RG8,
        /// <summary>
        /// Unsigned normalized format. FP values on interval [0.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>		
        RG16 = GL_RG16,
        /// <summary>
        /// Unsigned normalized format. FP values on interval [0.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>		
        RGB4 = GL_RGB4,
        /// <summary>
        /// Unsigned normalized format. FP values on interval [0.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>		
        RGB5 = GL_RGB5,
        /// <summary>
        /// Unsigned normalized format. FP values on interval [0.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>		
        RGB8 = GL_RGB8,
        /// <summary>
        /// Unsigned normalized format. FP values on interval [0.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>		
        RGB10 = GL_RGB10,
        /// <summary>
        /// Unsigned normalized format. FP values on interval [0.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>		
        RGB12 = GL_RGB12,
        /// <summary>
        /// Unsigned normalized format. FP values on interval [0.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>		
        RGB16 = GL_RGB16,
        /// <summary>
        /// Unsigned normalized format. FP values on interval [0.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>		
        RGBA2 = GL_RGBA2,
        /// <summary>
        /// Unsigned normalized format. FP values on interval [0.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>		
        RGBA4 = GL_RGBA4,
        /// <summary>
        /// Unsigned normalized format. FP values on interval [0.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>		
        RGB5_A1 = GL_RGB5_A1,
        /// <summary>
        /// Unsigned normalized format. FP values on interval [0.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>		
        RGBA8 = GL_RGBA8,
        /// <summary>
        /// Unsigned normalized format. FP values on interval [0.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>		
        RGB10_A2 = GL_RGB10_A2,
        /// <summary>
        /// Unsigned normalized format. FP values on interval [0.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>		
        RGBA12 = GL_RGBA12,
        /// <summary>
        /// Unsigned normalized format. FP values on interval [0.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>	
        RGBA16 = GL_RGBA16,

        /// <summary>
        /// Signed normalized formats. FP values on interval [-1.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>
        SNormR8 = GL_R8_SNORM,
        /// <summary>
        /// Signed normalized formats. FP values on interval [-1.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>
        SNormR16 = GL_R16_SNORM,
        /// <summary>
        /// Signed normalized formats. FP values on interval [-1.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>
        /// <summary>
        /// Signed normalized formats. FP values on interval [-1.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>
        SNormRG8 = GL_RG8_SNORM,
        /// <summary>
        /// Signed normalized formats. FP values on interval [-1.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>
        SNormRG16 = GL_RG16_SNORM,
        /// <summary>
        /// Signed normalized formats. FP values on interval [-1.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>
        SNormRGB16 = GL_RGB16_SNORM,
        /// <summary>
        /// Signed normalized formats. FP values on interval [-1.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>
        SNormRGBA8 = GL_RGBA8_SNORM,
        /// <summary>
        /// Signed normalized formats. FP values on interval [-1.0, 1.0].
        /// R/G/B/A indicates components in the vector formats (Red, Green, Blue, 
        /// Alpha). Numbers following indicate bitness.
        /// </summary>
        SNormRGBA16 = GL_RGBA16_SNORM,

        /// <summary>
        /// Signed integer formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        SIntR8 = GL_R8I,
        /// <summary>
        /// Signed integer formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        SIntR16 = GL_R16I,
        /// <summary>
        /// Signed integer formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        /// <summary>
        /// Signed integer formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        SIntRG8 = GL_RG8I,
        /// <summary>
        /// Signed integer formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        SIntRG16 = GL_RG16I,
        /// <summary>
        /// Signed integer formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        SIntRGB8 = GL_RGB8I,
        /// <summary>
        /// Signed integer formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        SIntRGB16 = GL_RGB16I,
        /// <summary>
        /// Signed integer formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        SIntRGBA8 = GL_RGBA8I,
        /// <summary>
        /// Signed integer formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        SIntRGBA16 = GL_RGBA16I,

        /// <summary>
        /// Signed integer formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        UIntR8 = GL_R8UI,
        /// <summary>
        /// Signed integer formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        UIntR16 = GL_R16UI,
        /// <summary>
        /// Signed integer formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        /// <summary>
        /// Signed integer formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        UIntRG8 = GL_RG8UI,
        /// <summary>
        /// Signed integer formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        UIntRG16 = GL_RG16UI,
        /// <summary>
        /// Signed integer formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        UIntRGB8 = GL_RGB8UI,
        /// <summary>
        /// Signed integer formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        UIntRGB16 = GL_RGB16UI,
        /// <summary>
        /// Signed integer formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        UIntRGBA8 = GL_RGBA8UI,
        /// <summary>
        /// Signed integer formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        UIntRGB10_A2 = GL_RGB10_A2UI,
        /// <summary>
        /// Signed integer formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        UIntRGBA16 = GL_RGBA16UI,

        /// <summary>
        /// Non-normalized FP formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        R16F = GL_R16F,
        /// <summary>
        /// Non-normalized FP formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        RG16F = GL_RG16F,
        /// <summary>
        /// Non-normalized FP formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        RGB16F = GL_RGB16F,
        /// <summary>
        /// Non-normalized FP formats. R/G/B/A indicates components in the vector formats 
        /// (Red, Green, Blue, Alpha). Numbers following indicate bitness.
        /// </summary>
        RGBA16F = GL_RGBA16F,

        /// <summary>
        /// 16-bit depth buffer format using normalized values on [0.0, 1.0].
        /// </summary>
        Depth16 = GL_DEPTH_COMPONENT16,
        /// <summary>
        /// 24-bit depth buffer format using normalized values on [0.0, 1.0].
        /// </summary>
        Depth24 = GL_DEPTH_COMPONENT24,
        /// <summary>
        /// 32-bit depth buffer format using normalized values on [0.0, 1.0].
        /// </summary>
        Depth32 = GL_DEPTH_COMPONENT32,
        /// <summary>
        /// 32-bit fp depth buffer format
        /// </summary>
        Depth32F = GL_DEPTH_COMPONENT32F,

        /// <summary>
        /// Stencil test image format with a bitdepth of 1
        /// </summary>
        Stencil1 = GL_STENCIL_INDEX1,
        /// <summary>
        /// Stencil test image format with a bitdepth of 4
        /// </summary>
        Stencil4 = GL_STENCIL_INDEX4,
        /// <summary>
        /// Stencil test image format with a bitdepth of 8
        /// </summary>
        Stencil8 = GL_STENCIL_INDEX8,
        /// <summary>
        /// Stencil test image format with a bitdepth of 16
        /// </summary>
        Stencil16 = GL_STENCIL_INDEX16,

        Depth24Stencil8 = GL_DEPTH24_STENCIL8,

        Depth32fStencil8 = GL_DEPTH32F_STENCIL8,
    };
}

#endif