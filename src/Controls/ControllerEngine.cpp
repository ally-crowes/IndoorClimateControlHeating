#include <Arduino.h>

#include "controllerEngine.h"

// ControllerEngine::ControllerEngine(SensorTypeNTC* sensor, Relay* relayA, Relay* relayB, ModeAction action, ModeSwitchingDevice switching)
// ControllerEngine::ControllerEngine(SensorTypeNTC* sensor, Relay* relayA, Relay* relayB, ModeAction action, ModeSwitchingDevice switching, float* eeprom_minTemperature, float* eeprom_maxTemperature)
// {
//     this->action = action;
//     this->switching = switching;
// }

// ControllerEngine::ControllerEngine(SensorTypeNTC* sensor, Relay* relayA, Relay* relayB, ModeAction action, ModeSwitchingDevice switching, float* eeprom_minTemperature, float* eeprom_maxTemperature)
//     : ControllerEngine(sensor, relayA, relayB, action, switching)
// {
//     this->eeprom_minTemperature = eeprom_minTemperature;
//     this->eeprom_maxTemperature = eeprom_maxTemperature;
// }

ControllerEngine::ControllerEngine(SensorTypeNTC* sensor, RelayDevice* relayA, RelayDevice* relayB, float* eeprom_minTemperature, float* eeprom_maxTemperature)
{
    this->sensorEngine = sensor;
    this->relayA = relayA;
    this->relayB = relayB;
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
            if (temperatureCurent <= sensorEngine->getMax())
                relaysOn();
            else
                relaysOff();
        }
        else if (temperatureCurent <= sensorEngine->getMin())
        {
            flagDriversWait = false;
        }
        break;

    case ModeAction::Coll:
        if (flagDriversWait == false)
        {
            if (temperatureCurent >= sensorEngine->getMin())
                relaysOn();
            else
                relaysOff();
        }
        else if (temperatureCurent >= sensorEngine->getMax())
        {
            flagDriversWait = false;
        }
        break;

    case ModeAction::RangeMatch:
        if (temperatureCurent <= sensorEngine->getMax() && temperatureCurent >= sensorEngine->getMin())
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
    return sensorEngine->getMin();
}

void ControllerEngine::setMinTemperature(float min)
{
    sensorEngine->setMin(min);
}

float ControllerEngine::getMaxTemperature()
{
    return sensorEngine->getMax();
}

void ControllerEngine::setMaxTemperature(float max)
{
    sensorEngine->setMax(max);
}

void ControllerEngine::changeMinUpTemperature(float step)
{
    setMinTemperature(getMinTemperature() + step);
    //eeprom_write_float(10, GetMinTemperature());
    eeprom_write_float(eeprom_minTemperature, getMinTemperature());
}

void ControllerEngine::changeMinDownTemperature(float step)
{
    setMinTemperature(getMinTemperature() - step);
    //eeprom_write_float(10, GetMinTemperature());
    eeprom_write_float(eeprom_minTemperature, getMinTemperature());
}

void ControllerEngine::changeMaxUpTemperature(float step)
{
    setMaxTemperature(getMaxTemperature() + step);
    // eeprom_write_float(20, GetMaxTemperature());
    eeprom_write_float(eeprom_maxTemperature, getMaxTemperature());
}

void ControllerEngine::changeMaxDownTemperature(float step)
{
    setMaxTemperature(getMaxTemperature() - step);
    // eeprom_write_float(20, GetMaxTemperature());
    eeprom_write_float(eeprom_maxTemperature, getMaxTemperature());
}
