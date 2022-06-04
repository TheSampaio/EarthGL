#version 330 core

in vec3 Color;
in vec2 UV;

out vec4 OutColor;

uniform sampler2D TextureSampler;

void main()
{
	vec3 TextureColor = texture(TextureSampler, UV).rgb;
	OutColor = vec4(TextureColor, 1.0);
}