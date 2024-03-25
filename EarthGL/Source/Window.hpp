#ifndef _EARTHGL_WINDOW_
#define _EARTHGL_WINDOW_

class Graphics;
struct GLFWwindow;

namespace EarthGL
{
    class Window
    {
    public:
        // Gets the class's static reference
        static Window& GetInstance() { static Window sInstance; return sInstance; }

        // === Attributes ===

        GLFWwindow* id;
        const int size[2] = { 800, 600 };

        // === MAIN methods ===

        bool Create(Graphics& graphics);
        bool Close();

        void PollEvents();
        void SwapBuffers();

    protected:
        Window();
        ~Window();

        // Deletes copy constructor and assigment operator
        Window(const Window&) = delete;
        Window operator=(const Window&) = delete;
    };
}

#endif
