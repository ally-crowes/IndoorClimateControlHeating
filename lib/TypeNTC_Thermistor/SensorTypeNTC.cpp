#include "SensorTypeNTC.h"

SensorTypeNTC::SensorTypeNTC(float min, float max, int pin)
{
    this->min = min;
    this->max = max;
    this->pin = pin;
    pinMode(pin, INPUT);
}

float SensorTypeNTC::GetMin() const
{
    return this->min;
}

void SensorTypeNTC::SetMin(float min)
{
    this->min = min >= this->max ? this->max - 0.1f : min;
}

float SensorTypeNTC::GetMax() const
{
    return this->max;
}

void SensorTypeNTC::SetMax(float max)
{
    this->max = max <= this->min ? this->min + 0.1f : max;
}

float SensorTypeNTC::GetTemperature()
{
    // Average value
    int average = 0;
    for (byte i = 0; i < 10; i++)
        average += analogRead(pin);
    average /= 10;

    //return (calculateNTC(average) / 10.0);
    // return getCalcNTCTemperature(average, 3380);
    return getCalcNTCTemperature(average, 3900);
}
