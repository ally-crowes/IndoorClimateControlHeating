#include "CalcNTCTemperature.h"
#include <Arduino.h>

float getCalcNTCTemperature(int adcValue, int BETA_COEF) {
  float result;
  result = RESIST_UP / ((float)1024 / adcValue - 1);
  result /= RESIST_BASE;                        // (R/Ro)
  result = log(result) / BETA_COEF;            // 1/B * ln(R/Ro)
  result += (float)1.0 / (TEMP_BASE + 273.15);  // + (1/To)
  result = (float)1.0 / result - 273.15;    // инвертируем и конвертируем в градусы по Цельсию
  return result;
}