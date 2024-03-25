// Specifies GLSL's version
#version 330 core

// Specifies input layouts
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec3 InNormal;
layout(location = 2) in vec3 InColor;
layout(location = 3) in vec2 InUV;

out vec3 Normal;
out vec3 Color; // Variable created to output Color
out vec2 UV;	// Variable created to output Texture

uniform mat4 NormalMatrix;
uniform mat4 ModelViewProjection; // Use MVP in shader program with uniform
uniform float Scale;			  // Use Scale in shader program with uniform

void main()
{
	Normal = vec3(NormalMatrix * vec4(InNormal, 0.0f));
	Color = InColor; // Send color to fragment shader
	UV = InUV;		 // Send texture to fragment shader

	gl_Position = ModelViewProjection * vec4(InPosition * Scale, 1.0); // Send vertice's position to fragment shader
}