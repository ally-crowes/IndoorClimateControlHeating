#include "includes.h"
#include "DisplayLCD_Menu.h"

DisplayLCD_Menu::DisplayLCD_Menu(LiquidCrystal_I2C* lcd, 
    ControllerEngine* controlEngine, ControllerIndoor* controlIndoor,
    ButtonTTB* btnLeft, ButtonTTB* btnModeSet, ButtonTTB* btnRight,
    DS3231* clockRTC)
{
    this->display = lcd; 
    this->controlEngine = controlEngine;
    this->controlIndoor = controlIndoor;
    this->btnLeft = btnLeft;
    this->btnModeSet = btnModeSet;
    this->btnRight = btnRight;
    this->clockRTC = clockRTC;

}

void DisplayLCD_Menu::Update()
{
    ControlButton();
    ControlScreens();
}

void DisplayLCD_Menu::ControlButton()
{
    int oldScreen;
    oldScreen = screen;

    if (btnRight->keyDown())
    {
        if (screenMode == ScreenMode::Screen)
        {
            screen++;
        }
        else if (screenMode == ScreenMode::Setting)
        {
            setting--;
        }
        else
        {
            if (screen == 3 && setting == 1) controlIndoor->SetMinTemperature(controlIndoor->GetMinTemperature() + 0.1f);
            if (screen == 3 && setting == 2) controlIndoor->SetMaxTemperature(controlIndoor->GetMaxTemperature() + 0.1f);
            if (screen == 4 && setting == 1) controlEngine->SetMinTemperature(controlEngine->GetMinTemperature() + 0.1f);
            if (screen == 4 && setting == 2) controlEngine->SetMaxTemperature(controlEngine->GetMaxTemperature() + 0.1f);
        }
    }
    else if (btnLeft->keyDown())
    {
        if (screenMode == ScreenMode::Screen)
        {
            screen--;
        }
        else if (screenMode == ScreenMode::Setting)
        {
            setting++;
        }
        else
        {
            if (screen == 3 && setting == 1) controlIndoor->SetMinTemperature(controlIndoor->GetMinTemperature() - 0.1f);
            if (screen == 3 && setting == 2) controlIndoor->SetMaxTemperature(controlIndoor->GetMaxTemperature() - 0.1f);
            if (screen == 4 && setting == 1) controlEngine->SetMinTemperature(controlEngine->GetMinTemperature() - 0.1f);
            if (screen == 4 && setting == 2) controlEngine->SetMaxTemperature(controlEngine->GetMaxTemperature() - 0.1f);
        }
    }
    else if (btnModeSet->keyDown())
    {
        if (screenMode == ScreenMode::Editing && (setting == 1 || setting == 2))
        {
            display->clear();
            screenMode = ScreenMode::Screen;
            screen = 1;
        }
        else if (setting != 0)
        {
            if (screen == 1)
            {
                switch (setting)
                {
                case 1:
                    screen = 3;
                    setting = 0;
                    break;

                case 2:
                    screen = 4;
                    setting = 0;
                    break;
                }
            }
            else if (screen == 2)
            {
                screenMode = ScreenMode::Editing;
            }
            else if (screen == 3) // Screen_CLOCK() 
            {
                screenMode = ScreenMode::Screen;
            }
        }
        else if (screen == 1 || screen == 3 || screen == 4)
        {
            screenMode = ScreenMode::Setting;
            setting++;
        }
    }

    display->setCursor(15,0); display->print(screenMode);
    display->setCursor(15,1); display->print(setting);

    if (oldScreen != screen)
    {
        display->clear();
    }
}
    
void DisplayLCD_Menu::ControlScreens()
{
    // if (flagUpdateScreen)
    // {
    //     flagUpdateScreen = false;
    // }
    screen = constrain(screen, 1 , 4);

    if (screenMode != ScreenMode::Editing)
    {
        switch (screen)
        {
        case 1:
            Screen_Main();
            Screen_MainSetting();
            break;

        case 2:
            Screen_CLOCK();
            break;

        case 3:
            Screen_MinMaxIndoor();
            Screen_MinMaxSetting();
            // screenMode == ScreenMode::Editing ? Screen_MinMaxEdit() : screenMode == ScreenMode::Setting ? Screen_MinMaxSetting() : Screen_MinMaxIndoor();
            break;

        case 4:
            Screen_MinMaxEngine();
            Screen_MinMaxSetting();
            // screenMode == ScreenMode::Editing ? Screen_MinMaxEdit() : Screen_MinMaxEngine();
            break;

            // case 2:
            //     Screen_02();
            //     break;

        default:
            break;
        }
    }
    else
    {
        switch (screen)
        {
        case 3:
        case 4:
            Screen_MinMaxSetting();
            break;
        
        default:
            break;
        }
    }

    // display->setCursor(15,1); display->print(setting);
}

