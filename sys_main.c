/** @file sys_main.c 
*   @brief Application main file
*   @date 11-Dec-2018
*   @version 04.07.01
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com 
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
#include "gio.h"
#include "i2c.h"
#include "rti.h"
#include "sys_vim.h"
#include "sci.h"
#include "sys_dma.h"
#include "ADS1115.h"
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
void rtiNotification(uint32 notification)
{
    // clear bit
    rtiREG1->INTFLAG |= rtiNOTIFICATION_COMPARE0;
    rtiStopCounter(rtiCOUNTER_BLOCK1);
}
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    float finalResult = 0.0;

    _enable_IRQ();

    vimInit();
    gioInit();
    i2cInit();
    rtiInit();
    sciInit();

    rtiEnableNotification(rtiNOTIFICATION_COMPARE0);


    // DMA Configuration
    /*
    i2cEnableNotification(i2cREG1, I2C_RX_INT);

    // enable TX DMA
    scilinREG->SETINT |= (1U << 16);
    // Enable Tx INT
    scilinREG->SETINT |= (1U << 8);

    // configure DMA Channel 10 I2C Recieve Hardware Request
    g_dmaCTRL ch10CtrlPkt = {0};

    ch10CtrlPkt.SADD = (uint32_t)(&i2cREG1->DRR); // Source Address
    ch10CtrlPkt.DADD = (uint32_t)(&scilinREG->TD); // Dest Address
    ch10CtrlPkt.FRCNT = 1; // Frame Count
    ch10CtrlPkt.ELCNT = 1; // Block Count
    ch10CtrlPkt.RDSIZE = sizeof(uint8_t); // size of I2C Recv Reg
    ch10CtrlPkt.WRSIZE = sizeof(uint32_t); // size of SCI Trans Reg
    ch10CtrlPkt.TTYPE = FRAME_TRANSFER; // every DMA Req causes Frame Transfer
    ch10CtrlPkt.ADDMODERD = ADDR_FIXED; // Read addr fixed
    ch10CtrlPkt.ADDMODEWR = ADDR_FIXED; // write addr fixed
    ch10CtrlPkt.AUTOINIT = AUTOINIT_OFF; // Source Address
    ch10CtrlPkt.CHCTRL = 0U; // no channel needs to be triggered after this


    ch10CtrlPkt.ELDOFFSET = 0; // Source Address
    ch10CtrlPkt.ELSOFFSET = 0; // Source Address
    ch10CtrlPkt.FRDOFFSET = 0; // Source Address
    ch10CtrlPkt.FRSOFFSET = 0; // Source Address

    dmaSetPriority(DMA_CH10, HIGHPRIORITY);
    dmaSetCtrlPacket(DMA_CH10, ch10CtrlPkt);
    dmaSetChEnable(DMA_CH10, DMA_HW);
    dmaSetChEnable(DMA_CH10, DMA_SW);
    dmaEnable();
    */

    uint8_t convResult[2] = {0};
    ADCStatus status = Failure;
    uint32_t result = 0;

    // Initialize ADC with desired Configuration
    if((status = extADCInit(i2cREG1, (SINGLESHOTCONV | INPMULSINGLEAIN1 | OPRMODESINGLE))) == Failure) {
        // TODO: send failure status over UART
        return 1;
    }

    while(1) {
        // block mode, read ADC conversion result
        if((status = extADCReadConvResult(i2cREG1, 2U, convResult)) == Failure) {
            // TODO: send failure status over UART
            break;
        }
        result = convResult[0] << 8;
        result |= convResult[1];

        finalResult = result * (6.144 * 2) / 65536;

        // keep compiler happy :)
        (void)finalResult;
        (void)status;

        sciSend(scilinREG, 2U, convResult);
    }

/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
/* USER CODE END */
