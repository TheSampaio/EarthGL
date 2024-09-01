#ifndef _EARTHGL_RENDERER_
#define _EARTHGL_RENDERER_

class Application;
struct EShaderSource;

class Renderer
{
public:
    static GLuint GetShaderProgram() { return GetInstance().mShaderProgram; }

	// Set-ups an uniform 1 integer
	static void SetUniform1i(const GLchar* name, GLint value)
	{ glUniform1i(glGetUniformLocation(Renderer::GetInstance().mShaderProgram, name), value); }

	// Set-ups an uniform 1 float
	static void SetUniform1f(const GLchar* name, GLfloat value)
	{ glUniform1f(glGetUniformLocation(Renderer::GetInstance().mShaderProgram, name), value); }

	// Set-ups an uniform 3 float
	static void SetUniform3f(const GLchar* name, GLfloat value1, GLfloat value2, GLfloat value3)
	{ glUniform3f(glGetUniformLocation(Renderer::GetInstance().mShaderProgram, name), value1, value2, value3); }

	// Set-ups an uniform matrix 4 float vector
	static void SetUniformMatrix4fv(const GLchar* name, glm::mat4 value)
	{ glUniformMatrix4fv(glGetUniformLocation(Renderer::GetInstance().mShaderProgram, name), 1, GL_FALSE, glm::value_ptr(value)); }

    // Activates the shader program
    static void UseShaderProgram()
    { glUseProgram(Renderer::GetInstance().mShaderProgram); }

    // Draw using indices
	static void Draw(std::vector<glm::uvec3> indices, GLenum primitive = GL_TRIANGLES)
	{ glDrawElements(primitive, static_cast<GLsizei>(indices.size() * 3), GL_UNSIGNED_INT, nullptr); }

    friend Application;

private:
    GLuint mShaderProgram;

    Renderer() {};
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer operator=(const Renderer&) = delete;

    bool Initialize();
    EShaderSource Parse(const string& filepath);
    GLuint Compile(GLuint shader, const std::string& source);

    static Renderer& GetInstance() { static Renderer sInstance; return sInstance; }
};

#endif