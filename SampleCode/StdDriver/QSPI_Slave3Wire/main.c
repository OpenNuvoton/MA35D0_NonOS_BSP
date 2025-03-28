/**************************************************************************//**
 * @file     main.c
 * @brief    Configure QSPI0 as Slave 3 wire mode and demonstrate how to
 *           communicate with an off-chip SPI Master device with FIFO mode.
 *           This sample code needs to work with SPI_MasterFifoMode sample code.
 *
 *
 * @copyright (C) 2023 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "NuMicro.h"

#define TEST_COUNT 16

#define QSPI_PORT  QSPI0

uint32_t g_au32SourceData[TEST_COUNT];
uint32_t g_au32DestinationData[TEST_COUNT];
volatile uint32_t g_u32TxDataCount;
volatile uint32_t g_u32RxDataCount;

void SYS_Init(void)
{
    /* Enable UART module clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Select UART module clock source as SYSCLK1 and UART module clock divider as 15 */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL2_UART0SEL_SYSCLK1_DIV2, CLK_CLKDIV1_UART0(15));

    /* Select PCLK0 as the clock source of QSPI0 */
    CLK_SetModuleClock(QSPI0_MODULE, CLK_CLKSEL4_QSPI0SEL_PCLK0, MODULE_NoMsk);

    /* Enable QSPI0 peripheral clock */
    CLK_EnableModuleClock(QSPI0_MODULE);

    /* Set GPE multi-function pins for UART0 RXD and TXD */
    SYS->GPE_MFPH &= ~(SYS_GPE_MFPH_PE14MFP_Msk | SYS_GPE_MFPH_PE15MFP_Msk);
    SYS->GPE_MFPH |= (SYS_GPE_MFPH_PE14MFP_UART0_TXD | SYS_GPE_MFPH_PE15MFP_UART0_RXD);

    /* Setup QSPI0 multi-function pins */
    SYS->GPD_MFPL &= ~(SYS_GPD_MFPL_PD1MFP_Msk | SYS_GPD_MFPL_PD2MFP_Msk | SYS_GPD_MFPL_PD3MFP_Msk);
    SYS->GPD_MFPL |= (SYS_GPD_MFPL_PD1MFP_QSPI0_CLK | SYS_GPD_MFPL_PD2MFP_QSPI0_MOSI0 | SYS_GPD_MFPL_PD3MFP_QSPI0_MISO0);

}

void QSPI_Init(void)
{

    /* Configure as a slave, clock idle low, 32-bit transaction, drive output on falling clock edge and latch input on rising edge. */
    /* Configure QSPI0 as a low level active device. */
    QSPI_Open(QSPI_PORT, SPI_SLAVE, SPI_MODE_0, 32, ptr_to_u32(NULL));

    /* Enable slave 3 wire mode */
    QSPI_PORT->SSCTL |= QSPI_SSCTL_SLV3WIRE_Msk;
}

int main(void)
{
    volatile uint32_t u32TxDataCount, u32RxDataCount;

    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, IP clock and multi-function I/O. */
    SYS_Init();

    /* Configure UART0: 115200, 8-bit word, no parity bit, 1 stop bit. */
    UART_Open(UART0, 115200);

    /* Init QSPI */
    QSPI_Init();

    sysprintf("\n\n");
    sysprintf("+----------------------------------------------------------------------+\n");
    sysprintf("|           QSPI0 Slave 3 Wire Mode Sample Code                        |\n");
    sysprintf("+----------------------------------------------------------------------+\n");
    sysprintf("\n");
    sysprintf("Configure QSPI0 as a slave.\n");
    sysprintf("Bit length of a transaction: 32\n");
    sysprintf("The I/O connection for QSPI0:\n");
    sysprintf("    QSPI0_CLK(PD1)\n    QSPI0_MISO(PD2)\n    QSPI0_MOSI(PD3)\n\n");
    sysprintf("SPI controller will enable FIFO mode and transfer %d data to a off-chip master device.\n", TEST_COUNT);
    sysprintf("In the meanwhile the SPI controller will receive %d data from the off-chip master device.\n", TEST_COUNT);
    sysprintf("After the transfer is done, the %d received data will be printed out.\n", TEST_COUNT);

    for(u32TxDataCount = 0; u32TxDataCount < TEST_COUNT; u32TxDataCount++)
    {
        /* Write the initial value to source buffer */
        g_au32SourceData[u32TxDataCount] = 0x00AA0000 + u32TxDataCount;
        /* Clear destination buffer */
        g_au32DestinationData[u32TxDataCount] = 0;
    }

    u32TxDataCount = 0;
    u32RxDataCount = 0;
    sysprintf("Press any key if the master device configuration is ready.\n");
    sysgetchar();
    sysprintf("\n");

    /* Set TX FIFO threshold and enable FIFO mode. */
    QSPI_SetFIFO(QSPI_PORT, 4, 4);

    /* Access TX and RX FIFO */
    while(u32RxDataCount < TEST_COUNT)
    {
        /* Check TX FULL flag and TX data count */
        if( (QSPI_GET_TX_FIFO_FULL_FLAG(QSPI_PORT) == 0) && (u32TxDataCount < TEST_COUNT) )
        {
            QSPI_WRITE_TX(QSPI_PORT, g_au32SourceData[u32TxDataCount++]); /* Write to TX FIFO */
        }

        /* Check RX EMPTY flag */
        if(QSPI_GET_RX_FIFO_EMPTY_FLAG(QSPI_PORT) == 0)
        {
            g_au32DestinationData[u32RxDataCount++] = QSPI_READ_RX(QSPI_PORT); /* Read RX FIFO */
        }
    }

    /* Print the received data */
    sysprintf("Received data:\n");
    for(u32RxDataCount = 0; u32RxDataCount < TEST_COUNT; u32RxDataCount++)
    {
        sysprintf("%d:\t0x%X\n", u32RxDataCount, g_au32DestinationData[u32RxDataCount]);
    }
    sysprintf("The data transfer was done.\n");

    sysprintf("\n\nExit QSPI driver sample code.\n");

    /* Reset QSPI0 */
    QSPI_Close(QSPI_PORT);

    while(1);
}


