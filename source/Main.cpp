#include "PCH.hpp"

#include "Application.hpp"
#include "Debug.hpp"
#include "Game.hpp"
#include "Graphics.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"
#include "Window.hpp"

class Sandbox : public Game
{
public:
    Sandbox();

protected:
    void OnStart() override;
    void OnUpdate() override;
    void OnDraw() override;
    void OnFinalize() override;

private:
    Mesh* mEarth;
};

Sandbox::Sandbox()
    : mEarth(nullptr)
{
    Window::SetSize(800, 800);
    Window::SetTitle("EarthGL");

    Graphics::SetBackgroundColour(40, 40, 40);
    Graphics::SetVerticalSynchronization(false);
}

void Sandbox::OnStart()
{
    Debug::Console(Information, "The application was started.");

    const std::vector<Vertex> vertices
    {
        { glm::vec3{ -0.5f,  0.5f,  0.0f } },
        { glm::vec3{  0.5f,  0.5f,  0.0f } },
        { glm::vec3{ -0.5f, -0.5f,  0.0f } },
        { glm::vec3{  0.5f, -0.5f,  0.0f } },
    };

    const std::vector<GLuint> indices
    {
        0, 1, 2,
        2, 1, 3
    };

    mEarth = new Mesh(vertices, indices);
}

void Sandbox::OnUpdate()
{
}

void Sandbox::OnDraw()
{
    mEarth->Draw(Renderer::GetShaderProgram());
}

void Sandbox::OnFinalize()
{
    Debug::Console(Information, "The application was finalized.");
}

int main()
{
    Application::Run(new Sandbox);
}
