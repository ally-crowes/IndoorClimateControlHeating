#ifndef __CONTROL_CLIMATE_H__
#define __CONTROL_CLIMATE_H__

#include "ControllerEngine.h"

class ControllerIndoor
{
private:
    SensorTypeNTC* sensorInternal;
    ControllerEngine* controlEngine;
    RelayDevice* pump;
    bool flagDriversWait = false;
    bool flagPumpWait = false;
    unsigned long delayPumpOff = 10000;   // Затримка перед вимкненям насосу після повного нагріву кімнат (10 сек)
    unsigned long delayPumpOn = 3000;   // Затримка увимкненям нагрівачів після увімкниням насосу (3 сек)
    
protected:
    float* eeprom_minTemperature;
    float* eeprom_maxTemperature;

public:
    ControllerIndoor(SensorTypeNTC* sensorInternal, ControllerEngine* controlEngine);
    ControllerIndoor(SensorTypeNTC* sensorInternal, ControllerEngine* controlEngine, RelayDevice* pump);
    ControllerIndoor(SensorTypeNTC* sensorInternal, ControllerEngine* controlEngine, RelayDevice* pump, float* eeprom_minTemperature, float* eeprom_maxTemperature);

    float getTemperature();
    void update();
    bool getCondition();
    float getMinTemperature();
    void setMinTemperature(float min);
    float getMaxTemperature();
    void setMaxTemperature(float max);
    bool getConditionPump();
    void changeMinUpTemperature(float step = 0.1);
    void changeMinDownTemperature(float step = 0.1);
    void changeMaxUpTemperature(float step = 0.1);
    void changeMaxDownTemperature(float step = 0.1);
    void setConditionPump(bool value);
    unsigned long getDelayPumpOff();
    void setDelayPumpOff(unsigned long time);
    unsigned long getDelayPumpOn();
    void setDelayPumpOn(unsigned long time);
};

#endif