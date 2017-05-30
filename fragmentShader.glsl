#version 440

layout(location = 0) uniform sampler2D texture1;
layout(location = 1) uniform sampler2D texture2;

in vec2 texCoordV;
in float aColor;

out vec4 color;

void main(void)
{
	color = mix(texture(texture1, texCoordV) * (1.0, 1.0, 1.0, aColor), texture(texture2, texCoordV), 0.8);
	//color = texelFetch(texture1, ivec2(gl_FragCoord.xy), 0);
}