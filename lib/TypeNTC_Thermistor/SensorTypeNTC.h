#ifndef __SENSORTYPENTC_H__
#define __SENSORTYPENTC_H__
#include <Arduino.h>
#include "b3380.h"
#include "CalcNTCTemperature.h"

class SensorTypeNTC
{
private:
    int pin = 0;
    float min = 0;
    float max = 0;

public:
    SensorTypeNTC(float min, float max, int pin);
    float GetMin() const;
    void SetMin(float min);
    float GetMax() const;
    void SetMax(float max);

    float GetTemperature();
};

#endif