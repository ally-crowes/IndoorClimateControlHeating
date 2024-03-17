#include "b3380.h"

// Функция вычисляет значение температуры в десятых долях градусов Цельсия
// в зависимости от значения АЦП.
int16_t calculateNTC(temperature_table_entry_type adcValue) {
  temperature_table_index_type l = 0;
  temperature_table_index_type r = (sizeof(termo_table) / sizeof(termo_table[0])) - 1;
  temperature_table_entry_type tHigh = TEMPERATURE_TABLE_READ(r);
  temperature_table_entry_type tLow = TEMPERATURE_TABLE_READ(0);
  
  // Проверка выхода за пределы и граничных значений
  if (adcValue <= tHigh) {
    #ifdef TEMPERATURE_UNDER
      if (adcValue < tHigh) 
        return TEMPERATURE_UNDER;
    #endif
    return TEMPERATURE_TABLE_STEP * r + TEMPERATURE_TABLE_START;
  }
  if (adcValue >= tLow) {
    #ifdef TEMPERATURE_OVER
      if (adcValue > tLow)
        return TEMPERATURE_OVER;
    #endif
    return TEMPERATURE_TABLE_START;
  }

  // Двоичный поиск по таблице
  while ((r - l) > 1) {
    temperature_table_index_type m = (l + r) >> 1;
    temperature_table_entry_type mid = TEMPERATURE_TABLE_READ(m);
    if (adcValue > mid) {
      r = m;
    } else {
      l = m;
    }
  }
  temperature_table_entry_type vl = TEMPERATURE_TABLE_READ(l);
  if (adcValue >= vl) {
    return l * TEMPERATURE_TABLE_STEP + TEMPERATURE_TABLE_START;
  }
  temperature_table_entry_type vr = TEMPERATURE_TABLE_READ(r);
  temperature_table_entry_type vd = vl - vr;
  int16_t res = TEMPERATURE_TABLE_START + r * TEMPERATURE_TABLE_STEP; 
  if (vd) {
    // Линейная интерполяция
    res -= ((TEMPERATURE_TABLE_STEP * (int32_t)(adcValue - vr) + (vd >> 1)) / vd);
  }
  return res;
}