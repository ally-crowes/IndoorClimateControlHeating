#include <Arduino.h>

#include "relayDevice.h"

RelayDevice::RelayDevice(int pin) : pin {pin}
{
    pinMode(pin, OUTPUT);
    ToggleCondition(false);
}

bool RelayDevice::getCondition()
{
    return this->status;
}

void RelayDevice::setCondition(bool status)
{
    this->status = status;
    ToggleCondition(status);
}

void RelayDevice::ToggleCondition(bool status)
{
    if (status)
    {
        digitalWrite(pin, HIGH);
        count++;
    }
    else
        digitalWrite(pin, LOW);
}

unsigned int RelayDevice::getCount()
{
    return count;
}