void DisplayLCD_Menu::Screen_Main() //ver1
{
        // LCD-Character Create
    display->createChar(1, custom_termometr_Cool);
    display->createChar(2, custom_termometr_Heat);
    display->createChar(3, custom_selected);
    display->createChar(4, custom_home);
    //display->createChar(5, custom_engine);
    display->createChar(5, custom_heatair);
    display->createChar(6, custom_play);
    display->createChar(7, custom_pause);

    // LCD-Character Paterns
    // display->createChar(, custom_termometr_GradusC);
    //
    // display->createChar(6, custom_on);
    // display->createChar(7, custom_off);
    //
    // display->createChar(3, custom_selected);
    // display->createChar(3, custom_clock);

    
    // 1 string
    
    // Climate Temperature
    Patern_TemperatureClimate(1,0);

    // Climate Condition Temperature
    Patern_ConditionTemperatureClimate(10,0);

    // Climate Condition Pump
    Patern_ConditionRelayClimate(12,0);
    
    // 2 string

    // Engine Temperature
    Patern_TemperatureEngine(1,1);
    
    // Engine Condition Temperature
    Patern_ConditionTemperatureEngine(10,1);
    
    // Engine RelayA status
    // Engine RelayB status
    Patern_ConditionRelayEngine(12,1);
}

void DisplayLCD_Menu::Screen_MainSetting()
{
    if (screenMode == ScreenMode::Setting)
    {
        setting = constrain(setting, 0, 2);

        switch (setting)
        {
        case 0:
            display->setCursor(0, 0);
            display->print(" ");
            display->setCursor(0, 1);
            display->print(" ");
            screenMode = ScreenMode::Screen;
            break;
        case 1:
            display->setCursor(0, 0);
            display->print("\3");
            display->setCursor(0, 1);
            display->print(" ");
            screenMode = ScreenMode::Setting;
            break;
        case 2:
            display->setCursor(0, 0);
            display->print(" ");
            display->setCursor(0, 1);
            display->print("\3");
            screenMode = ScreenMode::Setting;
            break;
        }
    }
}

void DisplayLCD_Menu::Screen_MinMaxIndoor()
{
    // 1 string

    // Climate Condition Temperature
    display->setCursor(0,0);
    display->print("\4="); //"T(climate)="
    Patern_ConditionTemperatureClimate(2,0);

    // Climate Min Temperature
    display->setCursor(5,0);
    display->print("min="); //"T(climate)="
    display->print(controlIndoor->GetMinTemperature());
    display->write(223);

    // Climate Temperature
    // display->setCursor(11,0);
    // display->print(controlIndoor->GetTemperature());
    // display->write(223);

    // 2 string

    // Climate Max Temperature
    display->setCursor(5,1);
    display->print("max="); //"T(climate)="
    display->print(controlIndoor->GetMaxTemperature());
    display->write(223);
}

void DisplayLCD_Menu::Screen_MinMaxEngine()
{
    // 1 string

    // Engine Condition Temperature
    display->setCursor(0,0);
    display->print("\5="); //"T(engine)="
    Patern_ConditionTemperatureEngine(2,0);

    // Engine Min Temperature
    display->setCursor(5,0);
    display->print("min="); //"T(engine)="
    display->print(controlEngine->GetMinTemperature());
    display->write(223);

    // Engine Temperature
    // display->setCursor(11, 0);
    // display->print(controlEngine->GetTemperature());
    // display->write(223);

    // 2 string

    // Engine Max Temperature
    display->setCursor(5,1);
    display->print("max="); //"T(engine)="
    display->print(controlEngine->GetMaxTemperature());
    display->write(223);
}

void DisplayLCD_Menu::Screen_MinMaxSetting()
{
    setting = constrain(setting, 0 , 2);

    switch (setting)
    {
    case 0:
        //display->clear();
        display->setCursor(4, 0);
        display->print(" ");
        display->setCursor(4, 1);
        display->print(" ");
        screenMode = ScreenMode::Screen;
        setting = 0;
        //screen = 1;
        break;
    case 1:
        display->setCursor(4, 0);
        if (screenMode == ScreenMode::Editing)
        {
            display->print("\6");
        }
        else
        {
            display->print("\3");
        }
        display->setCursor(4, 1);
        display->print(" ");
        // screenMode = ScreenMode::Setting;
        break;
    case 2:
        display->setCursor(4, 0);
        display->print(" ");
        display->setCursor(4, 1);
        if (screenMode == ScreenMode::Editing)
        {
            display->print("\6");
        }
        else
        {
            display->print("\3");
        }
        // screenMode = ScreenMode::Setting;
        break;
    }
}

// Merged to method Screen_MinMaxSetting() [if/else]
void DisplayLCD_Menu::Screen_MinMaxEdit()
{
   setting = constrain(setting, 0 , 2);

    switch (setting)
    {
    case 0:
        // display->clear();
        screenMode = ScreenMode::Screen;
        screen = 1;
        break;
    case 1:
        display->setCursor(4, 0);
        display->print("\6");
        display->setCursor(4, 1);
        display->print(" ");
        // screenMode = ScreenMode::Editing;
        if (screen == 3) Screen_MinMaxIndoor();
        if (screen == 4) Screen_MinMaxEngine();
        break;
    case 2:
        display->setCursor(4, 0);
        display->print(" ");
        display->setCursor(4, 1);
        display->print("\6");
        // screenMode = ScreenMode::Editing;
        if (screen == 3) Screen_MinMaxIndoor();
        if (screen == 4) Screen_MinMaxEngine();
        break;
    }
}

