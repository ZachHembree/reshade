#include "render/qfx_capture_buffer.hpp"

namespace QuakeFX
{
	CaptureBuffer::CaptureBuffer() :
		capturing(false),
		captured(false)
	{
		lastDrawFbo = QfxFramebufferObj::GetCurrent(FramebufferTargs::Draw);
		lastReadFbo = QfxFramebufferObj::GetCurrent(FramebufferTargs::Read);

		GLuint	lastVao = QfxVertexArrayObj::GetCurrentVAO(),
			lastVBuff = QfxBuffer::GetCurrentBuffer(BufferBindingTargets::ArrayBuffer),
			lastIBuff = QfxBuffer::GetCurrentBuffer(BufferBindingTargets::ElementArray),
			lastProgram = QfxProgram::GetCurrentProgram(),
			lastTexture = QfxTextureBase::GetCurrentTexture(TexBindings::TwoD);
		GLenum lastTexUnit = QfxTextureBase::GetActiveTextureUnit();

		// Configure FBO to draw to texture
		fbo.Bind(FramebufferTargs::Framebuffer);
		fbo.SetDrawBuffer(ColorBuffers::ColorAttach);
		texture.Bind(lastTexUnit);
		fbo.SetFramebufferTexture(texture, FramebufferTargs::Framebuffer, FramebufferAttachments::Color);

		// Set up quad to render texture
		vertices =
		{
			vec2(-1.0f, -1.0f), vec2(0.0f, 0.0f),	// 0
			vec2(-1.0f, 1.0f),	vec2(0.0f, 1.0f),	// 1
			vec2(1.0f, 1.0f),	vec2(1.0f, 1.0f),	// 2
			vec2(1.0f, -1.0f),	vec2(1.0f, 0.0f)	// 3
		};
		triangles =
		{
			ivec3(0, 1, 2),
			ivec3(0, 2, 3)
		};
		QfxVertexLayout vertLayout =
		{
			{ BufferDataTypes::Float, 2 },
			{ BufferDataTypes::Float, 2 }
		};

		vao.Bind();
		vertexBuffer = QfxBuffer(BufferBindingTargets::ArrayBuffer, BufferUsagePatterns::StaticDraw);
		vertexBuffer.Bind();
		vertexBuffer.SetData(vertices);

		indexBuffer = QfxBuffer(BufferBindingTargets::ElementArray, BufferUsagePatterns::StaticDraw);
		indexBuffer.Bind();
		indexBuffer.SetData(triangles);

		vao.SetLayout(vertLayout);

		// Create program to render the quad
		program = QfxProgram(std::string(defaultShaderSrc));

		QfxVertexArrayObj::BindVAO(lastVao);
		QfxBuffer::BindBuffer(BufferBindingTargets::ArrayBuffer, lastVBuff);
		QfxBuffer::BindBuffer(BufferBindingTargets::ElementArray, lastIBuff);
		QfxProgram::BindProgram(lastProgram);
		QfxTextureBase::BindTexture(TexTargs::TwoD, lastTexture, lastTexUnit);
		QfxFramebufferObj::BindFramebuffer(lastReadFbo, FramebufferTargs::Read);
		QfxFramebufferObj::BindFramebuffer(lastDrawFbo, FramebufferTargs::Draw);
	}

	/// <summary>
	/// Returns true if the buffer has started capturing color data.
	/// </summary>
	bool CaptureBuffer::GetIsCapturing() { return capturing; }

	/// <summary>
	/// Returns true if the buffer has been stopped after successfully capturing color
	/// data.
	/// </summary>
	bool CaptureBuffer::GetIsCaptured() { return captured; }

	/// <summary>
	/// Starts capturing framebuffer color data using the current viewport size.
	/// </summary>
	void CaptureBuffer::StartCapture()
	{
		glm::ivec4 vp(0,0,0,0);
		glGetIntegerv(GL_VIEWPORT, &vp[0]);

		StartCapture(vp);
	}

