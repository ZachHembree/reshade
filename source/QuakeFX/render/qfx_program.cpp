#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "render/qfx_program.hpp"

namespace QuakeFX
{
	using std::string;
	using std::stringstream;
	using std::ifstream;

	ProgramData GetParsedShaderSrc(const string& source);

	QfxProgram::QfxProgram() :
		id(0)
	{ }

	QfxProgram::QfxProgram(QfxProgram&& other) noexcept :
		id(other.id),
		uniformCache(std::move(other.uniformCache))
	{
		other.id = 0;
	}

	QfxProgram::QfxProgram(const string& source) :
		id(glCreateProgram())
	{
		if (id != 0)
		{
			ProgramData parsedSrc = GetParsedShaderSrc(source);
			QfxShader vertex(parsedSrc.vertexSrc, ShaderType::Vertex);
			QfxShader fragment(parsedSrc.fragmentSrc, ShaderType::Fragment);

			glAttachShader(id, vertex.GetID());
			glAttachShader(id, fragment.GetID());
			glLinkProgram(id);
			glValidateProgram(id);

			if (!(GetLinkStatus() && GetValidateStatus()))
			{
				string log = GetInfoLog();
				glDeleteProgram(id);
				id = 0;

				throw log;
			}
		}
		else
		{
			throw string("Failed to create shader program.");
		}
	}

	QfxProgram::~QfxProgram()
	{
		if (id != 0)
		{
			glDeleteProgram(id);
		}
	}

	/// <summary>
	/// Binds the shader program to the rendering context for use in the next draw call(s)
	/// </summary>
	void QfxProgram::Bind() { glUseProgram(id); }

	/// <summary>
	/// Unbinds the shader program from the rendering context
	/// </summary>
	void QfxProgram::Unbind() { Validate(); glUseProgram(0); }

	/// <summary>
	/// Unbinds the current shader program from the rendering context
	/// </summary>
	void QfxProgram::UnbindProgram() { glUseProgram(0); }

	/// <summary>
	/// Sets an integer uniform accessible by shader programs
	/// </summary>
	void QfxProgram::SetUniform(const std::string& name, int value)
	{
		Validate();
		glUniform1i(GetUniformLocation(name), value);
	}

	/// <summary>
	/// Sets a 4D float vector as a uniform
	/// </summary>
	void QfxProgram::SetUniform(const std::string& name, glm::vec4 value)
	{
		Validate();
		glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	/// <summary>
	/// Sets a 4x4 float matrix as a uniform
	/// </summary>
	void QfxProgram::SetUniform(const std::string& name, glm::mat4 value)
	{
		Validate();
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
	}

	/// <summary>
	/// Retrieves the location of the uniform with the given name
	/// </summary>
	int QfxProgram::GetUniformLocation(const std::string& name)
	{
		if (uniformCache.find(name) != uniformCache.end())
			return uniformCache[name];

		int location;
		location = glGetUniformLocation(id, name.c_str());
		uniformCache[name] = location;

		return location;
	}

	QfxProgram& QfxProgram::operator=(QfxProgram&& rhs) noexcept
	{
		id = rhs.id;
		uniformCache = std::move(rhs.uniformCache);

		rhs.id = 0;

		return *this;
	}

	/// <summary>
	/// Returns the program id
	/// </summary>
	uint QfxProgram::GetID() const
	{
		return id;
	}

	/// <summary>
	/// Retrieves the program parameter associated with the given enum
	/// </summary>
	int QfxProgram::GetProgramParam(ProgramParameters paramEnum) const
	{
		int value;
		glGetProgramiv(id, (int)paramEnum, &value);

		return value;
	}

	/// <summary>
	/// Returns linking and validation information
	/// </summary>
	string QfxProgram::GetInfoLog() const
	{
		int length = GetInfoLogLength();
		char* message = new char[length];
		glGetProgramInfoLog(id, length, &length, message);

		return string(message, length);
	}

	/// <summary>
	/// Returns the length of the info log
	/// </summary>
	/// <returns></returns>
	int QfxProgram::GetInfoLogLength() const { return GetProgramParam(ProgramParameters::InfoLogLength); }

	/// <summary>
	/// Returns true if the program is flagged for deletion
	/// </summary>
	bool QfxProgram::GetDeleteStatus() const { return GetProgramParam(ProgramParameters::DeleteStatus); }

	/// <summary>
	/// Returns true if last link operation was successful
	/// </summary>
	bool QfxProgram::GetLinkStatus() const { return GetProgramParam(ProgramParameters::LinkStatus); }

	/// <summary>
	/// Returns true if last validation operation was successful
	/// </summary>
	bool QfxProgram::GetValidateStatus() const { return GetProgramParam(ProgramParameters::ValidateStatus); }

	/// <summary>
	/// Returns the number of attached shaders
	/// </summary>
	int QfxProgram::GetShaderCount() const { return GetProgramParam(ProgramParameters::AttachedShaders); }

	/// <summary>
	/// Returns the length of the compiled binary in bytes
	/// </summary>
	int QfxProgram::GetBinaryLength() const { return GetProgramParam(ProgramParameters::InfoLogLength); }

	void QfxProgram::BindProgram(GLuint id) { glUseProgram(id); }

	GLuint QfxProgram::GetCurrentProgram()
	{
		GLint id;
		glGetIntegerv(GL_CURRENT_PROGRAM, &id);

		return (GLuint)id;
	}

	void QfxProgram::Validate()
	{
#ifndef NDEBUG
		if (id != GetCurrentProgram())
			REPORTERROR("Invalid operation. Shader programs must be bound before they can be used.")
#endif
	}

	/// <summary>
	/// Parses combined GLSL source into separate vertex and fragment source strings
	/// for compilation. Does not support other shader types.
	/// </summary>
	ProgramData GetParsedShaderSrc(const string& source)
	{
		stringstream srcStream(source);
		string line;
		stringstream sStream[2];
		ShaderType type = ShaderType::Invalid;

		while (getline(srcStream, line))
		{
			if (line.find("#shader") != string::npos)
			{
				if (line.find("vertex") != string::npos)
					type = ShaderType::Vertex;
				else if (line.find("fragment") != string::npos)
					type = ShaderType::Fragment;
				else
					type = ShaderType::Invalid;
			}
			else
			{
				switch (type)
				{
				case ShaderType::Vertex:
					sStream[0] << line << '\n';
					break;
				case ShaderType::Fragment:
					sStream[1] << line << '\n';
					break;
				}
			}
		}

		return ProgramData
		{
			sStream[0].str(),
			sStream[1].str(),
		};
	}
}
