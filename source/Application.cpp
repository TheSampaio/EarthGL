#include "PCH.hpp"

#include "Application.hpp"
#include "Debug.hpp"
#include "Game.hpp"
#include "Graphics.hpp"
#include "Input.hpp"
#include "Window.hpp"

Application::Application()
    : m_pGame(nullptr)
{
    if (glfwInit() != GLFW_TRUE)
        Debug::Console("[ERRO] Failed to initialize GLFW.");
}

Application::~Application()
{
    delete m_pGame;
    glfwTerminate();
}

void Application::IRun(Game& game)
{
    m_pGame = &game;

    if (!Window::GetInstance().Create())
        Debug::Console("[ERRO] Failed to create the window.");

    if (!Graphics::GetInstance().Initialize())
        Debug::Console("[ERRO] Failed to initialize GLEW.");

    game.OnStart();

    do
    {
        Input::GetInstance().PollEvents();
        game.OnUpdate();

        Graphics::GetInstance().ClearBuffers();
        game.OnDraw();
        
        Graphics::GetInstance().SwapBuffers(Window::GetInstance());

    } while (!Window::Close());
    
    game.OnFinalize();
}
