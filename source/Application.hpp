#pragma once

class Game;

class Application
{
public:
    static void Run(Game* pGame) { return GetInstance().IRun(*pGame); }

private:
    Game* m_pGame;

    Application();
    ~Application();

    void IRun(Game& game);

    static Application& GetInstance() { static Application sInstance; return sInstance; }
};
