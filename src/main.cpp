#include "includes.h"

/*---------------------------------------------------------*/
/*  Global Variables                                       */
/*---------------------------------------------------------*/

SensorTypeNTC* sensorEngine = new SensorTypeNTC(29.6, 30.6, SENSOR_1);
SensorTypeNTC* sensorIndoor = new SensorTypeNTC(28.0, 29.0, SENSOR_2);
// SensorTypeNTC* sensorEngine = new SensorTypeNTC(50, 58, SENSOR_1);
// SensorTypeNTC* sensorIndoor = new SensorTypeNTC(25.9, 26.6, SENSOR_2);

Relay* relayA = new Relay(RELAY_1);
Relay* relayB = new Relay(RELAY_2);
// Relay* relayReserved = new Relay(RELAY_3);
Relay* relayPumb = new Relay(RELAY_4);

ControllerEngine* controlEngine = new ControllerEngine(sensorEngine, relayA, relayB);
// ControllerIndoor* controlIndoor = new ControllerIndoor(sensorIndoor, controlEngine);
ControllerIndoor* controlIndoor = new ControllerIndoor(sensorIndoor, controlEngine, relayPumb);

DS3231* clockRTC = new DS3231();

LiquidCrystal_I2C* lcd = new LiquidCrystal_I2C(0x3F, 16, 2);

ButtonTTB* buttonLeft = new ButtonTTB(BUTTON_1); 
ButtonTTB* buttonModeSet = new ButtonTTB(BUTTON_2); 
ButtonTTB* buttonRight = new ButtonTTB(BUTTON_3); 

// DisplayLCD_Menu* display = new DisplayLCD_Menu(lcd, controlEngine, controlIndoor, buttonLeft, buttonModeSet, buttonRight, clockRTC);
MenuLCD1602* menu = new MenuLCD1602(lcd, controlEngine, controlIndoor, clockRTC);

int incomingByte = 0x30;

void setup()
{

  //  Debugging: Compatible Tools - avr-stub
  //  initialize GDB stub (avr-stub)
  // debug_init();

  //  Open hardware serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial)
  {
    // wait for hardware serial port to connect. Needed for Leonardo only
  }
  Serial.println("Indoor climat control heat - Ver.2");

  controlEngine->SetAction(ModeAction::Heat); // Режим - Нагрівання
  controlEngine->SetSwitchingDevice(ModeSwitchingDevice::TicTac); // Почергове включення нагрівачів
  controlIndoor->SetConditionPump(true); // Увімкнути насос
  controlIndoor->SetDelayPumpOn(4000);   // Затримка увимкненям нагрівачів після увімкниням насосу
  controlIndoor->SetDelayPumpOff(10000); // Затримка перед вимкненям насосу після повного нагріву кімнат

  lcd->init();
  lcd->backlight();
  // lcd->clear();

  menu->PrintMainMenu();
}

void loop()
{
  // controlEngine.Update();
  controlIndoor->Update();

  // display->Update();
  // menu->PrintMainMenu();
  // menu->PrintIndoorSubmenu();
  // menu->PrintEngineSubmenu();

  if (Serial.available() > 0)
  {
    // считываем входящий байт:
    incomingByte = Serial.read();
    lcd->clear();
    Serial.println(incomingByte, DEC);
  }

  if (incomingByte == 0x30)
  {
    menu->PrintMainMenu();
  }
  else if (incomingByte == 0x31)
  {
    menu->PrintIndoorSubmenu();
  }
  else if (incomingByte == 0x32)
  {
    menu->PrintEngineSubmenu();
  }

  /*
  bool pin_button_flag = buttonRight->keyDown();
  if (pin_button_flag)
  {
    toggle_led();
  }
    digitalWrite(CONTROL_PIN2, pin_button_flag);
  // if (buttonLeft->keyDown())
  //   toggle_led2();
  */
}
