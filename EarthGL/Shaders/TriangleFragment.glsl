#version 330 core // Specify GLSL's version

in vec3 Color; // Variable created to input Color
in vec2 UV;    // Variable created to input Texture

out vec4 OutColor; // Variable created to output Color

uniform sampler2D TextureSampler; // Use Texture in shader program with uniform

void main()
{
	vec3 TextureColor = texture(TextureSampler, UV).rgb; // Get texture's color
	OutColor = vec4(TextureColor, 1.0);					 // Get the final color
}