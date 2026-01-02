#ifndef __SENSORTYPENTC_H__
#define __SENSORTYPENTC_H__

class SensorTypeNTC
{
private:
    int pin = 0;
    float min = 0;
    float max = 0;

public:
    SensorTypeNTC(float min, float max, int pin);
    const float getMin() const;
    void setMin(float min);
    const float getMax() const;
    void setMax(float max);

    float getTemperature();
};

#endif