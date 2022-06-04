#include <iostream>
#include <array>
#include <fstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <stb/stb_image.h>

// Window's width and height
static const unsigned int WindowWidth = 1024;
static const unsigned int WindowHeight = 720;

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

// Store vertices, color, texture and normal coordinates
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec2 UV;
};

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

	// Initialize window's context
	glfwMakeContextCurrent(Window);

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
	GLuint TextureID = LoadTextures("textures/earth_2k.jpg");

	// Define quad's normalized coordinates
	std::array<Vertex, 6> Quad
	{	//				VERTICES				 //			   COLOR			 //			TEXTURE		  //
		Vertex{ glm::vec3{ -1.0f, -1.0f,  0.0f },  glm::vec3{ 1.0f, 1.0f, 1.0f },  glm::vec2{ 0.0f, 0.0f } },
		Vertex{ glm::vec3{  1.0f, -1.0f,  0.0f },  glm::vec3{ 1.0f, 1.0f, 1.0f },  glm::vec2{ 1.0f, 0.0f } },
		Vertex{ glm::vec3{ -1.0f,  1.0f,  0.0f },  glm::vec3{ 1.0f, 1.0f, 1.0f },  glm::vec2{ 0.0f, 1.0f } },

		Vertex{ glm::vec3{ -1.0f,  1.0f,  0.0f }, glm::vec3 { 1.0f, 1.0f, 1.0f },  glm::vec2{ 0.0f, 1.0f} },
		Vertex{ glm::vec3{  1.0f, -1.0f,  0.0f }, glm::vec3 { 1.0f, 1.0f, 1.0f },  glm::vec2{ 1.0f, 0.0f} },
		Vertex{ glm::vec3{  1.0f,  1.0f,  0.0f }, glm::vec3 { 1.0f, 1.0f, 1.0f },  glm::vec2{ 1.0f, 1.0f} }
	};

	// Model Matrix
	glm::mat4 ModelMatrix = glm::identity<glm::mat4>();

	// View Matrix
	glm::vec3 Position(0, 0, 2);
	glm::vec3 Direction(0, 0, 0);
	glm::vec3 UpVector(0, 1, 0);
	glm::mat4 ViewMatrix = glm::lookAt(Position, Direction, UpVector);

	// Projection Matrix
	constexpr float FieldOfView = glm::radians(80.0f);
	const float AspectRatio = WindowWidth / WindowHeight;
	const float Near = 0.001f;
	const float Far = 1000.0f;
	glm::mat4 ProjectionMatrix = glm::perspective(FieldOfView, AspectRatio, Near, Far);

	// Model View Projection Matrix
	glm::mat4 ModelViewProjection = ProjectionMatrix * ViewMatrix * ModelMatrix;
	
	GLuint VertexBuffer;													  // Copy triangle data from RAM to GPU's memory
	glGenBuffers(1, &VertexBuffer);											  // Generate our vertex buffer ID
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);							  // Define the vertex buffer as a "Array Buffer"
	glBufferData(GL_ARRAY_BUFFER, sizeof(Quad), Quad.data(), GL_STATIC_DRAW); // Copy triangle's data to VRAM

	// Define background color
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	// Initialize the gameloop
	while (!glfwWindowShouldClose(Window))
	{
		glClear(GL_COLOR_BUFFER_BIT); // Clear window's framebuffer color
		glUseProgram(ProgramID);	  // Active shader program

		// Get ModelViewProjection uniform from the shader program
		GLint ModelViewProjectionLoc = glGetUniformLocation(ProgramID, "ModelViewProjection");
		glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(ModelViewProjection));

		// Get TextureSampler uniform from the shader program
		GLint TextureSamplerLoc = glGetUniformLocation(ProgramID, "TextureSampler");
		glUniform1i(TextureSamplerLoc, 0);

		// Active the loaded texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureID);

		glEnableVertexAttribArray(0); // Active a pointer to vertex array buffer in Position
		glEnableVertexAttribArray(1); // Active a pointer to vertex array buffer in Color
		glEnableVertexAttribArray(2); // Active a pointer to vertex array buffer in UV

		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer); // Tell to OpenGL that we want to use this array buffer

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);										   // Configure vertex buffer's layout 0 (Position)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE,  sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color))); // Configure vertex buffer's layout 1 (Color)
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE,  sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, UV)));	   // Configure vertex buffer's layout 2 (Texture)

		glDrawArrays(GL_TRIANGLES, 0, Quad.size()); // Draw our quad

		// Turn off what we need anymore
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Disable shader program
		glUseProgram(0);

		glfwPollEvents();		 // Process all window's events
		glfwSwapBuffers(Window); // Swap window's buffers
	}

	// Delete buffers that we need anymore
	glDeleteBuffers(1, &VertexBuffer);

	// Finish GLFW library
	glfwTerminate();
	return 0;
}