#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

#include <stb/stb_image.h>

#define CAMERA_POSITION	0.0f, 0.0f, 18000.0f
#define CAMERA_SPEED 8000.0f
#define CAMERA_MIN_CLIP 0.01f
#define CAMERA_MAX_CLIP 1000000.0f

#define BACKGROUND_COLOR 0.0f, 0.0f, 0.0f, 1.0f

#define EARTH_SCALE 10000.0f

// Window's width and height
static unsigned int WindowWidth = 1024;
static unsigned int WindowHeight = 720;

// Return a file's content
std::string ReadFile(const char* FilePath)
{
	std::string FileSContent;

	if (std::ifstream FileStream{ FilePath, std::ios::in })
	{
		// Read the file's content stored in "FilePath"
		FileSContent.assign(std::istreambuf_iterator<char>(FileStream), std::istreambuf_iterator<char>());
	}

	return FileSContent;
}

// Check if a shader was successfully compiled
void CheckShader(GLuint ShaderID)
{
	// ShaderID must be a compiled shader
	GLint Result = GL_TRUE;
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);

	if (Result == GL_FALSE)
	{
		// Get log's length 
		GLint InfoLogLength = 0;
		glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

		if (InfoLogLength > 0)
		{
			std::string ShaderInfoLog(InfoLogLength, '\0');
			glGetShaderInfoLog(ShaderID, InfoLogLength, nullptr, &ShaderInfoLog[0]);

			std::cout << "[ERRO] Failed to compile shader, " << ShaderInfoLog << std::endl;

			glfwTerminate();
			char Input = getchar();
			if (Input != '\0') exit(0);
		}
	}
}

// Load, build and link shaders to a shader program
GLuint LoadShaders(const char* VertexShaderFile, const char* FragmentShaderFile)
{
	std::string VertexShaderSource, FragmentShaderSource;

	VertexShaderSource = ReadFile(VertexShaderFile);
	FragmentShaderSource = ReadFile(FragmentShaderFile);

	std::cout << "[INFO] Loading \"" << VertexShaderFile; std::cout << "\"" << std::endl;

	if (VertexShaderSource.empty())
	{
		std::cout << "[ERRO] Failed to load \"" << VertexShaderFile; std::cout << "\"" << std::endl << std::endl;
	}

	std::cout << "[INFO] Loading \"" << FragmentShaderFile; std::cout << "\"" << std::endl;

	if (FragmentShaderSource.empty())
	{
		std::cout << "[ERRO] Failed to load \"" << VertexShaderFile; std::cout << "\"" << std::endl << std::endl;
	}

	// Create vertex and fragment shaders's IDs.
	GLuint VertexShaderID, FragmentShaderID;

	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::cout << "[INFO] Building \"" << VertexShaderFile; std::cout << "\"" << std::endl;
	const char* VertexShaderSourcePtr = VertexShaderSource.c_str();
	glShaderSource(VertexShaderID, 1, &VertexShaderSourcePtr, nullptr);
	glCompileShader(VertexShaderID);
	CheckShader(VertexShaderID);

	std::cout << "[INFO] Building \"" << FragmentShaderFile; std::cout << "\"" << std::endl;
	const char* FragmentShaderSourcePtr = FragmentShaderSource.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentShaderSourcePtr, nullptr);
	glCompileShader(FragmentShaderID);
	CheckShader(FragmentShaderID);

	// Create shader program
	std::cout << "======\n[INFO] Linking shader program" << std::endl;
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check if the link was successful
	GLint Result = GL_TRUE;
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);

	if (Result == GL_FALSE)
	{
		GLint InfoLogLength = 0;
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);

		if (InfoLogLength > 0)
		{
			std::string ProgramInfoLog(InfoLogLength, '\0');
			glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramInfoLog[0]);

			std::cout << "[ERRO] Failed to link shader program, " << ProgramInfoLog << std::endl;
			
			glfwTerminate();
			char Input = getchar();
			if (Input != '\0') exit(0);
		}
	}
	
	// Eliminate what we need anymore
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	// Delete what we need anymore
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

