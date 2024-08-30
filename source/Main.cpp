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
    Window::SetSize(800, 600);
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
    glm::mat4 model = glm::mat4(1.f);
    glm::mat4 view = glm::mat4(1.f);
    glm::mat4 projection = glm::mat4(1.f);

    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));

    float aspect = static_cast<float>(Window::GetSize()[0]) / static_cast<float>(Window::GetSize()[1]);
    projection = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 100.0f);

    glUseProgram(Renderer::GetShaderProgram());

    glUniformMatrix4fv(glGetUniformLocation(Renderer::GetShaderProgram(), "uModel"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(Renderer::GetShaderProgram(), "uView"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(Renderer::GetShaderProgram(), "uProjection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void Sandbox::OnDraw()
{
    mEarth->Draw(Renderer::GetShaderProgram());
}

void Sandbox::OnFinalize()
{
    delete mEarth;

    Debug::Console(Information, "The application was finalized.");
}

int main()
{
    Application::Run(new Sandbox);
}
