/*
 * PROGRAM NAME:  adc2tempC_NTCschA
 * VERSION:       1.0
 * FILENAME:      adc2tempC_NTCschA.h
 * FILE CREATED:  Nov 11, 2023
 * LAST MODIFIED: [2026-01-01]
 * DEVELOPED BY:  Andrey Dubrovin
 * DESCRIPTION:   Rthermistor = 10 kOm, B-3380
 *
 * FUNCTION LIST:
 *     adc2tempC_NTCschA()
 */

/*---------------------------------------------------------*/
/*  Global Defines                                         */
/*---------------------------------------------------------*/

#ifndef RESIST_UP
#define RESIST_UP 10000     // сопротивление постоянное R верхнего плеча делителя (Ом)
#endif /* RESIST_UP */

#ifndef RESIST_BASE
#define RESIST_BASE 10000   // сопротивление термистора Ro (Ом) при TEMP_BASE (градусах по Цельсию)
#endif /* RESIST_BASE */

#ifndef TEMP_BASE
#define TEMP_BASE 25        // температура (градусов Цельсия), при которой измерено сопротивление термистора RESIST_BASE
#endif /* TEMP_BASE */

/*---------------------------------------------------------*/
/*  Function Prototypes                                    */
/*---------------------------------------------------------*/

float adc2tempC_NTCschA(const int adcValue, const int bConstant);
