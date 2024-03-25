#include "Debug.hpp"

#include <iostream>

void EarthGL::Debug::Pause()
{
    (std::getchar() != '\0') ? '\0' : '\0';
}

void EarthGL::Debug::Print(const char* text, bool bLine)
{
    (bLine) ? std::cout << text << std::endl : std::cout << text;
}