// Load and configure textures
GLuint LoadTextures(const char* TextureFile)
{
	// Load a texture file
	std::cout << "[INFO] Loading \"" << TextureFile; std::cout << "\"" << std::endl;

	int TextureWidth = 0, TextureHeight = 0, TextureSlots = 0;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* TextureData = stbi_load(TextureFile, &TextureWidth, &TextureHeight, &TextureSlots, 3);

	// Verify if texture was loaded
	if (!TextureData)
	{
		std::cout << "[ERRO] Failed to load \"" << TextureFile; std::cout << "\"" << std::endl << std::endl;
		assert(TextureData);
	}

	GLuint TextureID;
	glGenTextures(1, &TextureID);			 // Generate texture ID
	glBindTexture(GL_TEXTURE_2D, TextureID); // Enable texture to be modified

	// === Edit image's parameters ======

	// Filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);				  // Mag filter setting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Min filter setting

	// Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	// ==================================

	// Copy texture to VRAM
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureWidth, TextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);

	// Generate texture's mimap
	glGenerateMipmap(GL_TEXTURE_2D);

	// Turn off texture because it was copied to GPU's RAM (VRAM)
	stbi_image_free(TextureData);
	glBindTexture(GL_TEXTURE_2D, 0);

	return TextureID;
}

class CameraBase
{
public:
	// Parameters
	float Speed = CAMERA_SPEED;
	float Sensitivity = 0.1f;

	// View Matrix
	glm::vec3 Location{ CAMERA_POSITION };
	glm::vec3 Direction{ 0.0f,  0.0f, -1.0f };
	glm::vec3 VectorUp{ 0.0f,  1.0f,  0.0f };

	// Projection Matrix
	float FieldOfView = glm::radians(80.0f);
	float AspectRatio = static_cast<float>(WindowWidth) / static_cast<float>(WindowHeight);
	float Near = CAMERA_MIN_CLIP;
	float Far = CAMERA_MAX_CLIP;

	void MoveForward(float Amount)
	{
		Location += glm::normalize(Direction) * Amount * Speed;
	}

	void MoveRight(float Amount)
	{
		glm::vec3 Right = glm::cross(Direction, VectorUp);
		Location += glm::normalize(Right) * Amount * Speed;
	}

	void MoveUp(float Amount)
	{
		Location += glm::normalize(VectorUp) * Amount * Speed;
	}

	void Look(float Pitch, float Yaw)
	{
		Pitch *= Sensitivity;
		Yaw *= Sensitivity;

		const glm::vec3 VectorRight = glm::normalize(glm::cross(Direction, VectorUp));
		const glm::mat4 Identity = glm::identity<glm::mat4>();

		glm::mat4 YawRotation = glm::rotate(Identity, glm::radians(Pitch), VectorUp);
		glm::mat4 PitchRotation = glm::rotate(Identity, glm::radians(Yaw), VectorRight);

		VectorUp = PitchRotation * glm::vec4{ VectorUp, 0.0f };
		Direction = YawRotation * PitchRotation * glm::vec4{ Direction, 0.0f };
	}

	void FixOrientation()
	{
		if ((VectorUp.x != 0.0f) || (VectorUp.z != 0.0f))
		{
			VectorUp.x = 0.0f;
			VectorUp.z = 0.0f;
		}
	}

