#include <avr/io.h>
#include <avr/pgmspace.h>
#include <inttypes.h>

/*
 * PROGRAM NAME:  CalcNTCTemperature
 *
 * VERSION:       1.0
 *
 * FILENAME:      CalcNTCTemperature.h
 *
 * FILE CREATED:  Nov 11, 2023
 *
 * LAST MODIFIED: [28-11-23]
 *
 * DEVELOPED BY:  Andrey Dubrovin
 *                
 * DESCRIPTION:   Rthermistor = 10 kOm, B-3380
 *
 * FUNCTION LIST:
 *
 */

/*---------------------------------------------------------*/
/*  Global Defines                                         */
/*---------------------------------------------------------*/

#define RESIST_UP 10000     // сопротивление постоянное R верхнего плеча делителя (Ом)
#define RESIST_BASE 10000   // сопротивление термистора Ro при TEMP_BASE градусах по Цельсию (Ом)
#define TEMP_BASE 25        // температура, при которой измерено RESIST_BASE (градусов Цельсия)

/*---------------------------------------------------------*/
/*  Function Protos                                        */
/*---------------------------------------------------------*/

float getCalcNTCTemperature(int adcValue, int BETA_COEF);
