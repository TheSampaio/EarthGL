#include "PCH.hpp"
#include "Debug.hpp"

Debug::Debug()
{
    spdlog::set_pattern("%^[%T] %v%$");
}

void Debug::IConsole(EDebugMode mode, const std::string& text)
{
    switch (mode)
    {
        case Error:
            spdlog::error(text);
            break;

        case Information:
            spdlog::info(text);
            break;

        case Warning:
            spdlog::warn(text);
            break;
    }
}