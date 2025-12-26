#pragma once
#include <chrono>

class TimeManager {
public:
    TimeManager();

    void Update(); // Met à jour l'heure actuelle

    int GetHours() const { return hours; }
    int GetMinutes() const { return minutes; }
    int GetSeconds() const { return seconds; }

private:
    int hours;
    int minutes;
    int seconds;
};
