#ifndef _TSENSOR_BASE_H_
#define _TSENSOR_BASE_H_

class ITSensor
{
public:
    virtual float getTemperature() const = 0;
};

#endif