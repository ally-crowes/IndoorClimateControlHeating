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

    printMainmenu();
}

/*
 * Pattern
 */

void MenuLCD1602::conditionObjectIndoor(int col, int row)
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

void MenuLCD1602::conditionObjectEngine(int col, int row)
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

void MenuLCD1602::conditionRelayEngine(int col, int row)
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

void MenuLCD1602::conditionRelayIndoor(int col, int row)
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

void MenuLCD1602::patternTemperature(int col, int row, Screen screen, bool visableChar)
{
    lcd->setCursor(col, row);

    if (screen == ScreenEngine)
    {
        lcd->print(controlEngine->GetTemperature());
    }
    else
    {
        lcd->print(controlIndoor->GetTemperature());
    }

    if (visableChar)
    {
        lcd->write(223);
    }
}

void MenuLCD1602::patternTemperatureMin(int col, int row, Screen screen)
{
    lcd->setCursor(col, row);

    lcd->print("min=");

    if (screen == ScreenEngine)
    {
        lcd->print(controlEngine->GetMinTemperature());
    }
    else
    {
        lcd->print(controlIndoor->GetMinTemperature());
    }

    lcd->write(223);
}

void MenuLCD1602::patternTemperatureMax(int col, int row, Screen screen)
{
    lcd->setCursor(col, row);

    lcd->print("max=");

    if (screen == ScreenEngine)
    {
        lcd->print(controlEngine->GetMaxTemperature());
    }
    else
    {
        lcd->print(controlIndoor->GetMaxTemperature());
    }

    lcd->write(223);
}

void MenuLCD1602::patternCursor(int col, int row, Control cursor)
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

void MenuLCD1602::printMainmenu(int select)
{
    // 1 Row (string)

    // Indoor Temperature
    lcd->setCursor(1, 0);
    lcd->print("\4=");
    patternTemperature(3, 0, ScreenIndoor);

    // Indoor Condition Temperature
    conditionObjectIndoor(10, 0);

    // Indoor Condition Pump
    conditionRelayIndoor(12, 0);

    // 2 Row (string)

    // Engine Temperature
    lcd->setCursor(1, 1);
    lcd->print("\5=");
    patternTemperature(3, 1, ScreenEngine);

    // Engine Condition Temperature
    conditionObjectEngine(10, 1);

    // Engine RelayA status
    // Engine RelayB status
    conditionRelayEngine(12, 1);


    ///
    // 1&2 Row
    //

    // Set cursor
    cursorTriggerCol(select, 0);
}

void MenuLCD1602::printEngineSubmenu(int select)
{
    //
    // 1 Row (string)
    //

    // Engine Logo
    lcd->setCursor(0, 0);
    lcd->print("\5");

    // Engine Condition
    conditionObjectEngine(2, 0);

    // Engine ConditionRelay
    conditionRelayEngine(3, 0);

    // Engine Min Temperature
    patternTemperatureMin(6, 0, ScreenEngine);

    //
    // 2 Row (string)
    //

    // Engine Temperature
    patternTemperature(0, 1, ScreenEngine, false);

    // Engine Max Temperature
    patternTemperatureMax(6, 1, ScreenEngine);
    
    //
    // 1&2 Row
    //

    // Set cursor
    cursorTriggerCol(select, 5);
}

void MenuLCD1602::printIndoorSubmenu(int select)
{
    //
    // 1 Row (string)
    //

    // Indoor Logo
    lcd->setCursor(0, 0);
    lcd->print("\4");

    // Indoor Condition
    conditionObjectIndoor(2, 0);

    // Indoor ConditionRelay
    conditionRelayIndoor(3, 0);

    // Indoor Min Temperature
    patternTemperatureMin(6, 0, ScreenIndoor);
    
    //
    // 2 Row (string)
    //
    
    // Indoor Temperature
    patternTemperature(0, 1, ScreenIndoor, false);

    // Indoor Max Temperature
    patternTemperatureMax(6, 1, ScreenIndoor);
    
    //
    // 1&2 Row
    //

    // Set cursor
    cursorTriggerCol(select, 5);
}

// Set single position column for 1&2 row
void MenuLCD1602::cursorTriggerCol(int select, int col)
{

    // if (oldSelect != select)
    // {
    //     Serial.print("select-");
    //     Serial.print(select);
    //     Serial.print(" oldSelect-");
    //     Serial.println(oldSelect);

        switch (select)
        {
        case 0:
            patternCursor(col, 0, Not);
            patternCursor(col, 1, Not);
            break;
        case 1:
            patternCursor(col, 0, Set);
            patternCursor(col, 1, Not);
            break;
        case 2:
            patternCursor(col, 0, Not);
            patternCursor(col, 1, Set);
            break;

        default:
            break;
        }

        oldSelect = select;
   // }
}

void MenuLCD1602::clear()
{
    lcd->clear();
}

void MenuLCD1602::printException(const char *marker, int code)
{
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print(marker);
    lcd->setCursor(0, 1);
    lcd->print(code);
}
