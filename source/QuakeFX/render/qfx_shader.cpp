#include "render/qfx_shader.hpp"
#include <string>

namespace QuakeFX
{
	using std::string;
	using std::stringstream;
	using std::ifstream;

	/// <summary>
	/// QfxShader default constructor
	/// </summary>
	QfxShader::QfxShader() :
		id(0),
		type(ShaderType::Invalid)
	{ }

	/// <summary>
	/// Initializes a new shader of the type specified from the given source
	/// code.
	/// </summary>
	QfxShader::QfxShader(const string& source, ShaderType type) :
		id(glCreateShader((int)(type))), // 0 if enum is invalid
		type(type)
	{
		if (id != 0)
		{
			// Set source code for shader
			const char* ptr = source.c_str();
			glShaderSource(id, 1, &ptr, NULL);

			// Compile shader from source
			glCompileShader(id);

			// Check compile status
			if (!GetCompileStatus())
			{
				string message = GetInfoLog();
				glDeleteShader(id);
				id = 0;

				throw std::exception(message.c_str());
			}
		}
		else
		{
			throw std::exception("Failed to create new shader.");
		}
	}

	/// <summary>
	/// Move constructor
	/// </summary>
	QfxShader::QfxShader(QfxShader&& other) noexcept :
		id(other.id),
		type(other.type)
	{
		other.id = 0;
		other.type = ShaderType::Invalid;
	}

	/// <summary>
	/// QfxShader destructor
	/// </summary>
	QfxShader::~QfxShader()
	{
		if (id != 0)
		{
			glDeleteShader(id);
		}
	}

	/// <summary>
	/// Move assignment operator
	/// </summary>
	QfxShader& QfxShader::operator=(QfxShader&& rhs) noexcept
	{
		id = rhs.id;
		type = rhs.type;
		rhs.id = 0;
		rhs.type = ShaderType::Invalid;

		return *this;
	}

	/// <summary>
	/// Retrieves the shader parameter associated with the given enum.
	/// </summary>
	int QfxShader::GetShaderParam(ShaderParameters paramEnum) const
	{
		int value;
		glGetShaderiv(id, (int)paramEnum, &value);

		return value;
	}

	/// <summary>
	/// Returns shader compilation log.
	/// </summary>
	string QfxShader::GetInfoLog() const
	{
		int length = GetInfoLogLength();
		char* message = new char[length];
		glGetShaderInfoLog(id, length, &length, message);

		return string(message, length);
	}

	/// <summary>
	/// Returns id used to reference the shader. Valid ID's are always non-zero.
	/// </summary>
	uint QfxShader::GetID() const { return id; }

	/// <summary>
	/// Returns the type enum for the shader
	/// </summary>
	ShaderType QfxShader::GetShaderType() const { return type; }

	/// <summary>
	/// Returns the number of characters in the shader's source code
	/// </summary>
	int QfxShader::GetSourceLength() const { return GetShaderParam(ShaderParameters::SourceLength); }

	/// <summary>
	/// Returns true if the last compile operation was successful
	/// </summary>
	bool QfxShader::GetCompileStatus() const { return GetShaderParam(ShaderParameters::CompileStatus); }

	/// <summary>
	/// Returns the number of characters in the shader's info log
	/// </summary>
	int QfxShader::GetInfoLogLength() const { return GetShaderParam(ShaderParameters::InfoLogLength); }

	/// <summary>
	/// Indicates whether or not the shader is marked for deletion
	/// </summary>
	bool QfxShader::GetDeleteStatus() const { return GetShaderParam(ShaderParameters::DeleteStatus); }
}
