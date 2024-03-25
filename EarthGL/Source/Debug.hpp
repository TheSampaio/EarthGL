#ifndef _EARTHGL_DEBUG_
#define _EARTHGL_DEBUG_

namespace EarthGL
{
    class Debug
    {
    public:
        // Gets the class's static reference
        static Debug& GetInstance() { static Debug sInstance; return sInstance; }

        // === MAIN methods ===

        void Pause();
        void Print(const char* text, bool bLine = true);

    protected:
        Debug() {};

        // Deletes copy constructor and assigment operator
        Debug(const Debug&) = delete;
        Debug operator=(const Debug&) = delete;
    };
}

#endif
