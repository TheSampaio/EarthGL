#include "PCH.hpp"
#include "Window.hpp"

Window::Window()
    : m_pId(nullptr)
{
    m_Size = { 800, 600 };
    m_Title = "Window";
}

Window::~Window()
{
    if (m_pId)
        glfwDestroyWindow(m_pId);
}

bool Window::Create()
{
    m_pId = glfwCreateWindow(
        m_Size[0],
        m_Size[1],
        m_Title.c_str(),
        nullptr,
        nullptr
    );

    glfwMakeContextCurrent(m_pId);

    return (m_pId) ? true : false;
}

bool Window::IClose()
{
    return glfwWindowShouldClose(m_pId);
}
