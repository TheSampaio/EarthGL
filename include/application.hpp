#ifndef _EARTHGL_APPLICATION_
#define _EARTHGL_APPLICATION_

class Game;
class Timer;

class Application
{
public:
    static void Run(Game* pGame) { return GetInstance().IRun(*pGame); }

private:
    Game* mpGame;
    Timer* mpTimer;

    Application();
    ~Application();

    Application(const Application&) = delete;
    Application operator=(const Application&) = delete;

    void IRun(Game& game);

    static Application& GetInstance() { static Application sInstance; return sInstance; }
};

#endif
