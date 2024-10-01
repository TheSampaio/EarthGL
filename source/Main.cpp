#include "PCH.hpp"

#include "Application.hpp"
#include "Camera.hpp"
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
    void OnUpdate(float deltaTime) override;
    void OnDraw() override;
    void OnFinalize() override;

private:
    Camera* mCamera;
    Mesh* mEarthMesh;
    Texture* mEarthBathy;
    Texture* mEarthShallow;
    Texture* mEarthClouds;
    glm::mat4 mModel;
    float mCounter;

    void GenerateSphereMesh(GLuint resolution, std::vector<Vertex>& vertices, std::vector<glm::uvec3>& indices);
};

Sandbox::Sandbox()
    : mCamera(nullptr), mEarthMesh(nullptr), mEarthBathy(nullptr), mEarthShallow(nullptr), mEarthClouds(nullptr), mModel(glm::mat4{ 1.0f })
{
    Graphics::SetAntiAliasing(true);
    Graphics::SetBackgroundColour(2.0f, 2.0f, 2.0f);
    Graphics::SetVerticalSynchronization(false);

    Window::SetSize(1280, 720);
    Window::SetTitle("EarthGL");
}

void Sandbox::OnStart()
{
    Debug::Console(Information, "The application was started.");

    // Set-up face culling
    Graphics::SetFaceCulling(true);

    // Generates all the sphere's vertices and indices
    std::vector<Vertex> vertices;
	std::vector<glm::uvec3> indices;

	GenerateSphereMesh(50, vertices, indices);

    mCamera = new Camera(glm::vec3(0.0f, 0.0f, 500.0f), 60.0f, 0.05f, 1000.0f);
    mEarthMesh = new Mesh(vertices, indices);
    mEarthBathy = new Texture("../../data/texture/diffuse-earth-bathy-4k.jpg", GL_RGB, GL_TEXTURE0);
    mEarthShallow = new Texture("../../data/texture/diffuse-earth-shallow-4k.jpg", GL_RGB, GL_TEXTURE1);
    mEarthClouds = new Texture("../../data/texture/diffuse-earth-clouds-2k.jpg", GL_RGB, GL_TEXTURE2);

    // Set-up the Earth's initial transform
    mModel = glm::rotate(mModel, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    mModel = glm::rotate(mModel, glm::radians(200.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    mModel = glm::scale(mModel, 200.0f * glm::vec3(1.0f));
}

void Sandbox::OnUpdate(float deltaTime)
{
    mCounter += deltaTime;
    mCamera->Update(deltaTime);

    // Creates a normal matrix
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(mModel));

    // Earth's rotation
    mModel = glm::rotate(mModel, glm::radians(-0.8f  * deltaTime), glm::vec3(0.0f, 0.0f, 1.0f));

    Renderer::UseShaderProgram();
    Renderer::SetUniform1f("uSpeed", mCounter);
    Renderer::SetUniformMatrix4fv("uModel", mModel);
    Renderer::SetUniformMatrix4fv("uNormalMatrix", normalMatrix);
}

void Sandbox::OnDraw()
{
    mCamera->Draw();
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
    delete mCamera;

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