	void Inputs(GLFWwindow* Window, double DeltaTime)
	{
		// Forward, Backward
		if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
		{
			MoveForward(1.0f * static_cast<float>(DeltaTime));
		}

		else if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
		{
			MoveForward(-1.0f * static_cast<float>(DeltaTime));
		}

		// Right, Left
		if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
		{
			MoveRight(1.0f * static_cast<float>(DeltaTime));
		}

		else if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
		{
			MoveRight(-1.0f * static_cast<float>(DeltaTime));
		}

		// Up, Down
		if (glfwGetKey(Window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			MoveUp(1.0f * static_cast<float>(DeltaTime));
		}

		else if (glfwGetKey(Window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			MoveUp(-1.0f * static_cast<float>(DeltaTime));
		}
	}

	// Get view matrix
	glm::mat4 GetView() const
	{
		return glm::lookAt(Location, Location + Direction, VectorUp);
	}

	// Get view projection matrix
	glm::mat4 GetViewProjection() const
	{
		glm::mat4 Projection = glm::perspective(FieldOfView, AspectRatio, Near, Far);
		return Projection * GetView();
	}
};

// Store vertices, color, texture and normal coordinates
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 Color;
	glm::vec2 UV;
};

struct DirectionalLight
{
	glm::vec3 Direction;
	GLfloat Intensity;
};

GLuint LoadGeometry()
{
	// Define quad's normalized coordinates
	std::array<Vertex, 4> Quad
	{	//				VERTICES								NORMAL			 //			   COLOR			 //			TEXTURE		  //
		Vertex{ glm::vec3{ -1.0f, -1.0f,  0.0f }, glm::vec3 { 0.0f, 0.0f, 1.0f },  glm::vec3{ 1.0f, 1.0f, 1.0f },  glm::vec2{ 0.0f, 0.0f } },
		Vertex{ glm::vec3{  1.0f, -1.0f,  0.0f }, glm::vec3 { 0.0f, 0.0f, 1.0f },  glm::vec3{ 1.0f, 1.0f, 1.0f },  glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{ -1.0f,  1.0f,  0.0f }, glm::vec3 { 0.0f, 0.0f, 1.0f },  glm::vec3{ 1.0f, 1.0f, 1.0f },  glm::vec2{ 0.0f, 1.0f } },
		Vertex{ glm::vec3{  1.0f,  1.0f,  0.0f }, glm::vec3 { 0.0f, 0.0f, 1.0f },  glm::vec3{ 1.0f, 1.0f, 1.0f },  glm::vec2{ 1.0f, 1.0f } },
	};

	// Define quad's normalized coordinates
	std::array<glm::ivec3, 2> Indices
	{
		glm::ivec3 { 0, 1, 2 },
		glm::ivec3 { 2, 1, 3 }
	};

	GLuint VertexArrayObjectQuad, VertexBuffer, ElementBuffer;	// Create variabes to create VAO, VBO and EBO buffers
	glGenBuffers(1, &VertexBuffer);		// Generate our vertex buffer ID
	glGenBuffers(1, &ElementBuffer);	// Generate our element buffer ID

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);							  // Define the vertex buffer as a "Array Buffer"
	glBufferData(GL_ARRAY_BUFFER, sizeof(Quad), Quad.data(), GL_STATIC_DRAW); // Copy triangle's data to VRAM (VBO)

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer); // Copy triangle's data to VRAM (EBO)
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices.data(), GL_STATIC_DRAW);

	// Generate a VAO
	glGenVertexArrays(1, &VertexArrayObjectQuad);

	// Enable VAO
	glBindVertexArray(VertexArrayObjectQuad);

	glEnableVertexAttribArray(0); // Active a pointer to vertex array buffer in Position
	glEnableVertexAttribArray(1); // Active a pointer to vertex array buffer in Normal
	glEnableVertexAttribArray(2); // Active a pointer to vertex array buffer in Color
	glEnableVertexAttribArray(3); // Active a pointer to vertex array buffer in UV

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);		  // Tell to OpenGL that we want to use this array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer); // Tell to OpenGL that we want to use this element buffer

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);										   // Configure vertex buffer's layout 0 (Position)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal))); // Configure vertex buffer's layout 1 (Normal)
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color)));  // Configure vertex buffer's layout 2 (Color)
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, UV)));	   // Configure vertex buffer's layout 3 (Texture)

	glBindVertexArray(0);

	return VertexArrayObjectQuad;
}

void GenerateSphereMesh(GLuint Resolution, std::vector<Vertex>& Vertices, std::vector<glm::ivec3>& Indices)
{
	Vertices.clear();
	Indices.clear();

	constexpr float PI = glm::pi<float>();
	constexpr float TwoPI = glm::two_pi<float>();
	const float InvResolution = 1.0f / static_cast<float>(Resolution - 1);

	for (GLuint Uindex = 0; Uindex < Resolution; ++Uindex)
	{
		const float U = Uindex * InvResolution;
		const float Phi = glm::mix(0.0f, TwoPI, static_cast<float>(U));

		for (GLuint Vindex = 0; Vindex < Resolution; ++Vindex)
		{
			const float V = Vindex * InvResolution;
			const float Theta = glm::mix(0.0f, PI, static_cast<float>(V));

			glm::vec3 VertexPosition
			{
				glm::sin(Theta) * glm::cos(Phi),
				glm::sin(Theta) * glm::sin(Phi),
				glm::cos(Theta)
			};

			Vertex Vertex
			{
				VertexPosition,
				glm::normalize(VertexPosition),
				glm::vec3 { 1.0f, 1.0f, 1.0f },
				glm::vec2 { 1.0 - U, V }
			};

			Vertices.push_back(Vertex);
		}
	}

	for (GLuint U = 0; U < Resolution - 1; ++U)
	{
		for (GLuint V = 0; V < Resolution - 1; ++V)
		{
			GLuint P0 =  U + V * Resolution;
			GLuint P1 = (U + 1) + V * Resolution;
			GLuint P2 = (U + 1) + (V + 1) * Resolution;
			GLuint P3 =  U + (V + 1) * Resolution;

			Indices.push_back(glm::ivec3{ P0, P1, P3 });
			Indices.push_back(glm::ivec3{ P3, P1, P2 });
		}
	}
}

