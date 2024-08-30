#include "PCH.hpp"
#include "Input.hpp"

#include "Graphics.hpp"
#include "Window.hpp"

static void OnFramebufferResize(GLFWwindow* window, int width, int height)
{
    Window::SetSize(width, height);
    glViewport(0, 0, width, height);
}

void Input::PollEvents()
{
    glfwPollEvents();
    glfwSetFramebufferSizeCallback(Window::GetId(), OnFramebufferResize);
}