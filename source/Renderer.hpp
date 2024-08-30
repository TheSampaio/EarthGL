#ifndef _EARTHGL_RENDERER_
#define _EARTHGL_RENDERER_

class Application;
struct EShaderSource;

class Renderer
{
public:
    static GLuint GetShaderProgram() { return GetInstance().mShaderProgram; }

    void Draw() {};

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