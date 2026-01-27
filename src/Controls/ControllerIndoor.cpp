#include <Arduino.h>

#include "controllerIndoor.h"

ControllerIndoor::ControllerIndoor(ITSensor* sensorInternal, ControllerEngine* controlEngine)
    : sensorInternal(sensorInternal), controlEngine(controlEngine)
{
}

ControllerIndoor::ControllerIndoor(ITSensor* sensorInternal, ControllerEngine* controlEngine, RelayDevice* pump)
    : ControllerIndoor(sensorInternal, controlEngine)
{
    this->pump = pump;
}

ControllerIndoor::ControllerIndoor(ITSensor* sensorInternal, ControllerEngine* controlEngine, RelayDevice* pump, float min, float max)
    : ControllerIndoor(sensorInternal, controlEngine, pump)
{
    this->min = min;
    this->max = max;
}
    
ControllerIndoor::ControllerIndoor(ITSensor* sensorInternal, ControllerEngine* controlEngine, RelayDevice* pump, float min, float max, float* eeprom_minTemperature, float* eeprom_maxTemperature)
    : ControllerIndoor(sensorInternal, controlEngine, pump, min, max)
{
    this->eeprom_minTemperature = eeprom_minTemperature;
    this->eeprom_maxTemperature = eeprom_maxTemperature;
}



float ControllerIndoor::getTemperature()
{
    return sensorInternal->getTemperature();
}

void ControllerIndoor::update()
{
    static unsigned long timerPump = 0;

    float temperatureCurent = getTemperature();

    switch (controlEngine->getAction())
    {
    case ModeAction::Heat:
        if (flagDriversWait == false)
        {
            if (flagPumpWait)
            {
                if ((millis() - timerPump) > delayPumpOn)
                {
                    flagPumpWait = false;
                }
            }
            else
            {
                if (temperatureCurent <= this->getMaxTemperature()) // Const - Supervise Engine
                {
                    controlEngine->update();
                }
                else // Begin - Wait Engine
                {
                    controlEngine->wait();
                    flagDriversWait = true;

                    flagPumpWait = true;
                    timerPump = millis();
                }
            }
        }
        else
        {
            if (flagPumpWait)
            {
                if ((millis() - timerPump) > delayPumpOff)
                {
                    flagPumpWait = false;
                    setConditionPump(false);
                }
            }

            if (temperatureCurent <= this->getMinTemperature())  // End - Wait Engine
            {
                flagDriversWait = false;

                setConditionPump(true);
                timerPump = millis();
                
                flagPumpWait = true;
            }
        }
        break;

    case ModeAction::Coll:
        if (temperatureCurent >= this->getMinTemperature())
            controlEngine->update();
        else
            controlEngine->wait();
        break;

    case ModeAction::RangeMatch:
        if (temperatureCurent <= this->getMaxTemperature() && temperatureCurent >= this->getMinTemperature())
            controlEngine->update();
        else
            controlEngine->wait();
        break;
    
    default:
        break;
    }
}

bool ControllerIndoor::getCondition()
{
    return !flagDriversWait;
}

void ControllerIndoor::setConditionPump(bool value)
{
    this->pump->setCondition(value);
}

bool ControllerIndoor::getConditionPump()
{
    return this->pump->getCondition();
}

float ControllerIndoor::getMinTemperature()
{
    return this->min;
}

void ControllerIndoor::setMinTemperature(float min)
{
    this->min = min >= this->max ? this->max - 0.1f : min;
}

float ControllerIndoor::getMaxTemperature()
{
    return this->max;
}

void ControllerIndoor::setMaxTemperature(float max)
{
    this->max = max <= this->min ? this->min + 0.1f : max;
}

unsigned long ControllerIndoor::getDelayPumpOff()
{
    return delayPumpOff;
}

void ControllerIndoor::setDelayPumpOff(unsigned long time)
{
    delayPumpOff = time;
}

unsigned long ControllerIndoor::getDelayPumpOn()
{
    return delayPumpOn;
}

void ControllerIndoor::setDelayPumpOn(unsigned long time)
{
    delayPumpOn = time;
}

void ControllerIndoor::changeMinUpTemperature(float step)
{
    setMinTemperature(getMinTemperature() + step);
    //eeprom_write_float(30, GetMinTemperature());
    eeprom_write_float(eeprom_minTemperature, getMinTemperature());
}

void ControllerIndoor::changeMinDownTemperature(float step)
{
    setMinTemperature(getMinTemperature() - step);
    //eeprom_write_float(30, GetMinTemperature());
    eeprom_write_float(eeprom_minTemperature, getMinTemperature());
}

void ControllerIndoor::changeMaxUpTemperature(float step)
{
    setMaxTemperature(getMaxTemperature() + step);
    // eeprom_write_float(40, GetMaxTemperature());
    eeprom_write_float(eeprom_maxTemperature, getMaxTemperature());
}

void ControllerIndoor::changeMaxDownTemperature(float step)
{
    setMaxTemperature(getMaxTemperature() - step);
    // eeprom_write_float(40, GetMaxTemperature());
    eeprom_write_float(eeprom_maxTemperature, getMaxTemperature());
}
