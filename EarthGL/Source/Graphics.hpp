#ifndef _EARTHGL_GRAPHICS_
#define _EARTHGL_GRAPHICS_

class Graphics
{
public:
    // Gets the class's static reference
    static Graphics& GetInstance() { static Graphics sInstance; return sInstance; }

    // === MAIN methods ===

    bool Init();
    void ClearBuffers(const int red = 0.0f, const int green = 0.0f, const int blue = 0.0f);

protected:
    Graphics();
    ~Graphics();

    // Deletes copy constructor and assigment operator
    Graphics(const Graphics&) = delete;
    Graphics operator=(const Graphics&) = delete;
};

#endif

