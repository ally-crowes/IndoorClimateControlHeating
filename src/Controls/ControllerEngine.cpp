#include <Arduino.h>

#include "controllerEngine.h"

ControllerEngine::ControllerEngine(ITSensor* sensor, RelayDevice* relayA, RelayDevice* relayB, float min, float max)
{
    this->sensorEngine = sensor;
    this->relayA = relayA;
    this->relayB = relayB;
        
    this->min = min;
    this->max = max;
}

ControllerEngine::ControllerEngine(ITSensor* sensor, RelayDevice* relayA, RelayDevice* relayB, float min, float max, float* eeprom_minTemperature, float* eeprom_maxTemperature)
    : ControllerEngine (sensor, relayA, relayB, min, max)
{
    this->eeprom_minTemperature = eeprom_minTemperature;
    this->eeprom_maxTemperature = eeprom_maxTemperature;
}

/* TODO
ControllerEngine::ControllerEngine(SensorTypeNTC* sensor, Relay* relayA, Relay* relayB)
//: ControllerEngine(sensor, relayA, relayB, ModeAction::Heat, ModeSwitchingDevice::TicTac)
{}

ControllerEngine::ControllerEngine(SensorTypeNTC* sensor, Relay* relay, ModeAction action) 
//: ControllerEngine(sensor, relay, new Relay(255), ModeAction::Heat, ModeSwitchingDevice::FirstOnly)
{}
*/


void ControllerEngine::relaysOn()
{
    if (flagRelay == false)
    {
        flagRelay = true;

        switch (switching)
        {
        case ModeSwitchingDevice::FirstOnly:
            relayA->setCondition(true);
            relayB->setCondition(false);
            break;

        case ModeSwitchingDevice::SecondOnly:
            relayA->setCondition(false);
            relayB->setCondition(true);
            break;

        case ModeSwitchingDevice::TicTac:
            if (count % 2)
            {
                relayA->setCondition(true);
                relayB->setCondition(false);
            }
            else
            {
                relayA->setCondition(false);
                relayB->setCondition(true);
            }
            count++;
            break;

        case ModeSwitchingDevice::Parallel:
            relayA->setCondition(true);
            relayB->setCondition(true);
            break;

        default:
            break;
        }
    }
}

void ControllerEngine::relaysOff()
{
    if (flagDriversWait == false)
    {
        relayA->setCondition(false);
        relayB->setCondition(false);

        flagRelay = false;
        flagDriversWait = true;
    }
}

ModeAction ControllerEngine::getAction() const
{
    return this->action;
}

void ControllerEngine::setAction(ModeAction action)
{
    this->action = action;
}

ModeSwitchingDevice ControllerEngine::getSwitchingDevice() const
{
    return this->switching;
}

void ControllerEngine::setSwitchingDevice(ModeSwitchingDevice switching)
{
    this->switching = switching;
}

float ControllerEngine::getTemperature()
{
    return sensorEngine->getTemperature();
}

void ControllerEngine::update()
{
    float temperatureCurent = getTemperature();

    switch (action)
    {
    case ModeAction::Heat:
        if (flagDriversWait == false)
        {
            if (temperatureCurent <= this->getMaxTemperature())
                relaysOn();
            else
                relaysOff();
        }
        else if (temperatureCurent <= this->getMinTemperature())
        {
            flagDriversWait = false;
        }
        break;

    case ModeAction::Coll:
        if (flagDriversWait == false)
        {
            if (temperatureCurent >= this->getMinTemperature())
                relaysOn();
            else
                relaysOff();
        }
        else if (temperatureCurent >= this->getMaxTemperature())
        {
            flagDriversWait = false;
        }
        break;

    case ModeAction::RangeMatch:
        if (temperatureCurent <= this->getMaxTemperature() && temperatureCurent >= this->getMinTemperature())
            relaysOn();
        else
            relaysOff();
        break;

    default:
        break;
    }
}

unsigned int ControllerEngine::getCount()
{
    return count;
}

void ControllerEngine::wait()
{
    relaysOff();
}

bool ControllerEngine::getConditionRelay(int n)
{
    bool result;

    if (n > 1)
    {
        result = relayB->getCondition();
    }
    else
    {
        result = relayA->getCondition();
    }

    return result;
}

bool ControllerEngine::getCondition()
{
    // return (!relayA->GetCondition() && !relayB->GetCondition()) ? false : true;
    return !flagDriversWait;
}

float ControllerEngine::getMinTemperature()
{
    return this->min;
}

void ControllerEngine::setMinTemperature(float min)
{
    this->min = min >= this->max ? this->max - 0.1f : min;
    //eeprom_write_float(10, GetMinTemperature());
    eeprom_write_float(eeprom_minTemperature, this->min);
}

float ControllerEngine::getMaxTemperature()
{
    return this->max;
}

void ControllerEngine::setMaxTemperature(float max)
{
    this->max = max <= this->min ? this->min + 0.1f : max;
    // eeprom_write_float(20, GetMaxTemperature());
    eeprom_write_float(eeprom_maxTemperature, this->max);
}

void ControllerEngine::changeMinUpTemperature(float step)
{
    setMinTemperature(getMinTemperature() + step);
    // //eeprom_write_float(10, GetMinTemperature());
    // eeprom_write_float(eeprom_minTemperature, getMinTemperature());
}

void ControllerEngine::changeMinDownTemperature(float step)
{
    setMinTemperature(getMinTemperature() - step);
    // //eeprom_write_float(10, GetMinTemperature());
    // eeprom_write_float(eeprom_minTemperature, getMinTemperature());
}

void ControllerEngine::changeMaxUpTemperature(float step)
{
    setMaxTemperature(getMaxTemperature() + step);
    // // eeprom_write_float(20, GetMaxTemperature());
    // eeprom_write_float(eeprom_maxTemperature, getMaxTemperature());
}

void ControllerEngine::changeMaxDownTemperature(float step)
{
    setMaxTemperature(getMaxTemperature() - step);
    // // eeprom_write_float(20, GetMaxTemperature());
    // eeprom_write_float(eeprom_maxTemperature, getMaxTemperature());
}
