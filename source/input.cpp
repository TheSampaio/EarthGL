#include "pch.hpp"
#include "input.hpp"

#include "debug.hpp"
#include "graphics.hpp"
#include "window.hpp"

static void OnFramebufferResize(GLFWwindow* window, int width, int height)
{
    Window::SetSize(width, height);
    glViewport(0, 0, width, height);
}

bool Input::IGetKeyPress(EKeyCode key)
{
    return (glfwGetKey(Window::GetId(), key) == GLFW_PRESS) ? true : false;
}

bool Input::IGetKeyRelease(EKeyCode key)
{
    return (glfwGetKey(Window::GetId(), key) == GLFW_RELEASE) ? true : false;
}

bool Input::IGetKeyTap(EKeyCode key)
{
    return false;
}

void Input::PollEvents()
{
    glfwPollEvents();
    
    // Assign callbacks (Events)
    glfwSetFramebufferSizeCallback(Window::GetId(), OnFramebufferResize);
}