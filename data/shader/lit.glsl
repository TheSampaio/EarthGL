#shader vertex
#version 330

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec2 iTexture;

out vec2 vTexture;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vTexture = iTexture;
    gl_Position = uProjection * uView * uModel * vec4(iPosition, 1.0);
}

#shader fragment
#version 330

in vec2 vTexture;
out vec4 oFragment;

uniform sampler2D uBathy2D;
uniform sampler2D uShallow2D;
uniform sampler2D uClouds2D;

void main()
{
    vec4 bathy = texture(uBathy2D, vTexture);
    vec4 shallow = texture(uShallow2D, vTexture);
    vec4 clouds = texture(uClouds2D, vTexture);
    
    oFragment = mix(bathy, shallow, 0.6) + clouds;
}
