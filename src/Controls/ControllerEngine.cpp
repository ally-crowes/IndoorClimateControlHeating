#include "ControllerEngine.h"

ControllerEngine::ControllerEngine(SensorTypeNTC* sensor, Relay* relayA, Relay* relayB, ModeAction action, ModeSwitchingDevice switching)
{
    this->sensorEngine = sensor;
    this->relayA = relayA;
    this->relayB = relayB;
    this->action = action;
    this->switching = switching;
}

ControllerEngine::ControllerEngine(SensorTypeNTC* sensor, Relay* relayA, Relay* relayB)
    : ControllerEngine(sensor, relayA, relayB, ModeAction::Heat, ModeSwitchingDevice::TicTac)
{}

ControllerEngine::ControllerEngine(SensorTypeNTC* sensor, Relay* relay, ModeAction action) 
    : ControllerEngine(sensor, relay, new Relay(255), ModeAction::Heat, ModeSwitchingDevice::FirstOnly)
{}

ControllerEngine::ControllerEngine(SensorTypeNTC* sensor, Relay* relayA, Relay* relayB, ModeAction action, ModeSwitchingDevice switching, float* eeprom_minTemperature, float* eeprom_maxTemperature)
    : ControllerEngine(sensor, relayA, relayB, action, switching)
{
    this->eeprom_minTemperature = eeprom_minTemperature;
    this->eeprom_maxTemperature = eeprom_maxTemperature;
}

ModeAction ControllerEngine::GetAction() const
{
    return this->action;
}

void ControllerEngine::SetAction(ModeAction action)
{
    this->action = action;
}

ModeSwitchingDevice ControllerEngine::GetSwitchingDevice() const
{
    return this->switching;
}

void ControllerEngine::SetSwitchingDevice(ModeSwitchingDevice switching)
{
    this->switching = switching;
}


float ControllerEngine::GetTemperature()
{
    return sensorEngine->GetTemperature();
}

void ControllerEngine::Update()
{
    float temperatureCurent = GetTemperature();

    switch (action)
    {
    case ModeAction::Heat:
        if (flagDriversWait == false)
        {
            if (temperatureCurent <= sensorEngine->GetMax())
                RelaysOn();
            else
                RelaysOff();
        }
        else if (temperatureCurent <= sensorEngine->GetMin())
        {
            flagDriversWait = false;
        }
        break;

    case ModeAction::Coll:
        if (flagDriversWait == false)
        {
            if (temperatureCurent >= sensorEngine->GetMin())
                RelaysOn();
            else
                RelaysOff();
        }
        else if (temperatureCurent >= sensorEngine->GetMax())
        {
            flagDriversWait = false;
        }
        break;

    case ModeAction::RangeMatch:
        if (temperatureCurent <= sensorEngine->GetMax() && temperatureCurent >= sensorEngine->GetMin())
            RelaysOn();
        else
            RelaysOff();
        break;

    default:
        break;
    }
}

void ControllerEngine::RelaysOn()
{
    if (flagRelay == false)
    {
        flagRelay = true;

        switch (switching)
        {
        case ModeSwitchingDevice::FirstOnly:
            relayA->SetCondition(true);
            relayB->SetCondition(false);
            break;

        case ModeSwitchingDevice::SecondOnly:
            relayA->SetCondition(false);
            relayB->SetCondition(true);
            break;

        case ModeSwitchingDevice::TicTac:
            if (count % 2)
            {
                relayA->SetCondition(true);
                relayB->SetCondition(false);
            }
            else
            {
                relayA->SetCondition(false);
                relayB->SetCondition(true);
            }
            count++;
            break;

        case ModeSwitchingDevice::Parallel:
            relayA->SetCondition(true);
            relayB->SetCondition(true);
            break;

        default:
            break;
        }
    }
}

void ControllerEngine::RelaysOff()
{
    if (flagDriversWait == false)
    {
        relayA->SetCondition(false);
        relayB->SetCondition(false);

        flagRelay = false;
        flagDriversWait = true;
    }
}

unsigned int ControllerEngine::GetCount()
{
    return count;
}

void ControllerEngine::Wait()
{
    RelaysOff();
}

bool ControllerEngine::GetConditionRelay(int n)
{
    bool result;

    if (n > 1)
    {
        result = relayB->GetCondition();
    }
    else
    {
        result = relayA->GetCondition();
    }

    return result;
}

bool ControllerEngine::GetCondition()
{
    // return (!relayA->GetCondition() && !relayB->GetCondition()) ? false : true;
    return !flagDriversWait;
}

float ControllerEngine::GetMinTemperature()
{
    return sensorEngine->GetMin();
}

void ControllerEngine::SetMinTemperature(float min)
{
    sensorEngine->SetMin(min);
}

float ControllerEngine::GetMaxTemperature()
{
    return sensorEngine->GetMax();
}

void ControllerEngine::SetMaxTemperature(float max)
{
    sensorEngine->SetMax(max);
}

void ControllerEngine::ChangeMinUpTemperature(float step)
{
    SetMinTemperature(GetMinTemperature() + step);
    //eeprom_write_float(10, GetMinTemperature());
    eeprom_write_float(eeprom_minTemperature, GetMinTemperature());
}

void ControllerEngine::ChangeMinDownTemperature(float step)
{
    SetMinTemperature(GetMinTemperature() - step);
    //eeprom_write_float(10, GetMinTemperature());
    eeprom_write_float(eeprom_minTemperature, GetMinTemperature());
}

void ControllerEngine::ChangeMaxUpTemperature(float step)
{
    SetMaxTemperature(GetMaxTemperature() + step);
    // eeprom_write_float(20, GetMaxTemperature());
    eeprom_write_float(eeprom_maxTemperature, GetMaxTemperature());
}

void ControllerEngine::ChangeMaxDownTemperature(float step)
{
    SetMaxTemperature(GetMaxTemperature() - step);
    // eeprom_write_float(20, GetMaxTemperature());
    eeprom_write_float(eeprom_maxTemperature, GetMaxTemperature());
}
