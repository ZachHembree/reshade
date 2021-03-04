#include "render/qfx_framebuffer_obj.hpp"

namespace QuakeFX
{
	QfxFramebufferObj::QfxFramebufferObj() :
		id(0),
		target(FramebufferTargs::Invalid)
	{
		glGenFramebuffers(1, &id);
	}

	QfxFramebufferObj::~QfxFramebufferObj()
	{
		if (id != 0)
		{
			glDeleteFramebuffers(1, &id);
		}
	}

	/// <summary>
	/// Returns the internal name of the FBO
	/// </summary>
	GLuint QfxFramebufferObj::GetID() const { return id; }

	/// <summary>
	/// Returns true if the FBO is bound for either read or draw
	/// </summary>
	bool QfxFramebufferObj::GetIsBound() const
	{
		return GetIsFramebufferBound(id, target);
	}

	/// <summary>
	/// Returns true if the framebuffer is complete.
	/// </summary>
	bool QfxFramebufferObj::GetIsComplete() const
	{
		return GetStatus() == FramebufferStates::Complete;
	}

	/// <summary>
	/// Binds the FBO to the rendering context
	/// </summary>
	void QfxFramebufferObj::Bind(FramebufferTargs newTarget)
	{
		target = newTarget;
		glBindFramebuffer((GLenum)newTarget, id);
	}

	/// <summary>
	/// Unbinds the FBO from the rendering context
	/// </summary>
	void QfxFramebufferObj::Unbind()
	{
		Validate();
		glBindFramebuffer((GLenum)target, 0);
	}

	void QfxFramebufferObj::Clear(FramebufferMasks bitMask)
	{
		Validate();
		ClearCurrent(bitMask);
	}

	void QfxFramebufferObj::ClearCurrent(FramebufferMasks bitMask)
	{
		glClear((GLbitfield)bitMask);
	}

	/// <summary>
	/// Specifies which color buffer is to be drawn to
	/// </summary>
	void QfxFramebufferObj::SetDrawBuffer(ColorBuffers buffer, uint colorAttachNum)
	{
		auto glBuffer = (GLenum)buffer;

		if (buffer == ColorBuffers::ColorAttach)
			glBuffer += colorAttachNum;

		glDrawBuffer(glBuffer);
	}

	/// <summary>
	/// Specifies a color buffer to be used as a source of pixels
	/// </summary>
	void QfxFramebufferObj::SetReadBuffer(ColorBuffers buffer, uint colorAttachNum)
	{
		auto glBuffer = (GLenum)buffer;

		if (buffer == ColorBuffers::ColorAttach)
			glBuffer += colorAttachNum;

		glReadBuffer(glBuffer);
	}

	/// <summary>
	/// Attaches a texture object as a logical buffer to the FBO
	/// </summary>
	void QfxFramebufferObj::SetFrameBufferRenderBuffer(QfxRenderBuffer& rBuffer, FramebufferAttachments attach, FramebufferTargs buffTarg, uint colorAttachNum)
	{
		Validate();
		auto glAttach = (GLenum)attach;

		if (attach == FramebufferAttachments::Color)
			glAttach += colorAttachNum;

		glFramebufferRenderbuffer((GLenum)buffTarg, glAttach, GL_RENDERBUFFER, rBuffer.GetID());
	}

	/// <summary>
	/// Attaches a texture object as a logical buffer to the FBO
	/// </summary>
	void QfxFramebufferObj::SetFramebufferTexture(QfxTextureBase& texture, FramebufferTargs buffTarg, FramebufferAttachments attach, uint colorAttachNum, GLint mipLevel)
	{
		Validate();
		auto glAttach = (GLenum)attach;

		if (attach == FramebufferAttachments::Color)
			glAttach += colorAttachNum;

		glFramebufferTexture((GLenum)buffTarg, glAttach, texture.GetID(), mipLevel);
	}

	/// <summary>
	/// Returns the status of the framebuffer when treated as if bound to its target.
	/// </summary>
	FramebufferStates QfxFramebufferObj::GetStatus() const
	{
		Validate();
		return GetFramebufferStatus(target);
	}

	/// <summary>
	/// Returns framebuffer status as a string
	/// </summary>
	const char* const QfxFramebufferObj::GetStatusString() const
	{
		return GetFramebufferStatusString(GetStatus());
	}

	/// <summary>
	/// Returns the number of samples supported for multisampled framebuffers
	/// </summary>
	GLint QfxFramebufferObj::GetMaxSamples()
	{
		GLint maxSamples;
		glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);

