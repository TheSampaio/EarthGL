#include "PCH.hpp"
#include "Graphics.hpp"
#include "Window.hpp"

bool Graphics::Initialize()
{
    return (glewInit() == GLEW_OK) ? true : false;
}

void Graphics::ClearBuffers()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::SwapBuffers(Window& window)
{
    glfwSwapInterval(1);
    glfwSwapBuffers(window.GetId());
}
