#ifndef QFX_FRAMEBUFFER_OBJ_H
#define QFX_FRAMEBUFFER_OBJ_H

#include <string>
#include "opengl/qfx_opengl.hpp"
#include "render/qfx_texture.hpp"
#include "render/qfx_render_buffer.hpp"

namespace QuakeFX
{
	/// <summary>
	/// Bit masks used to target specific buffers
	/// </summary>
	enum class FramebufferMasks
	{
		Color = GL_COLOR_BUFFER_BIT,
		Depth = GL_DEPTH_BUFFER_BIT,
		Stencil = GL_STENCIL_BUFFER_BIT,
		ColorDepth = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
		DepthStencil = GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT,
		ColorDepthStencil = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT,
	};

	/// <summary>
	/// OpenGL framebuffer targets. Used to specify whether a framebuffer should
	/// be used for draw/read
	/// </summary>
	enum class FramebufferTargs
	{
		Invalid = 0,

		/// <summary>
		/// Bind framebuffer to become the target for rendering
		/// </summary>
		Draw = GL_DRAW_FRAMEBUFFER,

		/// <summary>
		/// Bind framebuffer to become the target for read back operations
		/// </summary>
		Read = GL_READ_FRAMEBUFFER,

		/// <summary>
		/// Bind framebuffer for both draw and read
		/// </summary>
		Framebuffer = GL_FRAMEBUFFER
	};

	/// <summary>
	/// Framebuffer states returned by glCheckFramebufferStatus
	/// </summary>
	enum class FramebufferStates
	{
		Unknown = 0,

		Complete = GL_FRAMEBUFFER_COMPLETE,

		/// <summary>
		/// Returned if the specified framebuffer is the default read or draw framebuffer, 
		/// but the default framebuffer does not exist.
		/// </summary>
		Undefined = GL_FRAMEBUFFER_UNDEFINED,

		/// <summary>
		/// Returned if any of the framebuffer attachment points are framebuffer incomplete.
		/// </summary>
		IncompleteAttach = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,

		/// <summary>
		/// Returned if the framebuffer does not have at least one image attached to it.
		/// </summary>
		MissingAttach = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,

		/// <summary>
		/// Returned if the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for 
		/// any color attachment point(s) named by GL_DRAW_BUFFERi.
		/// </summary>
		IncompleteDrawBuffer = GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,

		/// <summary>
		/// Returned if GL_READ_BUFFER is not GL_NONE and the value of 
		/// GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point 
		/// named by GL_READ_BUFFER.
		/// </summary>
		IncompleteReadBuffer = GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER,

		/// <summary>
		/// Returned if the combination of internal formats of the attached images violates 
		/// an implementation-dependent set of restrictions.
		/// </summary>
		Unsupported = GL_FRAMEBUFFER_UNSUPPORTED,

		/// <summary>
		/// Returned if the value of GL_RENDERBUFFER_SAMPLES is not the same for all attached 
		/// renderbuffers; if the value of GL_TEXTURE_SAMPLES is the not same for all attached 
		/// textures; or, if the attached images are a mix of renderbuffers and textures, the 
		/// value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES.
		/// 
		/// Also returned if the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same 
		/// for all attached textures; or, if the attached images are a mix of renderbuffers 
		/// and textures, the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for 
		/// all attached textures.
		/// </summary>
		IncompleteMultisample = GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,

		/// <summary>
		/// Returned if any framebuffer attachment is layered, and any populated attachment is
		/// not layered, or if all populated color attachments are not from textures of the 
		/// same target.
		/// </summary>
		IncompleteLayerTargets = GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS
	};

	/// <summary>
	/// Framebuffer attachments used to determine whether a given texture/render buffer
	/// is used as a color, depth or stencil buffer.
	/// </summary>
	enum class FramebufferAttachments
	{
		/// <summary>
		/// Used to specify a buffer being used as an FBO's color buffer. FBO's support
		/// multiple color attachments ranging from #0 to GL_MAX_COLOR_ATTACHMENTS - 1.
		/// </summary>
		Color = GL_COLOR_ATTACHMENT0,

		/// <summary>
		/// Used to indicate a buffer being used for depth testing.
		/// </summary>
		Depth = GL_DEPTH_ATTACHMENT,

