#include "PCH.hpp"
#include "Graphics.hpp"

#include "Debug.hpp"
#include "Window.hpp"

Graphics::Graphics()
{
    m_BackgoundColour = {0, 0, 0};
    m_VerticalSynchronization = true;
}

bool Graphics::Initialize()
{
    return (glewInit() == GLEW_OK) ? true : false;
}

void Graphics::ClearBuffers()
{
    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(
        static_cast<GLfloat>(m_BackgoundColour[0] / 255.0f),
        static_cast<GLfloat>(m_BackgoundColour[1] / 255.0f),
        static_cast<GLfloat>(m_BackgoundColour[2] / 255.0f),
        1.0
    );
}

void Graphics::SwapBuffers(Window& window)
{
    glfwSwapInterval(m_VerticalSynchronization);
    glfwSwapBuffers(window.GetId());
}

void Graphics::CreateViewport(Window &window)
{
    glViewport(0, 0, window.GetSize()[0], window.GetSize()[1]);

    Debug::Console(Information, std::format("Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER))));
    Debug::Console(Information, std::format("Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR))));

    if (const char* byte = reinterpret_cast<const char*>(glGetString(GL_VERSION)))
    {
        string text(byte);
        string data = text.substr(0, text.find_first_of(" "));
        Debug::Console(Information, std::format("OpenGL: {}\n===", data));
    }
}

void Graphics::ISetFaceCulling(bool enable)
{
    (enable) ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);
}
