#include "PCH.hpp"
#include "Debug.hpp"

void Debug::IConsole(const std::string& text, bool bBreakLine)
{
    (bBreakLine) ? std::cout << text.c_str() << std::endl : std::cout << text.c_str();
}