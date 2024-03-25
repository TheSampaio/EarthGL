#include "Graphics.hpp"

#include <GL/glew.h>

bool Graphics::Init()
{
	return (glewInit() != GLEW_OK) ? false : true;
}

void Graphics::ClearBuffers(const int red, const int green, const int blue)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(static_cast<GLfloat>(red / 255.0f),
		static_cast<GLfloat>(green / 255.0f),
		static_cast<GLfloat>(blue / 255.0f),
		1.0f);
}

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}
