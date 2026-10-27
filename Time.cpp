#include "Time.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

std::string GetFormattedTime()
{
    auto now = std::chrono::system_clock::now();

    std::time_t time = std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};

    localtime_s(&localTime, &time);

    std::ostringstream result;

    result << std::put_time(&localTime, "%H:%M:%S");

    return result.str();
}
