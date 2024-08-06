#pragma once

class Debug
{
public:
    static void Console(const string& text) { GetInstance().IConsole(text); }

private:
    Debug() {};

    void IConsole(const string& text, bool bBreakLine = true);

    static Debug& GetInstance() { static Debug sInstance; return sInstance; }
};
