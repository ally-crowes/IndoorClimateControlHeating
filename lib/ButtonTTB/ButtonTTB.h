#ifndef __BUTTON_TTB_H__
#define __BUTTON_TTB_H__

//  Timer Terminated Bounce  
class ButtonTTB
{
private:
    int pin;
    bool flag_read_button = false;  // Прапорець читання кнопки
    unsigned long timer = 0;        // Змінна часу останнього натискання
    /*
    void setup();
    void toggle_led1(int pin);
    void toggle_led2(int pin);
    */

public:
    ButtonTTB(int pin);
    ButtonTTB(int pin, int mode);

    /*
    void read_button();
    */
    bool keyDown();
    //void keyPress();
    bool keyUp();
};

#endif