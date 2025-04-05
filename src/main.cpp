#include "includes.h"


/*---------------------------------------------------------*/
/*  Global Variables                                       */
/*---------------------------------------------------------*/
float minTemperatureEngine = eeprom_read_float(EEPROM_TemperatureEngineMin);// 29.6; 
float maxTemperatureEngine = eeprom_read_float(EEPROM_TemperatureEngineMax);// 30.6; 
float minTemperatureIndoor = eeprom_read_float(EEPROM_TemperatureIndoorMin);// 28.0; 
float maxTemperatureIndoor = eeprom_read_float(EEPROM_TemperatureIndoorMax);// 29.0; 

// Sensor
SensorTypeNTC* sensorEngine = new SensorTypeNTC(minTemperatureEngine, maxTemperatureEngine, SENSOR_1);
SensorTypeNTC* sensorIndoor = new SensorTypeNTC(minTemperatureIndoor, maxTemperatureIndoor, SENSOR_2);
// SensorTypeNTC* sensorEngine = new SensorTypeNTC(50, 58, SENSOR_1);
// SensorTypeNTC* sensorIndoor = new SensorTypeNTC(25.9, 26.6, SENSOR_2);

// Driver
Relay* relayA = new Relay(RELAY_1);
Relay* relayB = new Relay(RELAY_2);
// Relay* relayReserved = new Relay(RELAY_3);
Relay* relayPumb = new Relay(RELAY_4);

// Controller
//ControllerEngine* controlEngine = new ControllerEngine(sensorEngine, relayA, relayB);
ControllerEngine* controlEngine = new ControllerEngine(sensorEngine, relayA, relayB, ModeAction::Heat, ModeSwitchingDevice::TicTac, EEPROM_TemperatureEngineMin, EEPROM_TemperatureEngineMax);
// ControllerIndoor* controlIndoor = new ControllerIndoor(sensorIndoor, controlEngine);
// ControllerIndoor* controlIndoor = new ControllerIndoor(sensorIndoor, controlEngine, relayPumb);
ControllerIndoor* controlIndoor = new ControllerIndoor(sensorIndoor, controlEngine, relayPumb, EEPROM_TemperatureIndoorMin, EEPROM_TemperatureIndoorMax);

// Clock
DS3231* clockRTC = new DS3231();

// Button
ButtonTTB* buttonLeft = new ButtonTTB(BUTTON_1); 
ButtonTTB* buttonModeSet = new ButtonTTB(BUTTON_2); 
ButtonTTB* buttonRight = new ButtonTTB(BUTTON_3); 

// Display - LED
LiquidCrystal_I2C* lcd = new LiquidCrystal_I2C(0x3F, 16, 2);
MenuLCD1602* menu = new MenuLCD1602(lcd, controlEngine, controlIndoor, clockRTC);
ControllerPacket* controlLed = new ControllerPacket(menu, controlEngine, controlIndoor);

void setup()
{
  // eeprom_write_float(10, 29.6); // ;
  // eeprom_write_float(20, 30.6); // ;
  // eeprom_write_float(30, 28.0); // ;
  // eeprom_write_float(40, 29.0); // ;

  //  Debugging: Compatible Tools - avr-stub
  //  initialize GDB stub (avr-stub)
  // debug_init();

  //  Open hardware serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial)
  {
    // wait for hardware serial port to connect. Needed for Leonardo only
  }
  Serial.println("Indoor climat control heat - Ver.3.1");

  controlEngine->SetAction(ModeAction::Heat); // Режим - Нагрівання
  controlEngine->SetSwitchingDevice(ModeSwitchingDevice::TicTac); // Почергове включення нагрівачів
  controlIndoor->SetConditionPump(true); // Увімкнути насос
  controlIndoor->SetDelayPumpOn(4000);   // Затримка увимкненям нагрівачів після увімкниням насосу
  controlIndoor->SetDelayPumpOff(10000); // Затримка перед вимкненям насосу після повного нагріву кімнат

  lcd->init();
  lcd->backlight();
  // lcd->clear();

  menu->initialize();
}

void loop()
{
  int incomingByte = 0x30;

  // controlEngine.Update();
  controlIndoor->Update();

  // display->Update();
  // menu->printMainMenu();
  // menu->printIndoorSubmenu();
  // menu->printEngineSubmenu();

  if (Serial.available() > 0)
  {
    // считываем входящий байт:
    incomingByte = Serial.read();
    Serial.println(incomingByte, DEC);
    controlLed->update(incomingByte);
  }
  else if (buttonLeft->keyDown())
  {
    controlLed->update(0x2D);
  }
  else if (buttonRight->keyDown())
  {
    controlLed->update(0x2B);
  }
  else if (buttonModeSet->keyDown())
  {
    controlLed->update(0x73);
  }
  else
  {
    controlLed->update();
  }

  // if (incomingByte == 0x30)
  // {
  //   menu->printMainMenu();
  // }
  // else if (incomingByte == 0x33)
  // {
  //   menu->printMainMenu(1);
  // }
  // else if (incomingByte == 0x34)
  // {
  //   menu->printMainMenu(2);
  // }

  // else if (incomingByte == 0x31)
  // {
  //   menu->printIndoorSubmenu(0);
  // }
  // else if (incomingByte == 0x32)
  // {
  //   menu->printEngineSubmenu(0);
  // }

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

