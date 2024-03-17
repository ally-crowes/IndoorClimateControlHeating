#include "Relay.h"

Relay::Relay(int pin) : pin {pin}
{
    pinMode(pin, OUTPUT);
    ToggleCondition(false);
}

bool Relay::GetCondition()
{
    return this->status;
}

void Relay::SetCondition(bool status)
{
    this->status = status;
    ToggleCondition(status);
}

void Relay::ToggleCondition(bool status)
{
    if (status)
    {
        digitalWrite(pin, HIGH);
        count++;
    }
    else
        digitalWrite(pin, LOW);
}

unsigned int Relay::GetCount()
{
    return count;
}
