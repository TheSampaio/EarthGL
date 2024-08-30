#shader vertex
#version 330

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec3 iColour;

out vec3 vColour;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vColour = iColour;
    gl_Position = uProjection * uView * uModel * vec4(iPosition, 1.0);
}

#shader fragment
#version 330

in vec3 vColour;
out vec4 oFragment;

void main()
{
    oFragment = vec4(vColour, 1.0);
}
