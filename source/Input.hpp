#pragma once

class Application;

class Input
{
public:
    friend Application;

private:
    void PollEvents();

    static Input& GetInstance() { static Input sInstance; return sInstance; }
};
