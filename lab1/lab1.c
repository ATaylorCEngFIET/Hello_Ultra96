/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"

#include "xgpiops.h"
#include "xbram.h"


#define mio_led0 20
#define mio_led1 19
#define mio_led2 18
#define mio_led3 17

#define word_1 0x4841434b
#define word_2 0x53544552
#define word_3 0x20554c54
#define word_4 0x52413936
#define word_5 0x20545554
#define word_6 0x4f524941
#define word_7 0x0000004c


XGpioPs Gpio;
XBram   Bram;
int main()
{

	int Status,i;
	XGpioPs_Config *GPIOConfigPtr;
	XBram_Config *BRAMConfigPtr;
	u32 secret_word[8];

    init_platform();

    BRAMConfigPtr = XBram_LookupConfig(XPAR_BRAM_0_DEVICE_ID);

	Status = XBram_CfgInitialize(&Bram, BRAMConfigPtr,BRAMConfigPtr->CtrlBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	for(i=0;i<8;i++){
		secret_word[i] = XBram_ReadReg(XPAR_BRAM_0_BASEADDR, i*4);
	}

    GPIOConfigPtr = XGpioPs_LookupConfig(XPAR_XGPIOPS_0_DEVICE_ID);

    Status = XGpioPs_CfgInitialize(&Gpio, GPIOConfigPtr, GPIOConfigPtr ->BaseAddr);

    if (Status != XST_SUCCESS) {

    return XST_FAILURE;

    }

    XGpioPs_SetDirectionPin(&Gpio, mio_led0, 1);
    XGpioPs_SetOutputEnablePin(&Gpio, mio_led0, 1);
    XGpioPs_SetDirectionPin(&Gpio, mio_led1, 1);
    XGpioPs_SetOutputEnablePin(&Gpio, mio_led1, 1);
    XGpioPs_SetDirectionPin(&Gpio, mio_led2, 1);
    XGpioPs_SetOutputEnablePin(&Gpio, mio_led2, 1);
    XGpioPs_SetDirectionPin(&Gpio, mio_led3, 1);
    XGpioPs_SetOutputEnablePin(&Gpio, mio_led3, 1);
    while(1){

        if( (secret_word[1]==word_1) && (secret_word[2]==word_2) && (secret_word[3]==word_3)
            && (secret_word[4]==word_4) && (secret_word[5]==word_5) && (secret_word[6]==word_6)
    		&& (secret_word[7]==word_7))
        {
			XGpioPs_WritePin(&Gpio, mio_led0, 0x0);
			XGpioPs_WritePin(&Gpio, mio_led1, 0x1);
			XGpioPs_WritePin(&Gpio, mio_led2, 0x0);
			XGpioPs_WritePin(&Gpio, mio_led3, 0x1);
			usleep(1000000);
			XGpioPs_WritePin(&Gpio, mio_led0, 0x1);
			XGpioPs_WritePin(&Gpio, mio_led1, 0x0);
			XGpioPs_WritePin(&Gpio, mio_led2, 0x1);
			XGpioPs_WritePin(&Gpio, mio_led3, 0x0);
			usleep(1000000);
        }
        else
        {
			XGpioPs_WritePin(&Gpio, mio_led0, 0x1);
			XGpioPs_WritePin(&Gpio, mio_led1, 0x1);
			XGpioPs_WritePin(&Gpio, mio_led2, 0x1);
			XGpioPs_WritePin(&Gpio, mio_led3, 0x1);
			usleep(1000000);
			XGpioPs_WritePin(&Gpio, mio_led0, 0x0);
			XGpioPs_WritePin(&Gpio, mio_led1, 0x0);
			XGpioPs_WritePin(&Gpio, mio_led2, 0x0);
			XGpioPs_WritePin(&Gpio, mio_led3, 0x0);
			usleep(1000000);
        }



    }

    cleanup_platform();
    return 0;
}
