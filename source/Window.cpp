#include "PCH.hpp"
#include "Window.hpp"

Window::Window()
    : mpId(nullptr)
{
    mSize = { 800, 600 };
    mTitle = "Window";
}

Window::~Window()
{
    if (mpId)
        glfwDestroyWindow(mpId);
}

bool Window::Create()
{
    // Set-up MSAA with eight samples
    glfwWindowHint(GLFW_SAMPLES, 8);

    // Set-up OpenGL's version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Set-up OpenGL's compatibility mode
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

    mpId = glfwCreateWindow(
        mSize[0],
        mSize[1],
        mTitle.c_str(),
        nullptr,
        nullptr
    );

    glfwMakeContextCurrent(mpId);

    return (mpId) ? true : false;
}

bool Window::IClose()
{
    return glfwWindowShouldClose(mpId);
}
