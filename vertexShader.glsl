#version 440

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) uniform float alphaVar;

out vec2 texCoordV;
out float aColor;

void main()
{
	texCoordV = texCoord;
	aColor = alphaVar;
	gl_Position = position;
}