#ifndef _EARTHGL_GRAPHICS_
#define _EARTHGL_GRAPHICS_

class Application;
class Window;

class Graphics
{
public:
    static void SetBackgroundColour(uchar red, uchar green, uchar blue) { GetInstance().m_BackgoundColour = { red, green, blue }; }

    static void SetFaceCulling(bool enable) { GetInstance().ISetFaceCulling(enable); }

    static void SetVerticalSynchronization(bool enable) { GetInstance().m_VerticalSynchronization = enable; }

    friend Application;

private:
    bool m_VerticalSynchronization;
    array<uchar, 3> m_BackgoundColour;

    Graphics();

    Graphics(const Graphics&) = delete;
    Graphics operator=(const Graphics&) = delete;

    bool Initialize();
    void ClearBuffers();
    void SwapBuffers(Window& window);
    void CreateViewport(Window& window);

    void ISetFaceCulling(bool enable);

    static Graphics& GetInstance() { static Graphics sInstance; return sInstance; }
};

#endif
