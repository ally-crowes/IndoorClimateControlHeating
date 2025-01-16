#include "ControllerIndoor.h"

ControllerIndoor::ControllerIndoor(SensorTypeNTC* sensorInternal, ControllerEngine* controlEngine)
{
    this->sensorInternal = sensorInternal;
    this->controlEngine = controlEngine;
}

ControllerIndoor::ControllerIndoor(SensorTypeNTC* sensorInternal, ControllerEngine* controlEngine, Relay* pump)
    : sensorInternal(sensorInternal), controlEngine(controlEngine)
{
    this->pump = pump;
}    

float ControllerIndoor::GetTemperature()
{
    return sensorInternal->GetTemperature();
}

void ControllerIndoor::Update()
{
    static unsigned long timerPump = 0;

    float temperatureCurent = GetTemperature();

    switch (controlEngine->GetAction())
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
                if (temperatureCurent <= sensorInternal->GetMax()) // Const - Supervise Engine
                {
                    controlEngine->Update();
                }
                else // Begin - Wait Engine
                {
                    controlEngine->Wait();
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
                    SetConditionPump(false);
                }
            }

            if (temperatureCurent <= sensorInternal->GetMin())  // End - Wait Engine
            {
                flagDriversWait = false;

                SetConditionPump(true);
                timerPump = millis();
                
                flagPumpWait = true;
            }
        }
        break;

    case ModeAction::Coll:
        if (temperatureCurent >= sensorInternal->GetMin())
            controlEngine->Update();
        else
            controlEngine->Wait();
        break;

    case ModeAction::RangeMatch:
        if (temperatureCurent <= sensorInternal->GetMax() && temperatureCurent >= sensorInternal->GetMin())
            controlEngine->Update();
        else
            controlEngine->Wait();
        break;
    
    default:
        break;
    }
}

bool ControllerIndoor::GetCondition()
{
    return !flagDriversWait;
}

void ControllerIndoor::SetConditionPump(bool value)
{
    this->pump->SetCondition(value);
}

bool ControllerIndoor::GetConditionPump()
{
    return this->pump->GetCondition();
}

float ControllerIndoor::GetMinTemperature()
{
    return sensorInternal->GetMin();
}

void ControllerIndoor::SetMinTemperature(float min)
{
    sensorInternal->SetMin(min);
}

float ControllerIndoor::GetMaxTemperature()
{
    return sensorInternal->GetMax();
}

void ControllerIndoor::SetMaxTemperature(float max)
{
    sensorInternal->SetMax(max);
}

unsigned long ControllerIndoor::GetDelayPumpOff()
{
    return delayPumpOff;
}

void ControllerIndoor::SetDelayPumpOff(unsigned long time)
{
    delayPumpOff = time;
}

unsigned long ControllerIndoor::GetDelayPumpOn()
{
    return delayPumpOn;
}

void ControllerIndoor::SetDelayPumpOn(unsigned long time)
{
    delayPumpOn = time;
}
