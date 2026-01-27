#ifndef __CONTROL_CLIMATE_H__
#define __CONTROL_CLIMATE_H__

#include "ControllerEngine.h"

class ControllerIndoor
{
private:
    ITSensor* sensorInternal = NULL;
    ControllerEngine* controlEngine = NULL;
    RelayDevice* pump = NULL;
    bool flagDriversWait = false;
    bool flagPumpWait = false;
    unsigned long delayPumpOff = 10000;   // Затримка перед вимкненям насосу після повного нагріву кімнат (10 сек)
    unsigned long delayPumpOn = 3000;   // Затримка увимкненям нагрівачів після увімкниням насосу (3 сек)
    
    float min = 0;
    float max = 0;
    
protected:
    float* eeprom_minTemperature = NULL;
    float* eeprom_maxTemperature = NULL;

public:
    ControllerIndoor(ITSensor* sensorInternal, ControllerEngine* controlEngine);
    ControllerIndoor(ITSensor* sensorInternal, ControllerEngine* controlEngine, RelayDevice* pump);
    ControllerIndoor(ITSensor* sensorInternal, ControllerEngine* controlEngine, RelayDevice* pump, float min, float max);
    ControllerIndoor(ITSensor* sensorInternal, ControllerEngine* controlEngine, RelayDevice* pump, float min, float max, float* eeprom_minTemperature, float* eeprom_maxTemperature);

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