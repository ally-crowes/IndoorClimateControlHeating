#include "includes.h"
#include "MenuLCD1602.h"

MenuLCD1602::MenuLCD1602(LiquidCrystal_I2C* lcd, ControllerEngine* controlEngine, ControllerIndoor* controlIndoor, DS3231* rtc)
    : lcd{lcd}, controlEngine{controlEngine}, controlIndoor{controlIndoor}, rtc{rtc}
{
}

MenuLCD1602::~MenuLCD1602()
{
}

void MenuLCD1602::Initialize()
{
    // LCD-Character Create
    lcd->createChar(1, custom_termometr_Cool);
    lcd->createChar(2, custom_termometr_Heat);
    lcd->createChar(3, custom_selected);
    lcd->createChar(4, custom_home);
    // lcd->createChar(5, custom_engine);
    lcd->createChar(5, custom_heat);
    lcd->createChar(6, custom_play);
    lcd->createChar(7, custom_pause);

    PrintMainMenu();
}

/*
 * Pattern
 */

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

void MenuLCD1602::PatternCursor(int col, int row, Cursor cursor)
{
    lcd->setCursor(col, row);

    if (cursor == Set)
    {
        lcd->print("\3");
    }
    else
    {
        lcd->print(" ");
    }
}

/*
 * Print
*/

void MenuLCD1602::PrintMainMenu(int selected)
{
    // 1 string

    // Cursor select Indoor
    if (selected == 1)
    {
        PatternCursor(0, 0, Set);
    }
    else
    {
        PatternCursor(0, 0, Not);
    }

    // Indoor Temperature
    PatternTemperatureIndoor(1, 0);

    // Indoor Condition Temperature
    PatternConditionIndoor(10, 0);

    // Indoor Condition Pump
    PatternConditionRelayIndoor(12, 0);

    // 2 string

    // Cursor select Engine
    if (selected == 2)
    {
        PatternCursor(0, 1, Set);
    }
    else
    {
        PatternCursor(0, 1, Not);
    }

    // Engine Temperature
    PatternTemperatureEngine(1, 1);

    // Engine Condition Temperature
    PatternConditionEngine(10, 1);

    // Engine RelayA status
    // Engine RelayB status
    PatternConditionRelayEngine(12, 1);
}

void MenuLCD1602::PrintEngineSubmenu(int selected)
{
    // 1 string

    // Engine Logo
    lcd->setCursor(0, 0);
    lcd->print("\5");

    // Engine Condition
    PatternConditionEngine(2, 0);

    // Engine ConditionRelay
    PatternConditionRelayEngine(3, 0);

    // Cursor select Min Temperature
    if (selected == 1)
    {
        PatternCursor(5, 0, Set);
    }
    else 
    {
        PatternCursor(5, 0, Not);
    }

    // Engine Min Temperature
    PatternMinTemperature(6, 0, Control::Engine);

    // 2 string

    // Engine Temperature
    PatternTemperature(0, 1, Engine);

    // Cursor select Max Temperature
    if (selected == 2)
    {
        PatternCursor(5, 1, Set);
    }
    else 
    {
        PatternCursor(5, 1, Not);
    }

    // Engine Max Temperature
    PatternMaxTemperature(6, 1, Control::Engine);
}

void MenuLCD1602::PrintIndoorSubmenu(int selected)
{
    // 1 string

    // Indoor Logo
    lcd->setCursor(0, 0);
    lcd->print("\4");

    // Indoor Condition
    PatternConditionIndoor(2, 0);

    // Indoor ConditionRelay
    PatternConditionRelayIndoor(3, 0);

    // Cursor select Min Temperature
    if (selected == 1)
    {
        PatternCursor(5, 0, Set);
    }
    else 
    {
        PatternCursor(5, 0, Not);
    }

    // Indoor Min Temperature
    PatternMinTemperature(6, 0, Indoor);

    // 2 string
    
    // Indoor Temperature
    PatternTemperature(0, 1, Indoor);
    
    // Cursor select Max Temperature
    if (selected == 2)
    {
        PatternCursor(5, 1, Set);
    }
    else 
    {
        PatternCursor(5, 1, Not);
    }

    // Indoor Max Temperature
    PatternMaxTemperature(6, 1, Indoor);
}
