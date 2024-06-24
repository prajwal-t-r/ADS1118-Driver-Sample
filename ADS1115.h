/*
 * ADS1115.h
 *
 *  Created on: 15-Jun-2024
 *      Author: DELL
 */

#ifndef ADS1115_H_
#define ADS1115_H_

#include "i2c.h"

typedef enum {
    Success,
    Failure
} ADCStatus;

//typedef Status ADCStatus;

#define SINGLESHOTCONV          (1U << 15)
#define INPMULSINGLEAIN1        (1U << 14)
#define OPRMODESINGLE           (1U << 8)
#define PGADEFAULT              (1U << 10)
#define SLAVE_ADDR 0b1001000


#define CONFIGREGADDR           (1U << 0)
#define CONVREGADDR             (0U)

ADCStatus extADCInit(i2cBASE_t *i2cInterface, uint16_t configFlags);
ADCStatus extADCWrite(i2cBASE_t *i2cInterface, uint8_t len, uint8_t *msgBuf);
ADCStatus extADCRead(i2cBASE_t *i2cInterface, uint8_t len, uint8_t *msgBuf);
ADCStatus extADCReadConvResult(i2cBASE_t *i2cInterface, uint8_t len, uint8_t *msgBuf);

void wasteClock(void);

#endif /* ADS1115_H_ */
