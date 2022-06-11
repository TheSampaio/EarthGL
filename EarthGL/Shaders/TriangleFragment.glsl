#version 330 core // Specify GLSL's version

in vec3 Normal;
in vec3 Color; // Variable created to input Color
in vec2 UV;    // Variable created to input Texture

out vec4 OutColor; // Variable created to output Color

uniform sampler2D TextureSampler; // Use Texture in shader program with uniform
uniform sampler2D CloudSampler;
uniform float Time;

uniform vec2 CloudRotationSpeed = vec2(0.002, -0.001);

uniform vec3 LightDirection;
uniform float LightIntensity;

void main()
{
	// Renormalize normals
	vec3 N = normalize(Normal);

	// Get L vector
	vec3 L = -normalize(LightDirection);

	// Get lambertian
	float Lambertian = max(dot(N, L), 0.0);

	// Get V vector
	vec3 V = vec3(0.0, 0.0, 1.0);

	// Get R vector
	vec3 R = reflect(-L, N);

	// Specular (R, V) ^ Alpha
	float Alpha = 40.0;
	float Specular = pow(max(dot(R, L), 0.0), Alpha);

	Specular = max(Specular, 0.0);

	vec3 EarthColor = texture(TextureSampler, UV).rgb; // Get texture's color
	vec3 CloudColor = texture(CloudSampler, UV + Time * CloudRotationSpeed).rgb;
	vec3 FinalColor = (EarthColor + CloudColor) * LightIntensity * Lambertian + Specular;

	OutColor = vec4(FinalColor, 1.0); // Get the final color
}