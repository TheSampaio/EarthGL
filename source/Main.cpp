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
    Mesh* mEarthMesh;
    Texture* mEarthBathy;
    Texture* mEarthShallow;
    Texture* mEarthClouds;

    void GenerateSphereMesh(GLuint resolution, std::vector<Vertex>& vertices, std::vector<glm::uvec3>& indices);
};

Sandbox::Sandbox()
    : mEarthMesh(nullptr), mEarthBathy(nullptr), mEarthShallow(nullptr), mEarthClouds(nullptr)
{
    Window::SetSize(1280, 720);
    Window::SetTitle("EarthGL");

    Graphics::SetBackgroundColour(10, 10, 10);
    Graphics::SetVerticalSynchronization(false);
}

void Sandbox::OnStart()
{
    Debug::Console(Information, "The application was started.");

    // Generates all the sphere's vertices and indices
    std::vector<Vertex> vertices;
	std::vector<glm::uvec3> indices;

	GenerateSphereMesh(50, vertices, indices);

    mEarthMesh = new Mesh(vertices, indices);
    mEarthBathy = new Texture("../../data/texture/diffuse-earth-bathy-4k.jpg", GL_RGB, GL_TEXTURE0);
    mEarthShallow = new Texture("../../data/texture/diffuse-earth-shallow-4k.jpg", GL_RGB, GL_TEXTURE1);
    mEarthClouds = new Texture("../../data/texture/diffuse-earth-clouds-2k.jpg", GL_RGB, GL_TEXTURE2);
}

void Sandbox::OnUpdate()
{
    // === 3D stuff ===
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(200.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, 10.0f * glm::vec3(1.0f));

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -24.0f));

    const float aspect = static_cast<float>(Window::GetSize()[0]) / static_cast<float>(Window::GetSize()[1]);
    projection = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 100.0f);

    Renderer::UseShaderProgram();
    Renderer::SetUniformMatrix4fv("uModel", model);
    Renderer::SetUniformMatrix4fv("uView", view);
    Renderer::SetUniformMatrix4fv("uProjection", projection);
}

void Sandbox::OnDraw()
{
    mEarthMesh->Draw();

    Renderer::SetUniform1i("uBathy2D", 0);
    mEarthBathy->Bind();

    Renderer::SetUniform1i("uShallow2D", 1);
    mEarthShallow->Bind();

    Renderer::SetUniform1i("uClouds2D", 2);
    mEarthClouds->Bind();
}

void Sandbox::OnFinalize()
{
    delete mEarthClouds;
    delete mEarthShallow;
    delete mEarthBathy;
    delete mEarthMesh;

    Debug::Console(Information, "The application was finalized.");
}

void Sandbox::GenerateSphereMesh(GLuint resolution, std::vector<Vertex> &vertices, std::vector<glm::uvec3> &indices)
{
	constexpr float PI = glm::pi<GLfloat>();
	constexpr float twoPI = glm::two_pi<GLfloat>();
	const float invResolution = 1.0f / static_cast<float>(resolution - 1);

	for (GLuint uIndex = 0; uIndex < resolution; ++uIndex)
	{
		const float U = uIndex * invResolution;
		const float phi = glm::mix(0.0f, twoPI, static_cast<float>(U));

		for (GLuint vIndex = 0; vIndex < resolution; ++vIndex)
		{
			const float V = vIndex * invResolution;
			const float theta = glm::mix(0.0f, PI, static_cast<GLfloat>(V));

			glm::vec3 vertexPosition
			{
				glm::sin(theta) * glm::cos(phi),
				glm::sin(theta) * glm::sin(phi),
				glm::cos(theta)
			};

			Vertex vertex
			{
				vertexPosition,
				glm::normalize(vertexPosition),
				glm::vec2 { -U, V }
			};

			vertices.push_back(vertex);
		}
	}

	for (GLuint U = 0; U < resolution - 1; ++U)
	{
		for (GLuint V = 0; V < resolution - 1; ++V)
		{
			GLuint P0 =  U + V * resolution;
			GLuint P1 = (U + 1) + V * resolution;
			GLuint P2 = (U + 1) + (V + 1) * resolution;
			GLuint P3 =  U + (V + 1) * resolution;

			indices.push_back(glm::ivec3{ P0, P1, P3 });
			indices.push_back(glm::ivec3{ P3, P1, P2 });
		}
	}
}

int main()
{
    Application::Run(new Sandbox);
}
