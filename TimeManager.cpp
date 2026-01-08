#include "TimeManager.h"
#include <ctime>

TimeManager::TimeManager()
    : hours(0), minutes(0), seconds(0)
{
    Update(); // Initialisation dès le départ
}

void TimeManager::Update()
{
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    hours = localTime->tm_hour;
    minutes = localTime->tm_min;
    seconds = localTime->tm_sec;
}