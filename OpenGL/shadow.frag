#version 450

in vec2 texCoord;

uniform sampler2D colorTex;
uniform sampler2D depthTex;
uniform sampler2D shadowMap;

layout(location = 0) out vec4 color;

void main()
{
	color = texture(colorTex, texCoord);
	float depth = texture(depthTex, texCoord).r;
	float lightDepth = texture(shadowMap, texCoord).r;
}