#ifndef _EARTHGL_TIMER_
#define _EARTHGL_TIMER_

class Timer
{
public:
    Timer();

    // Gets the elapsed time since the timer's start
    float GetElapsedTime();

    // Starts the timer
    void Start();

    // Stops the timer
    void Stop();

    // Resets the timer
    float Reset();

private:
    std::chrono::steady_clock::time_point m_Start;
    std::chrono::steady_clock::time_point m_End;

    bool m_bPaused;
};

#endif
