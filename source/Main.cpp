#include "PCH.hpp"

#include "Application.hpp"
#include "Debug.hpp"
#include "Game.hpp"
#include "Window.hpp"

class Sandbox : public Game
{
public:
    Sandbox();

private:
    virtual void OnStart() override;
    virtual void OnUpdate() override;
    virtual void OnDraw() override;
    virtual void OnFinalize() override;
};

Sandbox::Sandbox()
{
    Window::SetSize(1280, 720);
    Window::SetTitle("EarthGL");
}

void Sandbox::OnStart()
{
    Debug::Console("[INFO] Started.");
}

void Sandbox::OnUpdate()
{
}

void Sandbox::OnDraw()
{
}

void Sandbox::OnFinalize()
{
    Debug::Console("[INFO] Finalized.");
}

int main()
{
    Application::Run(new Sandbox);
}
