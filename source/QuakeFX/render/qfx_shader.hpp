#ifndef QFX_SHADER_H
#define QFX_SHADER_H
#include <string>
#include "opengl/qfx_opengl.hpp"

namespace QuakeFX
{
	/// <summary>
	/// Container for parsed shader source code
	/// </summary>
	struct ProgramData
	{
		const std::string vertexSrc;
		const std::string fragmentSrc;
	};

	enum class ShaderType
	{
		Invalid = -1,
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER
	};

	/// <summary>
	/// Enumerables used to access shader properties
	/// </summary>
	enum class ShaderParameters
	{
		/// <summary>
		/// Returns the type enum for the shader
		/// </summary>
		ShaderType = GL_SHADER_TYPE,

		/// <summary>
		/// Indicates whether or not the shader is marked for deletion
		/// </summary>
		DeleteStatus = GL_DELETE_STATUS,

		/// <summary>
		/// Returns true if the last compile operation was successful
		/// </summary>
		CompileStatus = GL_COMPILE_STATUS,

		/// <summary>
		/// Returns the number of characters in the shader's info log
		/// </summary>
		InfoLogLength = GL_INFO_LOG_LENGTH,

		/// <summary>
		/// Returns the number of characters in the shader's source code
		/// </summary>
		SourceLength = GL_SHADER_SOURCE_LENGTH
	};

	/// <summary>
	/// Wrapper for OpenGL shader source code information. Move only.
	/// </summary>
	class QfxShader
	{
	private:
		uint id;
		ShaderType type;

	public:
		/// <summary>
		/// QfxShader default constructor
		/// </summary>
		QfxShader();

		/// <summary>
		/// Initializes a new shader of the type specified from the given source
		/// code.
		/// </summary>
		QfxShader(const std::string& source, ShaderType type);

		/// <summary>
		/// Copy constructor. Deleted.
		/// </summary>
		QfxShader(const QfxShader& other) = delete;

		/// <summary>
		/// Move constructor
		/// </summary>
		QfxShader(QfxShader&& other) noexcept;

		/// <summary>
		/// QfxShader destructor
		/// </summary>
		~QfxShader();

		/// <summary>
		/// Copy assignment operator. Deleted.
		/// </summary>
		QfxShader& operator=(const QfxShader& rhs) = delete;

		/// <summary>
		/// Move assignment operator
		/// </summary>
		QfxShader& operator=(QfxShader&& rhs) noexcept;

		/// <summary>
		/// Retrieves the shader parameter associated with the given enum.
		/// </summary>
		int GetShaderParam(ShaderParameters paramEnum) const;

		/// <summary>
		/// Returns shader compilation log.
		/// </summary>
		std::string GetInfoLog() const;

		/// <summary>
		/// Returns id used to reference the shader. Valid ID's are always non-zero.
		/// </summary>
		uint GetID() const;

		/// <summary>
		/// Returns the type enum for the shader
		/// </summary>
		ShaderType GetShaderType() const;

		/// <summary>
		/// Returns the number of characters in the shader's source code
		/// </summary>
		int GetSourceLength() const;

		/// <summary>
		/// Returns true if the last compile operation was successful
		/// </summary>
		bool GetCompileStatus() const;

		/// <summary>
		/// Returns the number of characters in the shader's info log
		/// </summary>
		int GetInfoLogLength() const;

		/// <summary>
		/// Indicates whether or not the shader is marked for deletion
		/// </summary>
		bool GetDeleteStatus() const;
	};
}

#endif // !QFX_SHADER_H
