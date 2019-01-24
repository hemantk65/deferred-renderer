#version 450

in vec3 v_position;
in vec2 v_texCoord;
in vec3 v_normal;

uniform sampler2D m_tex;

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 position;

void main()
{
	color = texture(m_tex, v_texCoord);
	normal = vec4(v_normal, 1.0f);
	position = vec4(v_position, 1.0f);
}