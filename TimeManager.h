#pragma once
#include <chrono>

class TimeManager {
public:
    TimeManager();

    void Update();

    // CORRIGÉ : plus clair pour le compilateur
    int GetHours(bool use24h = true) const { 
        if (use24h) {
            return hours;
        } else {
            // Format 12h
            int h12 = hours % 12;
            return (h12 == 0) ? 12 : h12;
        }
    }

    int GetMinutes() const {
        return minutes; 
    }

    int GetSeconds() const {
        return seconds; 
    }

    // Optionnel : méthode pour savoir si c'est AM ou PM
    bool IsPM() const {
        return hours >= 12;
    }

private:
    int hours;
    int minutes;
    int seconds;
};