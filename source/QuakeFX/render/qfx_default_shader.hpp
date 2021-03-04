#ifndef QFX_DEFAULT_SHADER_H
#define QFX_DEFAULT_SHADER_H

const char *const defaultShaderSrc =
R"(
#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;
out vec2 v_TexCoord;

void main()
{
    gl_Position.xy = position;
	gl_Position.zw = vec2(0.0f, 1.0f);
    v_TexCoord = texCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec2 v_TexCoord;
uniform sampler2D u_Texture;

void main()
{
    color = texture(u_Texture, v_TexCoord);
}
)";

#endif // !QFX_DEFAULT_SHADER_H