		/// <summary>
		/// Used to indicate a buffer being used for stencil testing.
		/// </summary>
		Stencil = GL_STENCIL_ATTACHMENT,

		/// <summary>
		/// Used to indicate a buffer being used for both depth and stencil testing.
		/// </summary>
		DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT
	};

	/// <summary>
	/// Color buffers that can be drawn into
	/// </summary>
	enum class ColorBuffers
	{
		None = GL_NONE,
		FrontLeft = GL_FRONT_LEFT,
		FrontRight = GL_FRONT_RIGHT,
		BackLeft = GL_BACK_LEFT,
		BackRight = GL_BACK_RIGHT,
		Front = GL_FRONT,
		Back = GL_BACK,
		Left = GL_LEFT,
		Right = GL_RIGHT,
		FrontAndBack = GL_FRONT_AND_BACK,

		/// <summary>
		/// Used in conjunction with FBOs
		/// </summary>
		ColorAttach = GL_COLOR_ATTACHMENT0,
	};

	/// <summary>
	/// Move only wrapper for OpenGL framebuffer objects
	/// </summary>
	class QfxFramebufferObj
	{
	public:
		QfxFramebufferObj();

		QfxFramebufferObj(const QfxFramebufferObj& other) = delete;

		~QfxFramebufferObj();

		QfxFramebufferObj& operator=(const QfxFramebufferObj& rhs) = delete;

		/// <summary>
		/// Returns the internal name of the FBO
		/// </summary>
		GLuint GetID() const;

		/// <summary>
		/// Returns true if the FBO is bound for either read or draw
		/// </summary>
		bool GetIsBound() const;

		/// <summary>
		/// Returns true if the framebuffer is complete.
		/// </summary>
		bool GetIsComplete() const;

		/// <summary>
		/// Binds the FBO to the rendering context
		/// </summary>
		void Bind(FramebufferTargs newTarget);

		/// <summary>
		/// Unbinds the FBO from the rendering context
		/// </summary>
		void Unbind();

		/// <summary>
		/// Attaches a texture object as a logical buffer to the FBO
		/// </summary>
		void SetFrameBufferRenderBuffer(QfxRenderBuffer& texture, FramebufferAttachments attach, FramebufferTargs buffTarg = FramebufferTargs::Framebuffer, uint colorAttachNum = 0);

		/// <summary>
		/// Attaches a texture object as a logical buffer to the FBO
		/// </summary>
		void SetFramebufferTexture(QfxTextureBase& texture, FramebufferTargs buffTarg, FramebufferAttachments attach, uint colorAttachNum = 0, GLint mipLevel = 0);

		/// <summary>
		/// Returns the status of the framebuffer when treated as if bound to its target.
		/// </summary>
		FramebufferStates GetStatus() const;

		/// <summary>
		/// Returns framebuffer status as a string
		/// </summary>
		const char* const GetStatusString() const;

		/// <summary>
		/// Clears data from the FBO based on the given bitmask
		/// </summary>
		void Clear(FramebufferMasks bitMask);

		/// <summary>
		/// Copies the contents of the FBO, using the specified color buffer, into the
		/// given destination framebuffer.
		/// </summary>
		void BlitTo(
			ColorBuffers srcColor,
			GLuint dstName,
			ColorBuffers dstColor,
			glm::ivec2 size,
			FramebufferMasks bufMask = FramebufferMasks::Color,
			bool autoUnbind = true,
			bool useNearestNeighbor = false
		);

		/// <summary>
		/// Copies the contents of the given framebuffer into the FBO, using the 
		/// specified color buffer.
		/// </summary>
		void BlitFrom(
			ColorBuffers dstColor,
			GLuint srcName,
			ColorBuffers srcColor,
			glm::ivec2 size,
			FramebufferMasks bufMask = FramebufferMasks::Color,
			bool autoUnbind = true,
			bool useNearestNeighbor = false
		);

		/// <summary>
		/// Copies the contents of the given read buffer to the given draw buffer.
		/// </summary>
		static void Blit(
			GLuint srcName,
			ColorBuffers srcColor,
			GLuint dstName,
			ColorBuffers dstColor,
			glm::ivec2 size,
			FramebufferMasks bufMask = FramebufferMasks::Color,
			bool useNearestNeighbor = false
		);