	/// <summary>
	/// Starts capturing framebuffer color data using the given viewport size.
	/// </summary>
	void CaptureBuffer::StartCapture(glm::ivec4 vp)
	{
		if (!capturing && !captured)
		{
			viewport = vp;

			GLuint lastTexture = QfxTextureBase::GetCurrentTexture(TexBindings::TwoD);
			GLenum lastTexUnit = QfxTextureBase::GetActiveTextureUnit();
			lastReadFbo = QfxFramebufferObj::GetCurrent(FramebufferTargs::Read);
			lastDrawFbo = QfxFramebufferObj::GetCurrent(FramebufferTargs::Draw);

			// Capture UI to texture
			texture.Bind(lastTexUnit);
			texture.SetSize(ivec2(vp[2], vp[3]));
			fbo.Bind(FramebufferTargs::Framebuffer);
			QfxFramebufferObj::SetDrawBuffer(ColorBuffers::ColorAttach);
			fbo.SetFramebufferTexture(texture, FramebufferTargs::Framebuffer, FramebufferAttachments::Color);

			glViewport(vp[0], vp[1], vp[2], vp[3]);
			fbo.Clear(FramebufferMasks::Color);

			QfxTextureBase::BindTexture(TexTargs::TwoD, lastTexture, lastTexUnit);

			captured = false;
			capturing = true;
		}
	}

	/// <summary>
	/// Resumes capturing after being stopped without clearing the buffer or resizing.
	/// </summary>
	void CaptureBuffer::ResumeCapture()
	{
		if (captured)
		{
			lastReadFbo = QfxFramebufferObj::GetCurrent(FramebufferTargs::Read);
			lastDrawFbo = QfxFramebufferObj::GetCurrent(FramebufferTargs::Draw);

			fbo.Bind(FramebufferTargs::Framebuffer);
			QfxFramebufferObj::SetDrawBuffer(ColorBuffers::ColorAttach);
			fbo.SetFramebufferTexture(texture, FramebufferTargs::Framebuffer, FramebufferAttachments::Color);

			glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
		}
	}

	/// <summary>
	/// Stops color data capture.
	/// </summary>
	void CaptureBuffer::StopCapture()
	{
		if (capturing)
		{
			QfxFramebufferObj::BindFramebuffer(lastReadFbo, FramebufferTargs::Read);
			QfxFramebufferObj::BindFramebuffer(lastDrawFbo, FramebufferTargs::Draw);

			captured = true;
			capturing = false;
		}
	}

	/// <summary>
	/// Draws color data to textured quad.
	/// </summary>
	void CaptureBuffer::Draw()
	{
		StopCapture();

		if (captured)
		{
			GLuint lastVao = QfxVertexArrayObj::GetCurrentVAO(),
				lastVBuff = QfxBuffer::GetCurrentBuffer(BufferBindingTargets::ArrayBuffer),
				lastIBuff = QfxBuffer::GetCurrentBuffer(BufferBindingTargets::ElementArray),
				lastProgram = QfxProgram::GetCurrentProgram(),
				lastTexture = QfxTextureBase::GetCurrentTexture(TexBindings::TwoD);
			GLenum lastTexUnit = QfxTextureBase::GetActiveTextureUnit();

			texture.Bind(lastTexUnit);
			program.Bind();
			program.SetUniform("u_Texture", lastTexUnit);
			vao.Bind();

			glDrawElements(GL_TRIANGLES, triangles.GetLength() * 3, GL_UNSIGNED_INT, nullptr);

			QfxVertexArrayObj::BindVAO(lastVao);
			QfxBuffer::BindBuffer(BufferBindingTargets::ArrayBuffer, lastVBuff);
			QfxBuffer::BindBuffer(BufferBindingTargets::ElementArray, lastIBuff);
			QfxProgram::BindProgram(lastProgram);
			QfxTextureBase::BindTexture(TexTargs::TwoD, lastTexture, lastTexUnit);

			captured = false;
		}
	}
}