void DisplayLCD_Menu::Screen_CLOCK()
{
    // RelayTimeClock
    bool century = false;
    bool h12Flag;
    bool pmFlag;
    display->setCursor(0, 0);
    display->print(clockRTC->getHour(h12Flag, pmFlag), DEC);
    display->print(":");
    display->print(clockRTC->getMinute(), DEC);
    display->print(":");
    display->print(clockRTC->getSecond(), DEC);
    display->print(" ");

    // display->setCursor(8, 1);
    display->setCursor(6, 1);
    display->print(clockRTC->getDate(), DEC);
    display->print("-");
    display->print(clockRTC->getMonth(century), DEC);
    display->print("-");
    display->print(clockRTC->getYear(), DEC);
}

void DisplayLCD_Menu::Patern_TemperatureClimate(int col, int row)
{
    display->setCursor(col, row);
    display->print("\4="); // "T(climate)="
    display->print(controlIndoor->GetTemperature());
    display->write(223);
}

void DisplayLCD_Menu::Patern_TemperatureEngine(int col, int row)
{
    display->setCursor(col, row);
    display->print("\5="); //"T(engine)="
    display->print(controlEngine->GetTemperature());
    display->write(223);
}

void DisplayLCD_Menu::Patern_ConditionTemperatureClimate(int col, int row)
{
    display->setCursor(col, row);

    // Climate Temperature
    if (controlIndoor->GetCondition())
    {
        display->print(char(1)); // Working
    }
    else
    {
        display->print(char(2)); // Await
    }
}

void DisplayLCD_Menu::Patern_ConditionTemperatureEngine(int col, int row)
{
    display->setCursor(col, row);

    // Climate Condition
    if (!controlIndoor->GetCondition())
    {
        display->print(char(7)); // Skip
    }
    else
    {
        // Engine Condition
        if (controlEngine->GetCondition())
        {
            display->print(char(1)); // Working
        }
        else
        {
            display->print(char(2)); // Wait
        }
    }
}

void DisplayLCD_Menu::Patern_ConditionRelayEngine(int col, int row)
{
    display->setCursor(col, row);

    // Engine RelayA
    if (controlEngine->GetConditionRelay(1))
    {
        display->print(char(6)); // Working
    }
    else
    {
        //display->write(165); // Awaits
        display->print(char(7));
    }
    
    // Engine RelayB status
    if (controlEngine->GetConditionRelay(2))
    {
        display->print(char(6)); // Working
    }
    else
    {
        //display->write(165); // Awaits
        display->print(char(7));
    }
}

void DisplayLCD_Menu::Patern_ConditionRelayClimate(int col, int row)
{
    display->setCursor(col, row);

    // Climate Pump
    if (controlIndoor->GetConditionPump())
    {
        display->print(char(6)); // Working
    }
    else
    {
        //display->write(165); // Awaits
        display->print(char(7));
    }
}




// First version: 
void DisplayLCD_Menu::Screen_02()
{
    // 1 string

    display->setCursor(0,0);
    display->print("\5="); //"T(engine)="
    display->print(controlEngine->GetTemperature());
    display->write(223);

    //display->setCursor(0,1);
    display->print("\4="); // "T(climate)="
    display->print(controlIndoor->GetTemperature());
    display->write(223);

    
    // 2 string

    // Status Relay
    display->setCursor(0,1);
    if (controlEngine->GetConditionRelay(1))
    {
        display->print(char(6)); // Working
    }
    else
    {
        display->print(char(7)); // Awaits
    }

    //display->setCursor(15,1);
    if (controlEngine->GetConditionRelay(2))
    {
        display->print(char(6)); // Working
    }
    else
    {
        display->print(char(7));  // Awaits
    }

    // Status climate Temperature
    display->setCursor(15,1);
    if (controlIndoor->GetCondition())
    {
        display->print(char(1)); // Working
    }
    else
    {
        display->print(char(2)); // Await
    }
    
    // RelayTimeClock
    //bool century = false;
    bool h12Flag;
    bool pmFlag;
    display->setCursor(5, 1);
    display->print(clockRTC->getHour(h12Flag, pmFlag), DEC);
    display->print(":");
    display->print(clockRTC->getMinute(), DEC);
    display->print(":");
    display->print(clockRTC->getSecond(), DEC);
    display->print(" ");
}




/**
 * 
 * DEBUG
 * 
 * Get message to LCD
 * 
 **/
void DisplayLCD_Menu::Debug_btnLeftTrue()
{
    display->setCursor(0, 0);
    display->print("leftTrue        ");
}

void DisplayLCD_Menu::Debug_btnLeftFalse()
{
    display->setCursor(0, 0);
    display->print("       leftFalse");
}

void DisplayLCD_Menu::Debug_btnRightTrue()
{
    display->setCursor(0, 1);
    display->print("rightTrue       ");
}

void DisplayLCD_Menu::Debug_btnRightFalse()
{
    display->setCursor(0, 1);
    display->print("      rightFalse");
}