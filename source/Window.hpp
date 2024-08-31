#ifndef _EARTHGL_WINDOW_
#define _EARTHGL_WINDOW_

class Application;

class Window
{
public:
    static GLFWwindow* GetId() { return GetInstance().mpId; }

    static string GetTitle() { return GetInstance().mTitle; }

    static array<ushort, 2> GetSize() { return GetInstance().mSize; }

    static void SetSize(const ushort width, const ushort height) { GetInstance().mSize = { width, height }; }

    static void SetTitle(const string& title) { GetInstance().mTitle = title; }

    static bool Close() { return GetInstance().IClose(); }

    friend Application;

private:
    GLFWwindow* mpId;

    string mTitle;
    array<ushort, 2> mSize;
    
    Window();
    ~Window();

    Window(const Window&) = delete;
    Window operator=(const Window&) = delete;

    bool Create();
    bool IClose();

    static Window& GetInstance() { static Window sInstance; return sInstance; }
};

#endif
