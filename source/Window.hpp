#ifndef _EARTHGL_WINDOW_
#define _EARTHGL_WINDOW_

class Application;

class Window
{
public:
    static GLFWwindow* GetId() { return GetInstance().m_pId; }

    static string GetTitle() { return GetInstance().m_Title; }

    static array<ushort, 2> GetSize() { return GetInstance().m_Size; }

    static void SetSize(const ushort width, const ushort height) { GetInstance().m_Size = { width, height }; }

    static void SetTitle(const string& title) { GetInstance().m_Title = title; }

    static bool Close() { return GetInstance().IClose(); }

    friend Application;

private:
    GLFWwindow* m_pId;
    string m_Title;
    array<ushort, 2> m_Size;

    Window();
    ~Window();

    Window(const Window&) = delete;
    Window operator=(const Window&) = delete;

    bool Create();
    bool IClose();

    static Window& GetInstance() { static Window sInstance; return sInstance; }
};

#endif
