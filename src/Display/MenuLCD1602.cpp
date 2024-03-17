#include "includes.h"
//#include "MenuLCD1602.h"

MenuLCD1602::MenuLCD1602(LiquidCrystal_I2C* lcd, ControllerEngine* controlEngine, ControllerIndoor* controlIndoor, DS3231* rtc)
{
    this->lcd = lcd;
    this->controlEngine = controlEngine;
    this->controlIndoor = controlIndoor;
    this->rtc = rtc;
}

MenuLCD1602::~MenuLCD1602()
{
}

/*
 * Pattern
 */

void PatternEngineLogo()
{
    
}

void MenuLCD1602::PatternConditionIndoor(int col, int row)
{
    lcd->setCursor(col, row);

    // Climate Temperature
    if (controlIndoor->GetCondition())
    {
        lcd->print(char(1)); // Working
    }
    else
    {
        lcd->print(char(2)); // Await
    }
}

void MenuLCD1602::PatternConditionEngine(int col, int row)
{
    lcd->setCursor(col, row);

    // Climate Condition
    if (!controlIndoor->GetCondition())
    {
        lcd->print(char(7)); // Skip controlEngine
    }
    else
    {
        // Engine Condition
        if (controlEngine->GetCondition())
        {
            lcd->print(char(1)); // Working
        }
        else
        {
            lcd->print(char(2)); // Wait
        }
    }
}

void MenuLCD1602::PatternConditionRelayEngine(int col, int row)
{
    lcd->setCursor(col, row);

    // Engine RelayA
    if (controlEngine->GetConditionRelay(1))
    {
        lcd->print(char(6)); // Working
    }
    else
    {
        //lcd->write(165); // Awaits
        lcd->print(char(7));
    }
    
    // Engine RelayB status
    if (controlEngine->GetConditionRelay(2))
    {
        lcd->print(char(6)); // Working
    }
    else
    {
        //lcd->write(165); // Awaits
        lcd->print(char(7));
    }
}

void MenuLCD1602::PatternConditionRelayIndoor(int col, int row)
{
    lcd->setCursor(col, row);

    // Climate Pump
    if (controlIndoor->GetConditionPump())
    {
        lcd->print(char(6)); // Working
    }
    else
    {
        //lcd->write(165); // Awaits
        lcd->print(char(7));
    }
}

void MenuLCD1602::PatternTemperatureIndoor(int col, int row)
{
    lcd->setCursor(col, row);
    lcd->print("\4="); // "T(Indoor)="
    PatternTemperature(col+2, row, Control::Indoor);
}

void MenuLCD1602::PatternTemperatureEngine(int col, int row)
{
    lcd->setCursor(col, row);
    lcd->print("\5="); //"T(engine)="
    PatternTemperature(col+2, row, Engine);
}

void MenuLCD1602::PatternTemperature(int col, int row, Control control)
{
    lcd->setCursor(col, row);

    if (control == Engine)
    {
        lcd->print(controlEngine->GetTemperature());
    }
    else
    {
        lcd->print(controlIndoor->GetTemperature());
    }

    lcd->write(223);
}

void MenuLCD1602::PatternMinTemperature(int col, int row, Control control)
{
    lcd->setCursor(col, row);

    lcd->print("min=");

    if (control == Engine)
    {
        lcd->print(controlEngine->GetMinTemperature());
    }
    else
    {
        lcd->print(controlIndoor->GetMinTemperature());
    }

    lcd->write(223);
}

void MenuLCD1602::PatternMaxTemperature(int col, int row, Control control)
{
    lcd->setCursor(col, row);

    lcd->print("max=");

    if (control == Control::Engine)
    {
        lcd->print(controlEngine->GetMaxTemperature());
    }
    else
    {
        lcd->print(controlIndoor->GetMaxTemperature());
    }

    lcd->write(223);
}

/*
 * Print
*/

void MenuLCD1602::PrintMainMenu()
{
    // LCD-Character Create
    lcd->createChar(1, custom_termometr_Cool);
    lcd->createChar(2, custom_termometr_Heat);
    lcd->createChar(3, custom_selected);
    lcd->createChar(4, custom_home);
    //lcd->createChar(5, custom_engine);
    lcd->createChar(5, custom_heat);
    lcd->createChar(6, custom_play);
    lcd->createChar(7, custom_pause);


    // 1 string
    
    // Indoor Temperature
    PatternTemperatureIndoor(1,0);

    // Indoor Condition Temperature
    PatternConditionIndoor(10,0);

    // Indoor Condition Pump
    PatternConditionRelayIndoor(12,0);
    
    // 2 string

    // Engine Temperature
    PatternTemperatureEngine(1,1);
    
    // Engine Condition Temperature
    PatternConditionEngine(10,1);
    
    // Engine RelayA status
    // Engine RelayB status
    PatternConditionRelayEngine(12,1);
}

void MenuLCD1602::PrintIndoorSubmenu()
{
    // LCD-Character Create
    lcd->createChar(1, custom_termometr_Cool);
    lcd->createChar(2, custom_termometr_Heat);
    lcd->createChar(3, custom_selected);
    lcd->createChar(4, custom_home);
    //lcd->createChar(5, custom_engine);
    lcd->createChar(5, custom_heat);
    lcd->createChar(6, custom_play);
    lcd->createChar(7, custom_pause);

    // 1 string

    // Indoor Logo
    lcd->setCursor(0, 0);
    lcd->print("\4="); //"logo indoor="

    // Indoor Condition
    PatternConditionIndoor(2, 0);

    // Indoor ConditionRelay
    PatternConditionRelayIndoor(4, 0);

    // Indoor Min Temperature
    PatternMinTemperature(6, 0, Indoor);

    // 2 string

    // Indoor Temperature
    lcd->setCursor(0, 1);
    lcd->print(controlIndoor->GetTemperature()); //"T(indoor)="
    //PatternTemperature(0, 1, Control::Indoor);

    // Indoor Max Temperature
    PatternMaxTemperature(6, 1, Indoor);
}
void MenuLCD1602::PrintEngineSubmenu()
{
    // LCD-Character Create
    lcd->createChar(1, custom_termometr_Cool);
    lcd->createChar(2, custom_termometr_Heat);
    lcd->createChar(3, custom_selected);
    lcd->createChar(4, custom_home);
    //lcd->createChar(5, custom_engine);
    lcd->createChar(5, custom_heat);
    lcd->createChar(6, custom_play);
    lcd->createChar(7, custom_pause);

    // 1 string

    // Engine Logo
    lcd->setCursor(0, 0);
    lcd->print("\5="); //"logo engine="

    // Engine Condition
    PatternConditionEngine(2, 0);

    // Engine ConditionRelay
    PatternConditionRelayEngine(3, 0);

    // Engine Min Temperature
    PatternMinTemperature(6, 0, Control::Engine);

    // 2 string

    // Engine Temperature
    lcd->setCursor(0, 1);
    lcd->print(controlEngine->GetTemperature()); //"T(engine)="
    //PatternTemperature(0, 1, Control::Engine);

    // Engine Max Temperature
    PatternMaxTemperature(6, 1, Control::Engine);
}
