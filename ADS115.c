/*
 * ADS115.c
 *
 *  Created on: 15-Jun-2024
 *      Author: DELL
 */

#include "ADS1115.h"
#include "rti.h"

void wasteClock(void) {
    rtiStartCounter(rtiCOUNTER_BLOCK1);

    // while the counter clock 1 is still running !
    // counter stopped by interrupt handler
    while(rtiREG1->GCTRL & (rtiCOUNTER_BLOCK1 << 1));

}

ADCStatus extADCInit(i2cBASE_t *i2cInterface, uint16_t configFlags) {
   ADCStatus status = Failure;
   uint8_t messageBuf[3] = {
       CONFIGREGADDR,
       ((configFlags & 0xFF00) >> 8),
       (configFlags & 0x00FF),
   };

   // Configure this once
   i2cSetSlaveAdd(i2cInterface, SLAVE_ADDR);

   if((status = extADCWrite(i2cInterface, 3U, messageBuf)) == Failure) {
       return status;
   }

   return Success;
}

ADCStatus extADCWrite(i2cBASE_t *i2cInterface, uint8_t len, uint8_t *msgBuf) {

   i2cSetDirection(i2cInterface, I2C_TRANSMITTER);

   i2cSetCount(i2cInterface, len);
   i2cSetMode(i2cInterface, I2C_MASTER);

   i2cSetStop(i2cInterface);
   i2cSetStart(i2cInterface);

   i2cSend(i2cInterface, len, msgBuf);

   /* Wait until Bus Busy is cleared */
   while(i2cIsBusBusy(i2cInterface) == true);

   /* Wait until Stop is detected */
   while(i2cIsStopDetected(i2cInterface) == 0);

   /* Clear the Stop condition */
   i2cClearSCD(i2cInterface);

   // Wait for some time
   wasteClock();

   return Success;
}

ADCStatus extADCRead(i2cBASE_t *i2cInterface, uint8_t len, uint8_t *msgBuf) {
    ADCStatus status = Failure;

    uint8_t messageBuf[1] = {
        CONVREGADDR,
    };

    if((status = extADCWrite(i2cInterface, 1U, messageBuf)) == Failure) {
        return status;
    }

    i2cSetDirection(i2cInterface, I2C_RECEIVER);

    i2cSetCount(i2cInterface, len);
    i2cSetMode(i2cInterface, I2C_MASTER);

    i2cSetStop(i2cInterface);
    i2cSetStart(i2cInterface);

    i2cReceive(i2cInterface, len, msgBuf);

    /* Wait until Bus Busy is cleared */
    while(i2cIsBusBusy(i2cInterface) == true);

    /* Wait until Stop is detected */
    while(i2cIsStopDetected(i2cInterface) == 0);

    /* Clear the Stop condition */
    i2cClearSCD(i2cInterface);

    // Wait for some time
    wasteClock();

    return Success;
}
ADCStatus extADCReadConvResult(i2cBASE_t *i2cInterface, uint8_t len, uint8_t *msgBuf) {
    return extADCRead(i2cInterface, len, msgBuf);
}
