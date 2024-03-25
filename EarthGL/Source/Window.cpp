#include "Window.hpp"
#include "Graphics.hpp"
#include "Debug.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

EarthGL::Window::Window()
    : id(nullptr)
{
    if (!glfwInit())
    {
        Debug::GetInstance().Print("[ERROR] Failed to initialize GLFW.", false);
        Debug::GetInstance().Pause();
    }
}

EarthGL::Window::~Window()
{
    if (id)
        glfwDestroyWindow(id);

    glfwTerminate();
}

bool EarthGL::Window::Create(Graphics& graphics)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    id = glfwCreateWindow(size[0], size[1], "EarthGL", nullptr, nullptr);

    if (!id)
    {
        Debug::GetInstance().Print("[ERROR] Failed to create the window.", false);
        Debug::GetInstance().Pause();
        return false;
    }

    if (graphics.Init())
    {
        Debug::GetInstance().Print("[ERROR] Failed to initialize graphics.", false);
        Debug::GetInstance().Pause();
        return false;
    }

    glfwMakeContextCurrent(id);
    glViewport(0, 0, size[0], size[1]);

    return true;
}

bool EarthGL::Window::Close()
{
    return glfwWindowShouldClose(id);
}

void EarthGL::Window::PollEvents()
{
    glfwPollEvents();
}

void EarthGL::Window::SwapBuffers()
{
    glfwSwapInterval(true);
    glfwSwapBuffers(id);
}
