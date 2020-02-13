#version 450

in vec2 texCoord;

uniform sampler2D colorTex;
uniform sampler2D depthTex;
uniform sampler2D blurTex;

layout(location = 0) out vec4 color;

void main()
{
	float focus = 0.9f;
	float depthDiff = abs(texture(depthTex, texCoord).r - focus);
	color = mix(texture(colorTex, texCoord), texture(blurTex, texCoord), depthDiff);
}