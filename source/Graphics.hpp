#ifndef EARTHGL_GRAPHICS
#define EARTHGL_GRAPHICS

class Application;
class Window;

class Graphics
{
public:
    friend Application;

private:
    Graphics() {};

    Graphics(const Graphics&) = delete;
    Graphics operator=(const Graphics&) = delete;

    bool Initialize();
    void ClearBuffers();
    void SwapBuffers(Window& window);

    static Graphics& GetInstance() { static Graphics sInstance; return sInstance; }
};

#endif
