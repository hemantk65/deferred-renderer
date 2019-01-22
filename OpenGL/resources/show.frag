#version 450

in vec2 texCoord;

uniform sampler2D m_tex;

layout(location = 0) out vec4 color;

void main()
{
	color = texture(m_tex, texCoord);
}