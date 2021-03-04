#ifndef QFX_PROGRAM_H
#define QFX_PROGRAM_H

#include <glm/glm.hpp>
#include <unordered_map>
#include "opengl/qfx_opengl.hpp"
#include "qfx_dynamic_collections.hpp"
#include "render/qfx_shader.hpp"

namespace QuakeFX
{
	enum class ProgramParameters
	{
		/// <summary>
		/// Returns true if the program is currently flagged for deletion
		/// </summary>
		DeleteStatus = GL_DELETE_STATUS,

		/// <summary>
		/// Returns true if the last link operation was successful
		/// </summary>
		LinkStatus = GL_LINK_STATUS,

		/// <summary>
		/// Returns true if the last validation operation was successful
		/// </summary>
		ValidateStatus = GL_VALIDATE_STATUS,

		/// <summary>
		/// Returns the number of characters in the program information log
		/// </summary>
		InfoLogLength = GL_INFO_LOG_LENGTH,

		/// <summary>
		/// Returns the number of shader objects attached to the program
		/// </summary>
		AttachedShaders = GL_ATTACHED_SHADERS,

		/// <summary>
		/// Returns the number of active attribute atomic counter buffers for
		/// the program
		/// </summary>
		ActiveAtomicCounterBuffers = GL_ACTIVE_ATOMIC_COUNTER_BUFFERS,

		/// <summary>
		/// Returns the number of active attribute variables for the program
		/// </summary>
		ActiveAttributes = GL_ACTIVE_ATTRIBUTES,

		/// <summary>
		/// Returns the length of the longest active attribute name for the 
		/// program
		/// </summary>
		ActiveAttributeMaxLength = GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,

		/// <summary>
		/// Returns the number of active uniform variables for the program
		/// </summary>
		ActiveUniforms = GL_ACTIVE_UNIFORMS,

		/// <summary>
		/// Returns the length of the longest active uniform variable name for
		/// the program
		/// </summary>
		ActiveUniforMaxLength = GL_ACTIVE_UNIFORM_MAX_LENGTH,

		/// <summary>
		/// Returns the length of the program binary in bytes
		/// </summary>
		ProgramBinaryLength = GL_PROGRAM_BINARY_LENGTH,

		/// <summary>
		/// Returns an array of 3 integers representing the size of the 
		/// work group as specified by its input layout qualifiers
		/// </summary>
		ComputeWorkGroupSize = GL_COMPUTE_WORK_GROUP_SIZE,

		/// <summary>
		/// Returns a constant indicating the buffer mode used when transform
		/// feedback is active
		/// </summary>
		TransformFeedbackBufferMode = GL_TRANSFORM_FEEDBACK_BUFFER_MODE,

		/// <summary>
		/// Returns the number of varying variables to capture in the transform 
		/// feedback mode for the program
		/// </summary>
		TransformFeedbackVaryings = GL_TRANSFORM_FEEDBACK_VARYINGS,

		/// <summary>
		/// Returns the length of the longest variable name ot be used for
		/// transform feedback
		/// </summary>
		TransformFeedbackVaryingMaxLength = GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH,

		/// <summary>
		/// Returns the maximum number of vertices that the geometry shader in the
		/// program will output
		/// </summary>
		GeometryVerticesOut = GL_GEOMETRY_VERTICES_OUT,

		/// <summary>
		/// Returns a constant indicating the primitive type accepted as input to the
		/// geometry shader contained in the program
		/// </summary>
		GeometryInputType = GL_GEOMETRY_INPUT_TYPE,

		/// <summary>
		/// Returns a constant indicating the primitive type that will be output by the
		/// geometry shader in the program
		/// </summary>
		GeometryOutputType = GL_GEOMETRY_OUTPUT_TYPE
	};

	class QfxProgram
	{
	public:
		QfxProgram();

		QfxProgram(const QfxProgram& other) = delete;

		QfxProgram(QfxProgram&& other) noexcept;

		QfxProgram(const std::string& source);

		~QfxProgram();

		QfxProgram& operator=(const QfxProgram& rhs) = delete;

		QfxProgram& operator=(QfxProgram&& rhs) noexcept;

		/// <summary>
		/// Returns the program id
		/// </summary>
		uint GetID() const;

		/// <summary>
		/// Binds the shader program to the rendering context for use in the next draw call(s)
		/// </summary>
		void Bind();

		/// <summary>
		/// Unbinds the shader program from the rendering context
		/// </summary>
		void Unbind();

		/// <summary>
		/// Unbinds the current shader program from the rendering context
		/// </summary>
		static void UnbindProgram();

		/// <summary>
		/// Sets an integer uniform accessible by shader programs
		/// </summary>
		void SetUniform(const std::string& name, int value);

		/// <summary>
		/// Sets a 4D float vector as a uniform
		/// </summary>
		void SetUniform(const std::string& name, glm::vec4 value);

		/// <summary>
		/// Sets a 4x4 float matrix as a uniform
		/// </summary>
		void SetUniform(const std::string& name, glm::mat4 value);

		/// <summary>
		/// Retrieves the location of the uniform with the given name
		/// </summary>
		int GetUniformLocation(const std::string& name);

		/// <summary>
		/// Retrieves the program parameter associated with the given enum
		/// </summary>
		int GetProgramParam(ProgramParameters paramEnum) const;

		/// <summary>
		/// Returns linking and validation information
		/// </summary>
		std::string GetInfoLog() const;

		/// <summary>
		/// Returns the length of the info log
		/// </summary>
		/// <returns></returns>
		int GetInfoLogLength() const;

		/// <summary>
		/// Returns true if the program is flagged for deletion
		/// </summary>
		bool GetDeleteStatus() const;

		/// <summary>
		/// Returns true if last link operation was successful
		/// </summary>
		bool GetLinkStatus() const;

		/// <summary>
		/// Returns true if last validation operation was successful
		/// </summary>
		bool GetValidateStatus() const;

		/// <summary>
		/// Returns the number of attached shaders
		/// </summary>
		int GetShaderCount() const;

		/// <summary>
		/// Returns the length of the compiled binary in bytes
		/// </summary>
		int GetBinaryLength() const;

		/// <summary>
		/// Binds the program associated with the given name.
		/// </summary>
		static void BindProgram(GLuint id);

		/// <summary>
		/// Returns the name of the shader program currently bound.
		/// </summary>
		static GLuint GetCurrentProgram();

	private:
		GLuint id;
		std::unordered_map<std::string, int> uniformCache;

		void Validate();
	};
}

#endif
