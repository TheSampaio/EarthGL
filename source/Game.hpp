#ifndef EARTHGL_GAME
#define EARTHGL_GAME

class Game
{
public:
    virtual void OnStart() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnDraw() = 0;
    virtual void OnFinalize() = 0;
};

#endif