		/// <summary>
		/// Copies the contents of the given read buffer to the given draw buffer.
		/// </summary>
		static void Blit(
			GLuint srcName,
			ColorBuffers srcColor,
			glm::ivec2 srcSize,
			GLuint dstName,
			ColorBuffers dstColor,
			glm::ivec2 dstSize,
			FramebufferMasks bufMask = FramebufferMasks::Color,
			bool useNearestNeighbor = false
		);

		/// <summary>
		/// Copies the contents of the given read buffer to the given draw buffer.
		/// </summary>
		static void Blit(
			GLuint srcName,
			ColorBuffers srcColor,
			glm::ivec2 srcOrigin,
			glm::ivec2 srcSize,
			GLuint dstName,
			ColorBuffers dstColor,
			glm::ivec2 dstOrigin,
			glm::ivec2 dstSize,
			FramebufferMasks bufMask = FramebufferMasks::Color,
			bool useNearestNeighbor = false
		);

		/// <summary>
		/// Copies the contents of the read buffer currently bound into the bound draw buffer.
		/// </summary>
		static void Blit(
			glm::ivec2 size,
			FramebufferMasks bufMask = FramebufferMasks::Color,
			bool useNearestNeighbor = false
		);

		/// <summary>
		/// Copies the contents of the read buffer currently bound into the bound draw buffer.
		/// </summary>
		static void Blit(
			glm::ivec2 srcSize,
			glm::ivec2 dstSize,
			FramebufferMasks bufMask = FramebufferMasks::Color,
			bool useNearestNeighbor = false
		);

		/// <summary>
		/// Copies the contents of the read buffer currently bound into the bound draw buffer.
		/// </summary>
		static void Blit(
			glm::ivec2 srcOrigin,
			glm::ivec2 srcSize,
			glm::ivec2 dstOrigin,
			glm::ivec2 dstSize,
			FramebufferMasks bufMask = FramebufferMasks::Color,
			bool useNearestNeighbor = false
		);

		/// <summary>
		/// Clears data from the framebuffer currently bound based on the given bitmask
		/// </summary>
		static void ClearCurrent(FramebufferMasks bitMask);

		/// <summary>
		/// Specifies which color buffer is to be drawn to
		/// </summary>
		static void SetDrawBuffer(ColorBuffers buffer, uint colorAttachNum = 0);
		
		/// <summary>
		/// Specifies a color buffer to be used as a source of pixels
		/// </summary>
		static void SetReadBuffer(ColorBuffers buffer, uint colorAttachNum = 0);

		/// <summary>
		/// Returns the number of samples supported for multisampled framebuffers
		/// </summary>
		static GLint GetMaxSamples();

		/// <summary>
		/// Retrieves the FBO bound at the given target.
		/// </summary>
		static GLuint GetCurrent(FramebufferTargs buffTarg);

		/// <summary>
		/// Binds the FBO to the rendering context
		/// </summary>
		static void BindFramebuffer(GLuint id, FramebufferTargs target);

		/// <summary>
		/// Returns true if the FBO is bound for either read or draw
		/// </summary>
		static bool GetIsFramebufferBound(GLuint id, FramebufferTargs target);

		/// <summary>
		/// Unbinds the currently bound FBO
		/// </summary>
		static void UnbindFramebuffer(FramebufferTargs target);

		/// <summary>
		/// Returns the maximum number of color buffer attachments supported
		/// </summary>
		static GLint GetMaxColorAttachments();

		/// <summary>
		/// Returns the status of the framebuffer bound to the given target
		/// </summary>
		static FramebufferStates GetFramebufferStatus(FramebufferTargs target);

		/// <summary>
		/// Returns the given status enum as a string
		/// </summary>
		static const char* const GetFramebufferStatusString(FramebufferStates fbStatus);

	private:
		GLuint id;
		FramebufferTargs target;

		void Validate(bool requireCompleteness = false) const;

		static void ValidateFramebuffer(GLuint id, FramebufferTargs target, bool requireCompleteness = false);
	};
}
#endif
