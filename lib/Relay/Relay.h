#ifndef __RELAY_H__
#define __RELAY_H__

#include <Arduino.h>

class Relay {
private:
    int pin;
    bool status = false;
    unsigned int count = 0;
    void ToggleCondition(bool status);

public:
    Relay(int pin);
    bool GetCondition();
    void SetCondition(bool status);
    unsigned int GetCount();
};

#endif
