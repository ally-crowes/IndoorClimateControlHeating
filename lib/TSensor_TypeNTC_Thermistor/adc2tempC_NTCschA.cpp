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

// #include <avr/io.h>
// #include <avr/pgmspace.h>
// #include <inttypes.h>
#include <Arduino.h>
#include "adc2tempC_NTCschA.h"

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
float adc2tempC_NTCschA(const int adcValue, const int bConstant) {
  float result;
  result = RESIST_UP / ((float)1024 / adcValue - 1);
  result /= RESIST_BASE;                             // (R/Ro)
  result = log(result) / bConstant;                  // 1/B * ln(R/Ro)
  result += (float)1.0 / (TEMP_BASE + 273.15);       // + (1/To)
  result = (float)1.0 / result - 273.15;             // инвертируем и конвертируем в градусы по Цельсию
  return result;
}
