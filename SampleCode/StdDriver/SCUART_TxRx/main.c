/**************************************************************************//**
 * @file     main.c
 *
 * @brief    Demonstrate smartcard UART mode by connecting PG.4 and PG.5 pins.
 *
 *
 * @copyright (C) 2023 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include <stdio.h>
#include "NuMicro.h"

// This is the string we used in loopback demo
uint8_t au8TxBuf[] = "Hello World!";


/**
  * @brief  The interrupt services routine of smartcard port 0
  * @param  None
  * @retval None
  */
void SC1_IRQHandler(void)
{
    // Print SCUART received data to UART port
    // Data length here is short, so we're not care about UART FIFO over flow.
    while(!SCUART_GET_RX_EMPTY(SC1)) {
        sysputchar(SCUART_READ(SC1));
        fflush(stdout);
    }
    // RDA is the only interrupt enabled in this sample, this status bit
    // automatically cleared after Rx FIFO empty. So no need to clear interrupt
    // status here.

    return;
}

void SYS_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable IP clock */
    CLK_EnableModuleClock(SC1_MODULE);
    CLK_EnableModuleClock(UART0_MODULE);

    /* Select IP clock source */
    CLK_SetModuleClock(SC1_MODULE, CLK_CLKSEL4_SC1SEL_HXT, CLK_CLKDIV1_SC1(1));
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL2_UART0SEL_HXT, CLK_CLKDIV1_UART0(1));

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();

    /* Set PG.4 and PG.5 pin for SC UART mode */
    /* Smartcard CLK pin is used for TX, and DAT pin is used for Rx */
    SYS->GPG_MFPL |= SYS_GPG_MFPL_PG4MFP_SC1_CLK | SYS_GPG_MFPL_PG5MFP_SC1_DAT;

    /* Set multi-function pins for UART */
    SYS->GPE_MFPH &= ~(SYS_GPE_MFPH_PE14MFP_Msk | SYS_GPE_MFPH_PE15MFP_Msk);
    SYS->GPE_MFPH |= (SYS_GPE_MFPH_PE14MFP_UART0_TXD | SYS_GPE_MFPH_PE15MFP_UART0_RXD);

    /* Lock protected registers */
    SYS_LockReg();
}

int main(void)
{
    /* Init System, IP clock and multi-function I/O
       In the end of SYS_Init() will issue SYS_LockReg()
       to lock protected register. If user want to write
       protected register, please issue SYS_UnlockReg()
       to unlock protected register if necessary */
    SYS_Init();
    /* Init UART to 115200-8n1 for print message */
    UART_Open(UART0, 115200);
    sysprintf("This sample code demos smartcard interface UART mode\n");
    sysprintf("Please connect SC1 CLK pin(PG.4) with SC1 DAT pin(PG.5)\n");
    sysprintf("Hit any key to continue\n");
    sysgetchar();

    // Open smartcard interface 0 in UART mode. The line config will be 115200-8n1
    // Can call SCUART_SetLineConfig() later if necessary
    SCUART_Open(SC1, 115200);

    // Enable receive interrupt, no need to use other interrupts in this demo
    SCUART_ENABLE_INT(SC1, SC_INTEN_RDAIEN_Msk);
    IRQ_SetHandler((IRQn_ID_t)SC1_IRQn, SC1_IRQHandler);
    IRQ_Enable ((IRQn_ID_t)SC1_IRQn);

    // Send the demo string out from SC1_CLK pin
    // Received data from SC1_DAT pin will be print out to UART console
    SCUART_Write(SC1, au8TxBuf, sizeof(au8TxBuf));

    // Loop forever. There's no where to go without an operating system.
    while(1);
}




