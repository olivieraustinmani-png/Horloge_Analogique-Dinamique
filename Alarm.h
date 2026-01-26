#ifndef ALARM_H
#define ALARM_H

struct Alarm {
    int hour = 0;
    int minute = 0;
    bool active = false;
    bool ringing = false;
    
    // Options de répétition (Snooze)
    bool snoozeActive = false;
    int snoozeHour = 0;
    int snoozeMinute = 0;
    int snoozeCount = 0; // Pour savoir combien de fois on a répété
};

#endif