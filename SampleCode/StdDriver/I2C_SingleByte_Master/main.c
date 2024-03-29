/**************************************************************************//**
 * @file     main.c
 *
 * @brief
 *           Show how to use I2C single byte API Read and write data to slave
 *           needs to work with I2C_Slave sample code.
 * @copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#include <stdio.h>
#include "NuMicro.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
volatile uint8_t g_u8DeviceAddr;

void SYS_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable HXT */
    CLK->PWRCTL |= CLK_PWRCTL_HXTEN_Msk;

    /* Waiting clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

    /* Enable IP clock */
    CLK_EnableModuleClock(I2C4_MODULE);
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL2_UART0SEL_HXT, CLK_CLKDIV1_UART0(2));
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_EnableModuleClock(GPM_MODULE);

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set GPE multi-function pins for UART0 RXD and TXD */
    SYS->GPE_MFPH &= ~(SYS_GPE_MFPH_PE14MFP_Msk | SYS_GPE_MFPH_PE15MFP_Msk);
    SYS->GPE_MFPH |= (SYS_GPE_MFPH_PE14MFP_UART0_TXD | SYS_GPE_MFPH_PE15MFP_UART0_RXD);
    /* Set multi-function pins for I2C4 */
    SYS->GPM_MFPL &= ~(SYS_GPM_MFPL_PM0MFP_Msk | SYS_GPM_MFPL_PM1MFP_Msk);
    SYS->GPM_MFPL |= SYS_GPM_MFPL_PM0MFP_I2C4_SDA | SYS_GPM_MFPL_PM1MFP_I2C4_SCL;

    /* I2C pin enable schmitt trigger */
    PM->SMTEN |= GPIO_SMTEN_SMTEN0_Msk | GPIO_SMTEN_SMTEN1_Msk;

    /* Lock protected registers */
    SYS_LockReg();
}

void I2C4_Init(void)
{
    /* Open I2C module and set bus clock */
    I2C_Open(I2C4, 100000);

    /* Get I2C4 Bus Clock */
    sysprintf("I2C clock %d Hz\n", I2C_GetBusClockFreq(I2C4));

    /* Set I2C 4 Slave Addresses */
    I2C_SetSlaveAddr(I2C4, 0, 0x15, 0);   /* Slave Address : 0x15 */
    I2C_SetSlaveAddr(I2C4, 1, 0x35, 0);   /* Slave Address : 0x35 */
    I2C_SetSlaveAddr(I2C4, 2, 0x55, 0);   /* Slave Address : 0x55 */
    I2C_SetSlaveAddr(I2C4, 3, 0x75, 0);   /* Slave Address : 0x75 */
}

void I2C4_Close(void)
{
    /* Disable I2C4 interrupt and clear corresponding GIC bit */
    I2C_DisableInt(I2C4);
    GIC_DisableIRQ(I2C4_IRQn);

    /* Disable I2C0 and close I2C0 clock */
    I2C_Close(I2C4);
    CLK_DisableModuleClock(I2C4_MODULE);

}


int32_t main(void)
{
    uint32_t i;
    uint8_t u8data, u8tmp, err;

    /* Init System, IP clock and multi-function I/O. */
    SYS_Init();

    /* Configure UART: 115200, 8-bit word, no parity bit, 1 stop bit. */
    UART_Open(UART0, 115200);

    /*
        This sample code sets I2C bus clock to 100kHz. Then, Master accesses Slave with Byte Write
        and Byte Read operations, and check if the read data is equal to the programmed data.
    */
    sysprintf("+--------------------------------------------------------+\n");
    sysprintf("| I2C Driver Sample Code for Single Byte Read/Write Test |\n");
    sysprintf("| Needs to work with I2C_Slave sample code               |\n");
    sysprintf("|                                                        |\n");
    sysprintf("| I2C Master (I2C4) <---> I2C Slave(I2C4)                |\n");
    sysprintf("| !! This sample code requires two borads to test !!     |\n");
    sysprintf("+--------------------------------------------------------+\n");

    sysprintf("\n");

    /* Init I2C4 */
    I2C4_Init();

    /* Slave Address */
    g_u8DeviceAddr = 0x15;

    err = 0;

    for(i = 0; i < 256; i++)
    {
        u8tmp = (uint8_t)i + 3;

        /* Single Byte Write (Two Registers) */
        while(I2C_WriteByteTwoRegs(I2C4, g_u8DeviceAddr, i, u8tmp));

        /* Single Byte Read (Two Registers) */
        u8data = I2C_ReadByteTwoRegs(I2C4, g_u8DeviceAddr, i);
        if(u8data != u8tmp)
        {
            err = 1;
            sysprintf("%03d: Single byte write data fail,  W(0x%X)/R(0x%X) \n", i, u8tmp, u8data);
        }
    }

    sysprintf("\n");

    if(err)
        sysprintf("Single byte Read/Write access Fail.....\n");
    else
        sysprintf("Single byte Read/Write access Pass.....\n");

    while(1);
}



