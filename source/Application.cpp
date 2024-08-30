#include "PCH.hpp"

#include "Application.hpp"
#include "Debug.hpp"
#include "Game.hpp"
#include "Graphics.hpp"
#include "Input.hpp"
#include "Renderer.hpp"
#include "Window.hpp"

Application::Application()
    : m_pGame(nullptr)
{
    if (glfwInit() != GLFW_TRUE)
        Debug::Console(Error, "Failed to initialize GLFW.");
}

Application::~Application()
{
    delete m_pGame;
    glfwTerminate();
}

void Application::IRun(Game& game)
{
    Input& input = Input::GetInstance();
    Window& window = Window::GetInstance();
    Graphics& graphics = Graphics::GetInstance();
    Renderer& renderer = Renderer::GetInstance();

    m_pGame = &game;

    if (!window.Create())
        Debug::Console(Error, "Failed to create the window.");

    if (!graphics.Initialize())
        Debug::Console(Error, "Failed to initialize GLEW.");

    renderer.Initialize();

    graphics.CreateViewport(window);

    game.OnStart();

    do
    {
        input.PollEvents();
        game.OnUpdate();

        graphics.ClearBuffers();
        game.OnDraw();
        
        graphics.SwapBuffers(window);

    } while (!Window::Close());
    
    game.OnFinalize();
}
