#include <Arduino.h>
#include "tSensorTypeNTC.h"

SensorTypeNTC::SensorTypeNTC(unsigned int pin) : _pin(pin)
{
    //this->_pin = pin;
    pinMode(_pin, INPUT);
}

float SensorTypeNTC::getTemperature() const
{
    // Average value
    int average = 0;
    for (byte i = 0; i < 10; i++)
        average += analogRead(_pin);
    average /= 10;

    // return adc2tempC_NTCschA(average, 3380);
    return adc2tempC_NTCschA(average, 3900);
}

/*
 *
 *    Vss -----
 *            |
 *            |
 *           | |
 *           | | R
 *           | |
 *            |        |
 *            *--------| pin ADC  
 *            |        |
 *           | |
 *           | | Rо
 *           | |
 *            |
 *            |
 *    GND ----*-------
 * 
 *         Sch. A
 * 
 */

/*
 * FUNCTION:   adc2tempC_NTCschA
 *
 * PARAMETERS: adcValue - value the analog read of pin,
 *             bConstant - B-Constant of Thermistor
 *
 * PURPOSE:    This function calculated value of temperature 
 *             bases of value adc pin and B-Constant
 *             
 * RETURN:     value of temperature in Celsius
 *
 * NOTES:      This function only for schematic A 
 *             (see file adc2temp_NTCschA.c, schematic - before)
 *
 * CHANGES:    
 *      2023-11-21: Andrii Dubrovin - Created
 *      2026-01-01: Andrii Dubrovin - Refactoring
 */
const float SensorTypeNTC::adc2tempC_NTCschA(const int adcValue, const int bConstant) const 
{
  float result;
  result = resistUp / ((float)1024 / adcValue - 1);  // R
  result /= resistBase;                              // R/Ro
  result = log(result) / bConstant;                  // 1/B * ln(R/Ro)
  result += (float)1.0 / (temperatureBase + 273.15); // + (1/To)
  result = (float)1.0 / result - 273.15;             // инвертируем и конвертируем в градусы по Цельсию
  return result;
}
