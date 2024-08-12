#ifndef EARTHGL_DEBUG
#define EARTHGL_DEBUG

class Debug
{
public:
    static void Console(const string& text) { GetInstance().IConsole(text); }

private:
    Debug() {};

    Debug(const Debug&) = delete;
    Debug operator=(const Debug&) = delete;

    void IConsole(const string& text, bool bBreakLine = true);

    static Debug& GetInstance() { static Debug sInstance; return sInstance; }
};

#endif
