#include "PCH.hpp"

#include "Application.hpp"
#include "Debug.hpp"
#include "Game.hpp"
#include "Graphics.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
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
    GLuint texture;
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
        { glm::vec3{ -0.5f,  0.5f,  0.0f }, glm::vec2 {  0.0f, 0.0f  } },
        { glm::vec3{  0.5f,  0.5f,  0.0f }, glm::vec2 {  0.0f, 1.0f  } },
        { glm::vec3{ -0.5f, -0.5f,  0.0f }, glm::vec2 {  1.0f, 0.0f  } },
        { glm::vec3{  0.5f, -0.5f,  0.0f }, glm::vec2 {  1.0f, 1.0f  } },
    };

    const std::vector<GLuint> indices
    {
        0, 1, 2,
        2, 1, 3
    };

    mEarth = new Mesh(vertices, indices);

    // === Texture loading ===

    // Flips image on load
    stbi_set_flip_vertically_on_load(true);

    // Loads a image from disk
    int texWidth = 0, texHeight = 0, column = 0;
    unsigned char* bytes = stbi_load("../../data/texture/complex-earth-4k.jpg", &texWidth, &texHeight, &column, 0);

    glGenTextures(1, &texture);

    // Binds a 2D texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set-ups the minification and magnification filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set-ups the texture's wrap behavior
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set-ups the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);

    // Generates a mipmap for the 2D texture
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free the data loaded from disk
    stbi_image_free(bytes);

    // Unbinds the 2D texture to avoid bugs
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Sandbox::OnUpdate()
{
    // === 3D stuff ===

    glm::mat4 model = glm::mat4(1.f);
    glm::mat4 view = glm::mat4(1.f);
    glm::mat4 projection = glm::mat4(1.f);

    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, glm::radians(55.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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
    glUseProgram(Renderer::GetShaderProgram());
    glUniform1i(glGetUniformLocation(Renderer::GetShaderProgram(), "uDiffuseSampler"), 0);

    // Activates the texture slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

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