		return maxSamples;
	}

	/// <summary>
	/// Returns true if the FBO is bound for either read or draw
	/// </summary>
	bool QfxFramebufferObj::GetIsFramebufferBound(GLuint id, FramebufferTargs target)
	{
		GLint drawFboId = 0, readFboId = 0;
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);
		glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFboId);

		if (drawFboId != 0 || readFboId != 0)
		{ 
			if (GL_FRAMEBUFFER_BINDING == (GLenum)target && ((GLuint)drawFboId != id || (GLuint)readFboId != id))
				return false;
			else if (GL_DRAW_FRAMEBUFFER_BINDING == (GLenum)target && (GLuint)drawFboId != id)
				return false;
			else if (GL_READ_FRAMEBUFFER_BINDING == (GLenum)target && (GLuint)readFboId != id)
				return false;
			else
				return true;
		}
		else
			return false;
	}

	GLuint QfxFramebufferObj::GetCurrent(FramebufferTargs target)
	{
		GLuint id = 0;
		GLint drawFboId = 0, readFboId = 0;
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);
		glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFboId);

		if ((drawFboId != 0 && readFboId != 0) && drawFboId != readFboId && target == FramebufferTargs::Framebuffer)
			REPORTERROR("Framebuffer target ambiguous. More than one FBO is bound to the specified targets.")

		if ((GL_DRAW_FRAMEBUFFER_BINDING & (GLenum)target) > 0 && (GLuint)drawFboId != id)
			id = (GLuint)drawFboId;
		else if ((GL_READ_FRAMEBUFFER_BINDING & (GLenum)target) > 0 && (GLuint)readFboId != id)
			id = (GLuint)readFboId;

		return id;
	}

	/// <summary>
	/// Copies the contents of the FBO, using the specified color buffer, into the
	/// given destination framebuffer.
	/// </summary>
	void QfxFramebufferObj::BlitTo(
		ColorBuffers srcColor,
		GLuint dstName,
		ColorBuffers dstColor,
		glm::ivec2 size,
		FramebufferMasks bufMask,
		bool autoUnbind,
		bool useNearestNeighbor
	)
	{
		Validate(true);
		Blit(id, srcColor, dstName, dstColor, size, bufMask, useNearestNeighbor);
		UnbindFramebuffer(FramebufferTargs::Framebuffer);

		if (!autoUnbind)
			Bind(target);
	}

	/// <summary>
	/// Copies the contents of the given framebuffer into the FBO, using the 
	/// specified color buffer.
	/// </summary>
	void QfxFramebufferObj::BlitFrom(
		ColorBuffers dstColor,
		GLuint srcName,
		ColorBuffers srcColor,
		glm::ivec2 size,
		FramebufferMasks bufMask,
		bool autoUnbind,
		bool useNearestNeighbor
	)
	{
		Validate(true);
		Blit(srcName, srcColor, id, dstColor, size, bufMask, useNearestNeighbor);
		UnbindFramebuffer(FramebufferTargs::Framebuffer);

		if (!autoUnbind)
			Bind(target);
	}

	/// <summary>
	/// Copies the contents of the given read buffer to the given draw buffer.
	/// </summary>
	void QfxFramebufferObj::Blit(
		GLuint srcName,
		ColorBuffers srcColor,
		GLuint dstName,
		ColorBuffers dstColor,
		glm::ivec2 size,
		FramebufferMasks bufMask,
		bool useNearestNeighbor
	)
	{
		Blit(
			srcName,
			srcColor,
			glm::ivec2(0, 0),
			size,
			dstName,
			dstColor,
			glm::ivec2(0, 0),
			size,
			bufMask,
			useNearestNeighbor
		);
	}

	/// <summary>
	/// Copies the contents of the given read buffer to the given draw buffer.
	/// </summary>
	void QfxFramebufferObj::Blit(
		GLuint srcName,
		ColorBuffers srcColor,
		glm::ivec2 srcSize,
		GLuint dstName,
		ColorBuffers dstColor,
		glm::ivec2 dstSize,
		FramebufferMasks bufMask,
		bool useNearestNeighbor
	)
	{
		Blit(
			srcName,
			srcColor,
			glm::ivec2(0, 0),
			srcSize,
			dstName,
			dstColor,
			glm::ivec2(0, 0),
			dstSize,
			bufMask,
			useNearestNeighbor
		);
	}

	/// <summary>
	/// Copies the contents of the given read buffer to the given draw buffer.
	/// </summary>
	void QfxFramebufferObj::Blit(
		GLuint srcName,
		ColorBuffers srcColor,
		glm::ivec2 srcOrigin,
		glm::ivec2 srcSize,
		GLuint dstName,
		ColorBuffers dstColor,
		glm::ivec2 dstOrigin,
		glm::ivec2 dstSize,
		FramebufferMasks bufMask,
		bool useNearestNeighbor
	)
	{
		BindFramebuffer(srcName, FramebufferTargs::Read);
		BindFramebuffer(dstName, FramebufferTargs::Draw);

		glReadBuffer((GLenum)srcColor);
		glDrawBuffer((GLenum)dstColor);

		Blit(srcOrigin, srcSize, dstOrigin, dstSize, bufMask, useNearestNeighbor);
	}

	/// <summary>
	/// Copies the contents of the read buffer currently bound into the bound draw buffer.
	/// </summary>
	void QfxFramebufferObj::Blit(
		glm::ivec2 size,
		FramebufferMasks bufMask,
		bool useNearestNeighbor
	)
	{
		Blit(glm::ivec2(0, 0), size, glm::ivec2(0, 0), size, bufMask, useNearestNeighbor);
	}

	/// <summary>
	/// Copies the contents of the read buffer currently bound into the bound draw buffer.
	/// </summary>
	void QfxFramebufferObj::Blit(
		glm::ivec2 srcSize,
		glm::ivec2 dstSize,
		FramebufferMasks bufMask,
		bool useNearestNeighbor
	)
	{
		Blit(glm::ivec2(0, 0), srcSize, glm::ivec2(0, 0), dstSize, bufMask, useNearestNeighbor);
	}

	/// <summary>
	/// Copies the contents of the read buffer currently bound into the bound draw buffer.
	/// </summary>
	void QfxFramebufferObj::Blit(
		glm::ivec2 srcOrigin,
		glm::ivec2 srcSize,
		glm::ivec2 dstOrigin,
		glm::ivec2 dstSize,
		FramebufferMasks bufMask,
		bool useNearestNeighbor
	)
	{
		auto glDepthStencil = (GLenum)bufMask & (GLenum)FramebufferMasks::DepthStencil;

		if (!useNearestNeighbor)
		{
			if (glDepthStencil > 0)
			{
				glBlitFramebuffer(
					srcOrigin.x, srcOrigin.y,
					srcSize.x, srcSize.y,
					dstOrigin.x, dstOrigin.y,
					dstSize.x, dstSize.y,
					glDepthStencil,
					GL_NEAREST
				);
			}

			if (( (GLenum)bufMask & (GLenum)FramebufferMasks::Color ) > 0)
			{
				glBlitFramebuffer(
					srcOrigin.x, srcOrigin.y,
					srcSize.x, srcSize.y,
					dstOrigin.x, dstOrigin.y,
					dstSize.x, dstSize.y,
					(GLenum)FramebufferMasks::Color,
					GL_LINEAR
				);
			}
		}
		else
		{
			glBlitFramebuffer(
				srcOrigin.x, srcOrigin.y,
				srcSize.x, srcSize.y,
				dstOrigin.x, dstOrigin.y,
				dstSize.x, dstSize.y,
				(GLenum)bufMask,
				GL_NEAREST
			);
		}
	}

	/// <summary>
	/// Binds the FBO to the rendering context
	/// </summary>
	void QfxFramebufferObj::BindFramebuffer(GLuint id, FramebufferTargs target)
	{
		glBindFramebuffer((GLenum)target, id);
	}

	/// <summary>
	/// Unbinds the currently bound FBO
	/// </summary>
	void QfxFramebufferObj::UnbindFramebuffer(FramebufferTargs target)
	{
		glBindFramebuffer((GLenum)target, 0);
	}

	/// <summary>
	/// Returns the maximum number of color buffer attachments supported
	/// </summary>
	GLint QfxFramebufferObj::GetMaxColorAttachments()
	{
		GLint maxAttach = 0;
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxAttach);

		return maxAttach;
	}

	/// <summary>
	/// Returns the status of the framebuffer bound to the given target
	/// </summary>
	FramebufferStates QfxFramebufferObj::GetFramebufferStatus(FramebufferTargs target)
	{
		return (FramebufferStates)glCheckFramebufferStatus((GLenum)target);
	}

	/// <summary>
	/// Returns the given status enum as a string
	/// </summary>
	const char* const QfxFramebufferObj::GetFramebufferStatusString(FramebufferStates fbStatus)
	{
		switch (fbStatus)
		{
		case FramebufferStates::Complete:
			return "Framebuffer Complete";
		case FramebufferStates::Undefined:
			return "Framebuffer Undefined";
		case FramebufferStates::IncompleteAttach:
			return "Incomplete: Attachment";
		case FramebufferStates::MissingAttach:
			return "Incomplete: Missing Attachment";
		case FramebufferStates::IncompleteDrawBuffer:
			return "Incomplete: Draw Buffer";
		case FramebufferStates::IncompleteReadBuffer:
			return "Incomplete: Read Buffer";
		case FramebufferStates::Unsupported:
			return "Framebuffer Unsupported";
		case FramebufferStates::IncompleteMultisample:
			return "Incomplete: Multisample";
		case FramebufferStates::IncompleteLayerTargets:
			return "Incomplete: Layer Targets";
		default:
			return "Unknown";
		}
	}

	void QfxFramebufferObj::Validate(bool requireCompleteness) const
	{
#ifndef NDEBUG
		ValidateFramebuffer(id, target, requireCompleteness);
#endif // !NDEBUG
	}

	void QfxFramebufferObj::ValidateFramebuffer(GLuint id, FramebufferTargs target, bool requireCompleteness)
	{
#ifndef NDEBUG
		if (!GetIsFramebufferBound(id, target))
			REPORTERROR("Cannot use framebuffer object without binding it.")
		else if (requireCompleteness)
		{
			FramebufferStates status = GetFramebufferStatus(target);

			if (status != FramebufferStates::Complete)
			{
				std::string error("Framebuffer must be complete in order to use this function. Error:");
				error += GetFramebufferStatusString(status);

				REPORTERROR(error.c_str());
			}
		}
#endif // !NDEBUG
	}
}
