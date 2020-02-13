#version 450

in vec3 v_position;
in vec2 v_texCoord;
in vec3 v_normal;

uniform sampler2D diffuse_tex0;
uniform sampler2D specular_tex0;
uniform sampler2D ambient_tex0;
uniform sampler2D height_tex0;
uniform vec4 reflectivity;

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 position;
layout(location = 3) out vec4 reflectance;

void main()
{
	color = texture(diffuse_tex0, v_texCoord);
	normal = texture(height_tex0, v_texCoord);
	position = vec4(v_position, 1.0f);
	reflectance = reflectivity;
}