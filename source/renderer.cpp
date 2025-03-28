#include "pch.hpp"
#include "renderer.hpp"

#include "debug.hpp"

struct EShaderSource
{
    std::string vertex;
    std::string fragment;
};

Renderer::~Renderer()
{
    glDeleteProgram(mShaderProgram);
}

bool Renderer::Initialize()
{
    // Compiling
    EShaderSource source = Parse(PROJECT_RESOURCE_DIR "shader/Lit.glsl");

    GLuint vertexShader = Compile(GL_VERTEX_SHADER, source.vertex);
    GLuint fragmentShader = Compile(GL_FRAGMENT_SHADER, source.fragment);

    mShaderProgram = glCreateProgram();
    glAttachShader(mShaderProgram, vertexShader);
    glAttachShader(mShaderProgram, fragmentShader);
    glLinkProgram(mShaderProgram);

    return true;
}

EShaderSource Renderer::Parse(const string &filepath)
{
    enum class ShaderType
    {
        None = -1,
        Vertex = 0,
        Fragment = 1,
    };

    std::ifstream file(filepath);
    std::string line;
    std::stringstream source[2];
    ShaderType shader = ShaderType::None;

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                    shader = ShaderType::Vertex;

                else if (line.find("fragment") != std::string::npos)
                    shader = ShaderType::Fragment;
            }

            else
            {
                source[(int)shader] << line << '\n';
            }
        }
    }

    else
        Debug::Console(Warning, "Failed to locate shader file.");

    return { source[0].str(), source[1].str() };
}

GLuint Renderer::Compile(GLuint type, const std::string &source)
{
    GLuint id = glCreateShader(type);
    const GLchar* const content = source.c_str();

    glShaderSource(id, 1, &content, nullptr);
    glCompileShader(id);

    // Log errors
    GLint result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        GLint length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        GLchar* message = (GLchar*)alloca(length * sizeof(GLchar));
        glGetShaderInfoLog(id, length, &length, message);

        Debug::Console(Error, std::format("Failed to compile {} shader.", (type == GL_VERTEX_SHADER) ? "vertex" : "fragment"));
        Debug::Console(Error, message);

        glDeleteShader(id);
        return 0;
    }

    return id;
}
