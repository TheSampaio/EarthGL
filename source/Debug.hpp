#ifndef _EARTHGL_DEBUG_
#define _EARTHGL_DEBUG_

enum EDebugMode
{
    Error = 0,
    Information,
    Warning,
};

class Debug
{
public:
    static void Console(EDebugMode mode, const string& text) { GetInstance().IConsole(mode, text); }

private:
    Debug();

    Debug(const Debug&) = delete;
    Debug operator=(const Debug&) = delete;

    void IConsole(EDebugMode mode, const string& text);

    static Debug& GetInstance() { static Debug sInstance; return sInstance; }
};

#endif
