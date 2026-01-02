#include <Arduino.h>
//#include "b3380.h"
#include "adc2tempC_NTCschA.h"

#include "tSensorTypeNTC.h"

SensorTypeNTC::SensorTypeNTC(float min, float max, int pin)
{
    this->min = min;
    this->max = max;
    this->pin = pin;
    pinMode(pin, INPUT);
}

const float SensorTypeNTC::getMin() const
{
    return this->min;
}

void SensorTypeNTC::setMin(float min)
{
    this->min = min >= this->max ? this->max - 0.1f : min;
}

const float SensorTypeNTC::getMax() const
{
    return this->max;
}

void SensorTypeNTC::setMax(float max)
{
    this->max = max <= this->min ? this->min + 0.1f : max;
}

float SensorTypeNTC::getTemperature()
{
    // Average value
    int average = 0;
    for (byte i = 0; i < 10; i++)
        average += analogRead(pin);
    average /= 10;

    // return (CalculateNTC(average) / 10.0);
    // return adc2tempC_NTCschA(average, 3380);
    return adc2tempC_NTCschA(average, 3900);
}
