#ifndef _EARTHGL_INPUT_
#define _EARTHGL_INPUT_

class Application;
class Window;

class Input
{
public:
    friend Application;

private:
    Input() {};

    Input(const Input&) = delete;
    Input operator=(const Input&) = delete;

    void PollEvents();

    static Input& GetInstance() { static Input sInstance; return sInstance; }
};

#endif
