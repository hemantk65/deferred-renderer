#version 450

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;
layout(location = 2) in vec3 a_normal;

uniform mat4 mvp;
uniform mat4 model;

out vec3 v_position;
out vec2 v_texCoord;
out vec3 v_normal;

void main()
{
	gl_Position = mvp * vec4(a_position, 1.0f);
	v_position = ((model * vec4(a_position, 1.0f)).xyz + 1.0f) * 0.5f;
	v_texCoord = a_texCoord;
	v_normal = (a_normal + 1.0f) * 0.5f;
}