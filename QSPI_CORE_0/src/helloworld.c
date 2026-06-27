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
#include "unistd.h"
#include "xil_io.h"
#include "xil_cache.h"
#include "xil_mmu.h"


int main()
{
//    init_platform();

	  xil_printf("\r\n bredore 1\n\r");
    // 1. Mark Core 1's DDR App region (0x20000000) as strongly-ordered/uncached
    Xil_SetTlbAttributes(0x20000000, 0x14de2);


    xil_printf("\r\n bredore 2\n\r");
    Xil_SetTlbAttributes(0xFFF00000, 0x14de2);


    xil_printf("\r\n bredore 3\n\r");
    // 3. Write Core 1's jump target address into the physical mailbox
    Xil_Out32(0xFFFFFFF0, 0x20000000);

    // 4. Double-protection: Flush the line just in case any early boot cache garbage remained
//    Xil_DCacheFlushRange(0xFFFFFFF0, 4);

    xil_printf("\r\n bredore 4\n\r");
    __asm__ volatile ("dsb");

    xil_printf("\r\n bredore 5\n\r");
//    __asm__ volatile ("sev");
    __asm__ volatile ("dmb\n\tdsb" : : : "memory");

    xil_printf("\r\n bredore 6\n\r");
    __asm__ volatile ("sev");


    int i=0;
    while(1)
    {
        xil_printf("\r\n CORE 0 is running %d\n\r",i++);
        sleep(1);
    }

    cleanup_platform();
    return 0;
}
