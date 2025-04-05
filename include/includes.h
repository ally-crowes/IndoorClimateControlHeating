/*---------------------------------------------------------*/
/*  Global Variables                                       */
/*---------------------------------------------------------*/


/*---------------------------------------------------------*/
/*  Includes                                               */
/*---------------------------------------------------------*/

/* Must be called before for Arduino Header Files */
#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <avr/eeprom.h>
/* 
    Debugging: Compatible Tools - avr-stub
*/
// #include "avr8-stub.h"
// #include "app_api.h" // only needed with flash breakpoints


/*
 * Is intended for project specific (private) libraries
 */
//#include "b3380_1.h"
//#include "SensorTypeNTC.h"
//#include "Relay.h"
#include "ButtonTTB.h"

/*
 * Application Header Files, call after project headers
 */
#include "main.h"
#include "Controls\ControllerEngine.h"
#include "Controls\ControllerIndoor.h"
#include "Controls\ControllerPacket.h"
//#include "Display\DisplayLCD_Menu.h"
//#include "Display\MenuLCD1602.h"
//#include "Display\ControllerLCD1602.h"

/*
 * Map pins Nano
 */
#define NOTINNANO_1   0
#define NOTINNANO_2   1
#define BUTTON_1      2
#define BUTTON_2      3
#define BUTTON_3      4
#define RELAY_1       5
#define RELAY_2       6
#define RELAY_3       7
#define RELAY_4       8
#define SPI_CS        9
#define SPI_SS_NOTUSE 10
#define SPI_MOSI      11
#define SPI_MISO      12
#define SPI_CLK       13
#define SENSOR_1        A0
#define SENSOR_2        A1
#define SENSOR_3        A2
#define RESERVED      A3
#define I2C_SDA       A4
#define I2C_SCL       A5
#define NOTINUNO_1    A6
#define NOTINUNO_1    A6

/*
 * Map EEPROM Nano
 */
#define EEPROM_TemperatureEngineMin (float*)10
#define EEPROM_TemperatureEngineMax (float*)20
#define EEPROM_TemperatureIndoorMin (float*)30
#define EEPROM_TemperatureIndoorMax (float*)40


// I2C (TWI) - Led display: LCD1602_I2C-pcf8574
//             Real-time clock (RTC) : DS3231 + flash AT24C32
// SPI - CD card: HW-203