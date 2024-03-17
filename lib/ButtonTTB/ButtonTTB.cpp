#include "ButtonTTB.h"
#include <Arduino.h>

/*
#define BUTTON_PIN_PULLUP  // 0-false / 1-true
#define BUTTON_PIN BUTTON_3 // Пін тактової кнопки

#define LED_PIN RELAY_3     // Пін світлодіода
#define LED_PIN2 RELAY_4     // Пін світлодіода
*/

ButtonTTB::ButtonTTB(int pin) : ButtonTTB(pin, INPUT_PULLUP)
{
}

ButtonTTB::ButtonTTB(int pin, int mode ) : pin{pin}
{
    /*
    setup();
    */
    pinMode(pin, mode); // Пін кнопки встановлюєм як вхід
}

/*
void ButtonTTB::setup()
{
    pinMode(pin, INPUT_PULLUP); // Пін кнопки встановлюєм як вхід
    pinMode(LED_PIN, OUTPUT);          // Пін світлодіода встановлюєм як вихід
    pinMode(LED_PIN2, OUTPUT);          // Пін світлодіода встановлюєм як вихід
}

void ButtonTTB::toggle_led1(int pin)
{
    static bool pin_led_state = false;    // Змінна стану світлодіода
    pin_led_state = !pin_led_state;       // Змінюєм стан на протилежний
    digitalWrite(pin, pin_led_state); // Записуєм стан в порт
}

void ButtonTTB::toggle_led2(int pin)
{
    static bool pin_led_state = false;    // Змінна стану світлодіода
    pin_led_state = !pin_led_state;       // Змінюєм стан на протилежний
    digitalWrite(pin, pin_led_state); // Записуєм стан в порт
}

void ButtonTTB::read_button()
{
    keyDown();
    keyUp();
}
*/

bool ButtonTTB::keyDown()
{
    /*
    static bool flag_read_button = false;  // Прапорець читання кнопки
    static unsigned long timer = 0;        // Змінна часу останнього натискання
    */
    bool result = false;

    if (!flag_read_button)
    {
#if (mode == INPUT_PULLUP)
        if (!digitalRead(pin))      // Якщо кнопка натиснута
#else
        if (digitalRead(pin))       // Якщо кнопка натиснута
#endif
        {
            flag_read_button = true;       // Вмикаєм читання кнопки - починаєм боротьбу з contact bounce
            timer = millis();              // Зберігаєм час натискання
        }
    }
    else                                   // Боротьба з contact bounce
    {
#if (mode == INPUT_PULLUP)
        if (!digitalRead(pin))      // Якщо кнопка утримується
#else
        if (digitalRead(pin))       // Якщо кнопка утримується
#endif
        {
            timer = millis();              // Перезаписуєм час натискання
        }
        else                               // Якщо кнопку відпустили
        {
            if ((millis() - timer) > 50)   // Очікуєм 50 мілісекунд коли закінчиться брязкіт
            {
                flag_read_button = false;  // Вимикаєм читання кнопки
                //toggle_led1(LED_PIN);              // Перемикаєм стан світлодіода на протилежний
                result = true;
            }
        }
    }

    return result;
}

bool ButtonTTB::keyUp()
{
    /*
    static bool flag_read_button = false;  // Прапорець читання кнопки
    static unsigned long timer = 0;        // Змінна часу останнього натискання
    */
    bool result = false;

    if (!flag_read_button)
    {
#if (mode == INPUT_PULLUP)
        if (digitalRead(pin))       // Якщо кнопка відпущена
#else
        if (!digitalRead(pin))      // Якщо кнопка відпущена
#endif
        {
            flag_read_button = true;       // Вмикаєм читання кнопки - починаєм боротьбу з contact bounce
            timer = millis();              // Зберігаєм час звільнена
        }
    }
    else                                   // Боротьба з contact bounce
    {
#if (mode == INPUT_PULLUP)
        if (digitalRead(pin))       // Якщо кнопка звільнена
#else
        if (!digitalRead(pin))      // Якщо кнопка звільнена
#endif
        {
            timer = millis();              // Перезаписуєм час натискання
        }
        else                               // Якщо кнопку відпустили
        {
            if ((millis() - timer) > 50)   // Очікуєм 50 мілісекунд коли закінчиться брязкіт
            {
                flag_read_button = false;  // Вимикаєм читання кнопки
                //toggle_led1(LED_PIN);              // Перемикаєм стан світлодіода на протилежний
                result = true;
            }
        }
    }
    
    return result;
}


/*
void loop()
{
  read_button();                       // Цикл читання кнопки
  // Виконуєм якусь роботу без блокування !(без delay())
}
*/