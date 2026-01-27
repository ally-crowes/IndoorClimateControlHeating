#ifndef __SENSORTYPENTC_H__
#define __SENSORTYPENTC_H__

#include "tSensor.h"

class SensorTypeNTC : public ITSensor
{
private:
    unsigned int _pin = 0;

    // R (Ом) - сопротивление постоянное верхнего плеча делителя (Ом)
    const float resistUp = 10000.;
    // Ro (Ом) - сопротивление термистора при temperatureBase (градусах по Цельсию)
    const float resistBase = 10000.;
    // температура (градусов Цельсия), при которой измерено сопротивление термистора resistBase
    const float temperatureBase = 25.;

public:
    explicit SensorTypeNTC(unsigned int pin);

    float getTemperature() const;

private:
    const float adc2tempC_NTCschA(const int adcValue, const int bConstant) const;
};

#endif