GLuint LoadSphere(GLuint& NumVertices, GLuint& NumIndices)
{
	std::vector<Vertex> Vertices;
	std::vector<glm::ivec3> Triangles;
	GenerateSphereMesh(50, Vertices, Triangles);

	NumVertices = static_cast<GLuint>(Vertices.size());
	NumIndices = static_cast<GLuint>(Triangles.size() * 3);

	GLuint VAO, VBO, EBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), Vertices.data(), GL_STATIC_DRAW);

	// EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, NumIndices * sizeof(GLuint), Triangles.data(), GL_STATIC_DRAW);

	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0); // Active a pointer to vertex array buffer in Position
	glEnableVertexAttribArray(1); // Active a pointer to vertex array buffer in Normal
	glEnableVertexAttribArray(2); // Active a pointer to vertex array buffer in Color
	glEnableVertexAttribArray(3); // Active a pointer to vertex array buffer in UV

	// Enable VBO and EBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);										   // Configure vertex buffer's layout 0 (Position)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal))); // Configure vertex buffer's layout 1 (Normal)
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color)));  // Configure vertex buffer's layout 2 (Color)
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, UV)));	   // Configure vertex buffer's layout 3 (Texture)

	glBindVertexArray(0);

	return VAO;
}

static CameraBase Camera;
bool bEnableMouseMovement = false;
glm::vec2 PreviusCursor{ 0.0f, 0.0f };

