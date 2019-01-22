#version 450

layout(location = 0) in vec2 a_position;

out vec2 texCoord;

void main()
{
	gl_Position = vec4(a_position, 0.0f, 1.0f);
	texCoord = (a_position + vec2(1.0f, 1.0f)) / 2.0f;
}