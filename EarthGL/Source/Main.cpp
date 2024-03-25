#include "Debug.hpp"
#include "Graphics.hpp"
#include "Window.hpp"

using namespace EarthGL;

int main()
{
    // === Start ===
    Debug& debug = Debug::GetInstance();
    Graphics& graphics = Graphics::GetInstance();
    Window& window = Window::GetInstance();

    window.Create(graphics);

    // Main loop
    while (!window.Close())
    {
        // === Update ===
        {
            window.PollEvents();
        }

        graphics.ClearBuffers(110, 80, 166);

        // === Draw ===
        {
            window.SwapBuffers();
        }
    }
    
    // === End ===
}
