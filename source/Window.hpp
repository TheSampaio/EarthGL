#pragma once

class Application;

class Window
{
public:
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

    bool Create();
    bool IClose();

    static Window& GetInstance() { static Window sInstance; return sInstance; }
};
