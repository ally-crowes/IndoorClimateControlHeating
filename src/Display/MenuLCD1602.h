#ifndef __MENULCD1602_H__
#define __MENULCD1602_H__

class MenuLCD1602
{
private:
    byte custom_termometr_Cool[8] = { 0b00100, 0b01010, 0b01010, 0b01010, 0b01010, 0b10001, 0b11111, 0b01110 };
    byte custom_termometr_Heat[8] = { 0b00100, 0b01110, 0b01110, 0b01110, 0b01110, 0b11111, 0b11111, 0b01110 };
    //byte custom_termometr_GradusC[8] = { 0b11000, 0b11000, 0b00110, 0b01001, 0b01000, 0b01000, 0b01001, 0b00110 };
    byte custom_termometr_GradusC[8] = { 0b11000, 0b11000, 0b00110, 0b01001, 0b01000, 0b01001, 0b00110, 0b00000 };
    byte custom_selected[8] = { 0b00000, 0b00000, 0b00001, 0b00011, 0b10110, 0b11100, 0b01000, 0b00000};
    byte custom_home[8] = { 0b00100, 0b01110, 0b11011, 0b11111, 0b10001, 0b10001, 0b11111, 0b00000 };
    byte custom_engine[8] = { 0b11011, 0b01110, 0b10001, 0b10101, 0b10001, 0b01110, 0b11011, 0b00000 };
    byte custom_heat[8] = { 0b01001, 0b10010, 0b10010, 0b01001, 0b01001, 0b10010, 0b00000, 0b11111 };
    byte custom_clock[8] = { 0b00000, 0b01110, 0b10101, 0b10111, 0b10001, 0b01110, 0b00000, 0b00000 };
    byte custom_play[8] = { 0b00000, 0b01000, 0b01100, 0b01110, 0b01100, 0b01000, 0b00000, 0b00000 };
    byte custom_pause[8] = { 0b00000, 0b01010, 0b01010, 0b01010, 0b01010, 0b01010, 0b00000, 0b00000};
    byte custom_on[8] = { 0b11111, 0b11011, 0b11011, 0b11111, 0b00100, 0b01010, 0b01010, 0b00100};
    byte custom_off[8] = { B00000, B00100, B00100, B00000, B11011, B10101, B10101, B11011};

    enum Control
    {
        Engine,
        Indoor
    };

    enum Cursor
    {
        Not,
        Set
    };

    LiquidCrystal_I2C* lcd;
    ControllerEngine* controlEngine;
    ControllerIndoor* controlIndoor;
    DS3231* rtc;

    void PatternConditionIndoor(int col, int row);
    void PatternConditionEngine(int col, int row);
    void PatternConditionRelayEngine(int col, int row);
    void PatternConditionRelayIndoor(int col, int row);
    void PatternTemperatureIndoor(int col, int row);
    void PatternTemperatureEngine(int col, int row);
    void PatternTemperature(int col, int row, Control control);
    void PatternMinTemperature(int col, int row, Control control);
    void PatternMaxTemperature(int col, int row, Control control);
    void PatternCursor(int col, int row, Cursor cursor);

public:
    MenuLCD1602(LiquidCrystal_I2C* lcd, ControllerEngine* controlEngine, ControllerIndoor* controlIndoor, DS3231* rtc);
    ~MenuLCD1602();

    void Initialize();
    // void PrintMainMenu();
    void PrintMainMenu(int selected = 0);
    // void PrintEngineSubmenu();
    void PrintEngineSubmenu(int selected = 0);
    // void PrintIndoorSubmenu();
    void PrintIndoorSubmenu(int selected = 0);
};

#endif