// Callbacks
void MouseButtonCallback(GLFWwindow* Window, int Button, int Input, int Modifiers)
{
	if (Button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (Input == GLFW_PRESS)
		{
			double MouseX, MouseY;			
			glfwGetCursorPos(Window, &MouseX, &MouseY);
			PreviusCursor = glm::vec2{ MouseX, MouseY };

			bEnableMouseMovement = true;
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		else if (Input == GLFW_RELEASE)
		{
			bEnableMouseMovement = false;
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}

void MouseMovementCallback(GLFWwindow* Window, double AxisX, double AxisY)
{
	if (bEnableMouseMovement)
	{
		glm::vec2 CurrentCursor{ AxisX, AxisY };
		glm::vec2 DeltaCursor = CurrentCursor - PreviusCursor;

		Camera.Look(-DeltaCursor.x, -DeltaCursor.y);
		PreviusCursor = CurrentCursor;

		Camera.FixOrientation();
	}
}

void ResizeViewportCallback(GLFWwindow* Window, int NewWidth, int NewHeight)
{
	WindowWidth = NewWidth;
	WindowHeight = NewHeight;

	Camera.AspectRatio = static_cast<float>(WindowWidth) / static_cast<float>(WindowHeight);
	glViewport(0, 0, NewWidth, NewHeight);
}

int main()
{
	// Initialize GLFW library
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "[ERRO] Failed to initilize glfw" << std::endl << std::endl;
		glfwTerminate();
	}

	// Create a window
	GLFWwindow* Window = glfwCreateWindow(WindowWidth, WindowHeight, "EarthGL", nullptr, nullptr);

	if (!Window)
	{
		std::cout << "[ERRO] Failed to create a window" << std::endl << std::endl;
		glfwTerminate();
	}

	// Assign new callbacks to GLFW
	glfwSetMouseButtonCallback(Window, MouseButtonCallback);
	glfwSetCursorPosCallback(Window, MouseMovementCallback);
	glfwSetFramebufferSizeCallback(Window, ResizeViewportCallback);

	glfwMakeContextCurrent(Window); // Initialize window's context
	glfwSwapInterval(true);			// Enable V-Sync (Default = true)

	// Initialize GLAD library
	if (!gladLoadGL())
	{
		std::cout << "[ERRO] Failed to initialize glad" << std::endl << std::endl;
		glfwTerminate();
	}

	// Verify OpenGL's version
	std::cout << "[SPEC] OpenGL Version:  " << glGetString(GL_VERSION) << std::endl;
	std::cout << "[SPEC] OpenGL Vendor:   " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "[SPEC] OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "[SPEC] GLSL Version:    " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "======"<< std::endl;

	// Load what we need
	GLuint ProgramID = LoadShaders("shaders/TriangleVertex.glsl", "shaders/TriangleFragment.glsl");
	GLuint TextureID = LoadTextures("textures/complex_earth_4k.jpg");
	GLuint CloudID = LoadTextures("textures/clouds_earth_2k.jpg");
	GLuint ObjectQuad = LoadGeometry();

	GLuint NumSphereVertices = 0;
	GLuint NumSphereIndices = 0;
	GLuint ObjectSphere = LoadSphere(NumSphereVertices, NumSphereIndices);

	std::cout << "======" << std::endl << "[INFO] Sphere Vertices: " << NumSphereVertices << std::endl;
	std::cout << "[INFO] Sphere Indices: " << NumSphereIndices << std::endl << "======" << std::endl;

	// Model Matrix
	glm::mat4 IdentityMatrix = glm::identity<glm::mat4>();
	glm::mat4 ModelMatrix = glm::rotate(IdentityMatrix, glm::radians(90.0f), glm::vec3{1.0f, 0.0f, 0.0f});

	// Define background color
	glClearColor(BACKGROUND_COLOR);

	// Enable backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Define light point
	DirectionalLight Light;
	Light.Direction = glm::vec3{ -0.8f, -0.5f, -1.0f };
	Light.Intensity = 1.4f;

	// Get delta time - #1
	double PreviousTime, CurrentTime, DeltaTime;
	PreviousTime = glfwGetTime();

	// Initialize the gameloop
	while (!glfwWindowShouldClose(Window))
	{
		// Get delta time - #2
		CurrentTime = glfwGetTime();
		DeltaTime = CurrentTime - PreviousTime;

		if (DeltaTime > 0.0)
		{
			PreviousTime = CurrentTime;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear window's framebuffer color and depth buffer
		glUseProgram(ProgramID);							// Active shader program

		glm::mat4 NormalMatrix = glm::inverse(glm::transpose(Camera.GetView() * ModelMatrix));
		glm::mat4 ViewProjectionMatrix = Camera.GetViewProjection();
		glm::mat4 ModelViewProjection = ViewProjectionMatrix * ModelMatrix;

		GLuint TimeLoc = glGetUniformLocation(ProgramID, "Time");
		glUniform1f(TimeLoc, CurrentTime);

		// Get ModelViewProjection uniform from the shader program
		GLuint ModelViewProjectionLoc = glGetUniformLocation(ProgramID, "ModelViewProjection");
		glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(ModelViewProjection));

		// Get NormalMatrix uniform from the shader program
		GLuint NormalMatrixLoc = glGetUniformLocation(ProgramID, "NormalMatrix");
		glUniformMatrix4fv(NormalMatrixLoc, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

		// Get Scale uniform from the shader program
		GLuint ScaleLoc = glGetUniformLocation(ProgramID, "Scale");
		glUniform1f(ScaleLoc, EARTH_SCALE);

		// Get TextureSampler uniform from the shader program
		GLuint TextureSamplerLoc = glGetUniformLocation(ProgramID, "TextureSampler");
		glUniform1i(TextureSamplerLoc, 0);

		GLuint CloudSamplerLoc = glGetUniformLocation(ProgramID, "CloudSampler");
		glUniform1i(CloudSamplerLoc, 1);

		GLuint LighDirectionLoc = glGetUniformLocation(ProgramID, "LightDirection");
		glUniform3fv(LighDirectionLoc, 1, glm::value_ptr(Camera.GetView() * glm::vec4{ Light.Direction, 0.0f }));

		GLuint LightIntensityLoc = glGetUniformLocation(ProgramID, "LightIntensity");
		glUniform1f(LightIntensityLoc, Light.Intensity);

		// Active the loaded textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureID);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, CloudID);

		// glBindVertexArray(ObjectQuad);
		glBindVertexArray(ObjectSphere);

		glPointSize(8.0f);							// Configure point's size
		glLineWidth(4.0f);							// Configure line's width
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // Configure draw mode
		glDrawElements(GL_TRIANGLES, NumSphereIndices, GL_UNSIGNED_INT, nullptr);

		// Disable what we don't need
		glUseProgram(0);
		glBindVertexArray(0);

		glfwPollEvents();		 // Process all window's events
		glfwSwapBuffers(Window); // Swap window's buffers

		// Process camera inputs
		Camera.Inputs(Window, DeltaTime);
	}

	// Delete buffers that we need anymore
	//glDeleteVertexArrays(1, &ObjectQuad);
	glDeleteVertexArrays(1, &ObjectSphere);

	// Finish GLFW library
	glfwTerminate();
	return 0;
}