class DisplayLCD_Menu
{
private:
    byte custom_termometr_Cool[8] = { 0b00100, 0b01010, 0b01010, 0b01010, 0b01010, 0b10001, 0b11111, 0b01110 };
    byte custom_termometr_Heat[8] = { 0b00100, 0b01110, 0b01110, 0b01110, 0b01110, 0b11111, 0b11111, 0b01110 };
    //byte custom_termometr_GradusC[8] = { 0b11000, 0b11000, 0b00110, 0b01001, 0b01000, 0b01000, 0b01001, 0b00110 };
    byte custom_termometr_GradusC[8] = { 0b11000, 0b11000, 0b00110, 0b01001, 0b01000, 0b01001, 0b00110, 0b00000 };
    byte custom_selected[8] = { 0b00000, 0b00000, 0b00001, 0b00011, 0b10110, 0b11100, 0b01000, 0b00000};
    byte custom_home[8] = { 0b00100, 0b01110, 0b11011, 0b11111, 0b10001, 0b10001, 0b11111, 0b00000 };
    byte custom_engine[8] = { 0b11011, 0b01110, 0b10001, 0b10101, 0b10001, 0b01110, 0b11011, 0b00000 };
    byte custom_heatair[8] = { 0b01001, 0b10010, 0b10010, 0b01001, 0b01001, 0b10010, 0b00000, 0b11111 };
    byte custom_clock[8] = { 0b00000, 0b01110, 0b10101, 0b10111, 0b10001, 0b01110, 0b00000, 0b00000 };
    byte custom_play[8] = { 0b00000, 0b01000, 0b01100, 0b01110, 0b01100, 0b01000, 0b00000, 0b00000 };
    byte custom_pause[8] = { 0b00000, 0b01010, 0b01010, 0b01010, 0b01010, 0b01010, 0b00000, 0b00000};
    byte custom_on[8] = { 0b11111, 0b11011, 0b11011, 0b11111, 0b00100, 0b01010, 0b01010, 0b00100};
    byte custom_off[8] = { B00000, B00100, B00100, B00000, B11011, B10101, B10101, B11011};

    enum ScreenMode
    {
        Screen,
        Setting,
        Editing
    };

    LiquidCrystal_I2C* display;
    
    ControllerIndoor* controlIndoor;
    ControllerEngine* controlEngine;
    
    DS3231* clockRTC;
    ButtonTTB* btnLeft;
    ButtonTTB* btnModeSet;
    ButtonTTB* btnRight;

    ScreenMode screenMode = ScreenMode::Screen;
    int screen = 1;   // Початковий екран
    int setting = 0;  // 0 - вихід з налаштування параметрів

    // bool flagScreenLocked = false;
    // bool flagSetting = false;
    /// @brief mapping debug screen
    // const int screenMap[4][3] = 
    // {
    //     {1, 2, 21},
    //     {1, 2, 22},
    //     {0, 1, 1},
    //     {0, 1, 1}
    // };

    
    void ControlButton();
    void ControlScreens();

    /*
    Screen 01: 
    Temperature Climate | Watch Climate | RelayPump
    Temperature Engine | RelayA | RelayB
    */
    void Screen_Main();
    void Screen_MainSetting();
    /*
    Screen 01: ver 2
    Logo Watch Temperature Climate |
    Logo Watch Temperature Engine  | RelayA RelayB
    */
    void Screen_012();

    /*
    Screen 02: 
    Temperature Climate | Temperature Engine
    RelayA | RelayB |  Time  | Watch Climate
    */
    void Screen_02();

    /*
    Screen CLOCK: 
    Time: Hour | Minute | Second
    Time: Date | Month  | Year
    */
    void Screen_CLOCK();

    /*
    Screen 03: Temperature Engine
    Temperature Climate Max | Temperature Climate
    Temperature Climate Min | Watch Climate
    */
    void Screen_MinMaxIndoor();
    
    /*
    Screen 03: Temperature Engine
    Temperature Engine Max | Temperature Engine
    Temperature Engine Min | Watch Engine
    */
    void Screen_MinMaxEngine();

    void Screen_MinMaxSetting();
    void Screen_MinMaxEdit();

    void Patern_TemperatureClimate(int col, int row);
    void Patern_TemperatureEngine(int col, int row);
    void Patern_ConditionTemperatureClimate(int col, int row);
    void Patern_ConditionTemperatureEngine(int col, int row);
    void Patern_ConditionRelayClimate(int col, int row);
    void Patern_ConditionRelayEngine(int col, int row);



    /*
    Debug Screens: 
    Button: leftTrue | leftFalse
    Button: rightTrue | rightFalse
    */
    void Debug_btnLeftTrue();
    void Debug_btnLeftFalse();
    void Debug_btnRightTrue();
    void Debug_btnRightFalse();

public:
    DisplayLCD_Menu(LiquidCrystal_I2C* lcd, 
        ControllerEngine* controlEngine, ControllerIndoor* controlIndoor,
        ButtonTTB* btnLeft,
        ButtonTTB* btnModeSet,
        ButtonTTB* btnRight,
        DS3231* clockRTC);
    
    void Update();
};
