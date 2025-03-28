#include "pch.hpp"

#include "application.hpp"
#include "debug.hpp"
#include "game.hpp"
#include "graphics.hpp"
#include "input.hpp"
#include "renderer.hpp"
#include "timer.hpp"
#include "window.hpp"

Application::Application()
    : mpGame(nullptr), mpTimer(nullptr)
{
    if (glfwInit() != GLFW_TRUE)
        Debug::Console(Warning, "Failed to initialize GLFW.");

    mpTimer = new Timer();
}

Application::~Application()
{
    delete mpGame;
    delete mpTimer;
    glfwTerminate();
}

void Application::IRun(Game& game)
{
    Graphics& graphics = Graphics::GetInstance();
    Input& input = Input::GetInstance();
    Renderer& renderer = Renderer::GetInstance();
    Window& window = Window::GetInstance();

    mpGame = &game;

    if (!window.Create())
        Debug::Console(Error, "Failed to create the window.");

    if (!graphics.Initialize())
        Debug::Console(Error, "Failed to initialize GLEW.");

    if(!renderer.Initialize())
        Debug::Console(Error, "Failed to initialize the renderer.");

    graphics.CreateViewport(window);
    mpTimer->Start();
    mpGame->OnStart();

    do
    {
        input.PollEvents();
        mpGame->OnUpdate(mpTimer->Reset());

        graphics.ClearBuffers();
        mpGame->OnDraw();
        
        graphics.SwapBuffers(window);

    } while (!Window::Close());
    
    mpGame->OnFinalize();
}
