#ifndef EARTHGL_INPUT
#define EARTHGL_INPUT

class Application;

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
