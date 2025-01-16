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

    printMainMenu();
}

/*
 * Pattern
 */

void MenuLCD1602::patternConditionIndoor(int col, int row)
{
    lcd->setCursor(col, row);

    // Climate Condition
    if (controlIndoor->GetCondition())
    {
        lcd->print(char(1)); // Working
    }
    else
    {
        lcd->print(char(2)); // Await
    }
}

void MenuLCD1602::patternConditionEngine(int col, int row)
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

void MenuLCD1602::patternConditionRelayEngine(int col, int row)
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

void MenuLCD1602::patternConditionRelayIndoor(int col, int row)
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

void MenuLCD1602::patternTemperatureIndoor(int col, int row)
{
    lcd->setCursor(col, row);
    lcd->print("\4="); // "T(Indoor)="
    patternTemperature(col+2, row, Control::Indoor);
}

void MenuLCD1602::patternTemperatureEngine(int col, int row)
{
    lcd->setCursor(col, row);
    lcd->print("\5="); //"T(engine)="
    patternTemperature(col+2, row, Engine);
}

void MenuLCD1602::patternTemperature(int col, int row, Control control)
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

void MenuLCD1602::patternTemperatureMin(int col, int row, Control control)
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

void MenuLCD1602::patternTemperatureMax(int col, int row, Control control)
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

void MenuLCD1602::patternCursor(int col, int row, Cursor cursor)
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

void MenuLCD1602::printMainMenu(int selected)
{
    // 1 string

    // Cursor select Indoor
    if (selected == 1)
    {
        patternCursor(0, 0, Set);
    }
    else if (selected == 2)
    {
        patternCursor(0, 1, Set);
    }
    else
    {
        patternCursor(0, 0, Not);
    }

    // Indoor Temperature
    patternTemperatureIndoor(1, 0);

    // Indoor Condition Temperature
    patternConditionIndoor(10, 0);

    // Indoor Condition Pump
    patternConditionRelayIndoor(12, 0);

    // 2 string

    // Cursor select Engine
    if (selected == 2)
    {
        patternCursor(0, 1, Set);
    }
    else
    {
        patternCursor(0, 1, Not);
    }

    // Engine Temperature
    patternTemperatureEngine(1, 1);

    // Engine Condition Temperature
    patternConditionEngine(10, 1);

    // Engine RelayA status
    // Engine RelayB status
    patternConditionRelayEngine(12, 1);
}

void MenuLCD1602::printEngineSubmenu(int selected)
{
    // 1 string

    // Engine Logo
    lcd->setCursor(0, 0);
    lcd->print("\5");

    // Engine Condition
    patternConditionEngine(2, 0);

    // Engine ConditionRelay
    patternConditionRelayEngine(3, 0);

    // Cursor select Min Temperature
    if (selected == 1)
    {
        patternCursor(5, 0, Set);
    }
    else if (selected == 2)
    {
        patternCursor(5, 1, Set);
    }
    else 
    {
        patternCursor(5, 0, Not);
    }

    // Engine Min Temperature
    patternTemperatureMin(6, 0, Control::Engine);

    // 2 string

    // Engine Temperature
    patternTemperature(0, 1, Engine);

    // Cursor select Max Temperature
    if (selected == 2)
    {
        patternCursor(5, 1, Set);
    }
    else 
    {
        patternCursor(5, 1, Not);
    }

    // Engine Max Temperature
    patternTemperatureMax(6, 1, Control::Engine);
}

void MenuLCD1602::printIndoorSubmenu(int selected)
{
    // 1 string

    // Indoor Logo
    lcd->setCursor(0, 0);
    lcd->print("\4");

    // Indoor Condition
    patternConditionIndoor(2, 0);

    // Indoor ConditionRelay
    patternConditionRelayIndoor(3, 0);

    // Cursor select Min Temperature
    if (selected == 1)
    {
        patternCursor(5, 0, Set);
    }
    else if (selected == 2)
    {
        patternCursor(5, 1, Set);
    }
    else 
    {
        patternCursor(5, 0, Not);
    }

    // Indoor Min Temperature
    patternTemperatureMin(6, 0, Indoor);

    // 2 string
    
    // Indoor Temperature
    patternTemperature(0, 1, Indoor);
    
    // Cursor select Max Temperature
    if (selected == 2)
    {
        patternCursor(5, 1, Set);
    }
    else 
    {
        patternCursor(5, 1, Not);
    }

    // Indoor Max Temperature
    patternTemperatureMax(6, 1, Indoor);
}

void MenuLCD1602::printException(int marker, int code)
{
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print(marker);
    lcd->setCursor(0, 1);
    lcd->print(code);
}
