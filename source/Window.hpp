#ifndef EARTHGL_WINDOW
#define EARTHGL_WINDOW

class Application;

class Window
{
public:
    static GLFWwindow* GetId() { return GetInstance().m_pId; }

    static void SetSize(const ushort width, const ushort height) { GetInstance().m_Size = { width, height }; }

    static void SetTitle(const string& title) { GetInstance().m_Title = title; }

    static bool Close() { return GetInstance().IClose(); }

    friend Application;

private:
    GLFWwindow* m_pId;
    array<ushort, 2> m_Size;
    string m_Title;

    Window();
    ~Window();

    Window(const Window&) = delete;
    Window operator=(const Window&) = delete;

    bool Create();
    bool IClose();

    static Window& GetInstance() { static Window sInstance; return sInstance; }
};

#endif
