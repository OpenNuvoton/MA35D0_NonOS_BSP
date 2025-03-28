/**************************************************************************//**
 * @file     main.c
 *
 * @brief    Demonstrate I2C PDMA mode and need to connect I2C5(Master) and I2C4(Slave).
 *
 * @copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "NuMicro.h"

#define PDMA                   ((PDMA_T *)  PDMA2_BASE)

#define I2C5_PDMA_TX_CH     0
#define I2C4_PDMA_RX_CH     1
#define I2C5_PDMA_RX_CH     2
#define I2C4_PDMA_TX_CH     3
#define PDMA_TEST_LENGTH    5

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
static uint8_t g_u8MasterTx_Buffer[PDMA_TEST_LENGTH] __attribute__((aligned(512)));
static uint8_t g_u8MasterRx_Buffer[PDMA_TEST_LENGTH] __attribute__((aligned(512)));
static uint8_t g_u8SlaveTx_Buffer[PDMA_TEST_LENGTH] __attribute__((aligned(512)));
static uint8_t g_u8SlaveRx_Buffer[PDMA_TEST_LENGTH] __attribute__((aligned(512)));
volatile uint32_t PDMA_DONE = 0;

volatile uint8_t g_u8DeviceAddr = 0x16;
volatile uint8_t g_u8EndFlag = 0;
volatile uint8_t g_u8MasterDataLen = 0;
volatile uint8_t g_u8SlaveDataLen = 0;
volatile uint16_t g_u8SlaveBufferAddr = 0;

typedef void (*I2C_FUNC)(uint32_t u32Status);

static I2C_FUNC s_I2C5HandlerFn = NULL;
static I2C_FUNC s_I2C4HandlerFn = NULL;


void PDMA_IRQHandler(void)
{
    uint32_t u32Status = PDMA->TDSTS;

    //Master TX
    if(u32Status & (0x1 << I2C5_PDMA_TX_CH))
    {
        sysprintf("\n I2C5 Tx done  ");
        PDMA->TDSTS = 0x1;
    }

    //Master RX
    if (u32Status & (0x1 << I2C5_PDMA_RX_CH))
    {
        sysprintf("\n I2C5 Rx done  ");
        PDMA->TDSTS = 0x1 << I2C5_PDMA_RX_CH;
        PDMA_DONE = 1;
    }

    //Slave RX
    if (u32Status & (0x1 << I2C4_PDMA_RX_CH))
    {
        sysprintf("\n I2C4 Rx done  ");
        PDMA->TDSTS = 0x1 << I2C4_PDMA_RX_CH;
        PDMA_DONE = 1;
    }

    //Slave TX
    if (u32Status & (0x1 << I2C4_PDMA_TX_CH))
    {
        sysprintf("\n I2C4 Tx done  ");
        PDMA->TDSTS = 0x1 << I2C4_PDMA_TX_CH;
    }
}


void I2C5_IRQHandler(void)
{
    uint32_t u32Status;

    u32Status = I2C_GET_STATUS(I2C5);

    if (I2C_GET_TIMEOUT_FLAG(I2C5))
    {
        /* Clear I2C5 Timeout Flag */
        I2C_ClearTimeoutFlag(I2C5);
    }
    else
    {
        if (s_I2C5HandlerFn != NULL)
            s_I2C5HandlerFn(u32Status);
    }
}


void I2C4_IRQHandler(void)
{
    uint32_t u32Status;

    u32Status = I2C_GET_STATUS(I2C4);

    if (I2C_GET_TIMEOUT_FLAG(I2C4))
    {
        /* Clear I2C5 Timeout Flag */
        I2C_ClearTimeoutFlag(I2C4);
    }
    else
    {
        if (s_I2C4HandlerFn != NULL)
            s_I2C4HandlerFn(u32Status);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C4 Slave TRx Callback Function                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_PDMA_SlaveRx(uint32_t u32Status)
{
    if(u32Status == 0x60)                       /* Own SLA+W has been receive; ACK has been return */
    {
        /*
            Note:
            During PDMA operation, I2C controller will not occur receive Address ACK interrupt
        */
    }
    else if(u32Status == 0x80)                 /* Previously address with own SLA address
                                                  Data has been received; ACK has been returned*/
    {
        /*
            Note:
            During PDMA operation, I2C controller will not occur receive Data ACK interrupt
        */
    }
    else if(u32Status == 0x88)                 /* Previously addressed with own SLA address; NOT ACK has
                                                   been returned */
    {
        I2C_SET_CONTROL_REG(I2C4, I2C_CTL_SI_AA);
    }
    else if(u32Status == 0xA0)                 /* A STOP or repeated START has been received while still
                                                   addressed as Slave/Receiver*/
    {
        I2C_SET_CONTROL_REG(I2C4, I2C_CTL_SI_AA);
    }
    else
    {
        /* TO DO */
        sysprintf("Status 0x%x is NOT processed\n", u32Status);
        while(1);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C4 PDMA Slave Tx Callback Function                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_PDMA_SlaveTx(uint32_t u32Status)
{
    uint8_t u8data;

    if (u32Status == 0x60)                       /* Own SLA+W has been receive; ACK has been return */
    {
        g_u8SlaveDataLen = 0;
        I2C_SET_CONTROL_REG(I2C4, I2C_CTL_SI_AA);
    }
    else if (u32Status == 0x80)                 /* Previously address with own SLA address
                                                   Data has been received; ACK has been returned*/
    {
        u8data = (unsigned char) I2C_GET_DATA(I2C4);
        g_u8SlaveRx_Buffer[g_u8SlaveDataLen++] = u8data;
        g_u8SlaveBufferAddr = (g_u8SlaveRx_Buffer[0] << 8) + g_u8SlaveRx_Buffer[1];

        if(g_u8SlaveDataLen == 2)
        {
            I2C_SET_CONTROL_REG(I2C4, I2C_CTL_SI);
        }
        else
        {
            I2C_SET_CONTROL_REG(I2C4, I2C_CTL_SI_AA);
        }
    }
    else if (u32Status == 0x88)                 /* Previously addressed with own SLA address; NOT ACK has
                                                   been returned */
    {
        I2C_SET_CONTROL_REG(I2C4, I2C_CTL_SI_AA);
    }
    else if (u32Status == 0xA0)                 /* A STOP or repeated START has been received while still
                                                   addressed as Slave/Receiver*/
    {
        I2C_SET_CONTROL_REG(I2C4, I2C_CTL_SI_AA);

        /* Enable I2C4 Slave TX */
        I2C4->CTL1 = I2C_CTL1_TXPDMAEN_Msk;
    }
    else if (u32Status == 0xA8)                  /* Own SLA+R has been receive; ACK has been return */
    {
        /*
           Note:
           During PDMA operation, I2C controller will not occur START interrupt
        */
    }
    else if (u32Status == 0xB8)                  /* Data byte in I2CDAT has been transmitted ACK has been received */
    {
        /*
           Note:
           During PDMA operation, I2C controller will not occur START interrupt
        */
    }
    else if (u32Status == 0xC0)                 /* Data byte or last data in I2CDAT has been transmitted
                                                   Not ACK has been received */
    {
        I2C_SET_CONTROL_REG(I2C4, I2C_CTL_SI_AA);
    }
    else if (u32Status == 0x88)                 /* Previously addressed with own SLA address; NOT ACK has
                                                   been returned */
    {
        I2C_SET_CONTROL_REG(I2C4, I2C_CTL_SI_AA);
    }
    else
    {
        /* TO DO */
        sysprintf("Status 0x%x is NOT processed\n", u32Status);

        while (1);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C5 PDMA Master Tx Callback Function                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_PDMA_MasterTx(uint32_t u32Status)
{
    if(u32Status == 0x08)                       /* START has been transmitted */
    {
        /*
           Note:
           During PDMA operation, I2C controller will not occur START interrupt
        */
    }
    else if(u32Status == 0x10)                  /* Repeat START has been transmitted */
    {

    }
    else if(u32Status == 0x18)                  /* SLA+W has been transmitted and ACK has been received */
    {
        /*
           Note:
           During PDMA operation, I2C controller will not occur address ACK interrupt
        */
    }
    else if(u32Status == 0x20)                  /* SLA+W has been transmitted and NACK has been received */
    {
        I2C_STOP(I2C5);
        I2C_START(I2C5);
    }
    else if(u32Status == 0x28)                  /* DATA has been transmitted and ACK has been received */
    {
        /*
           Note:
           During PDMA operation, I2C controller will not occur data ACK interrupt
        */
    }
    else
    {
        /* TO DO */
        sysprintf("Status 0x%x is NOT processed\n", u32Status);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C4 PDMA Master Rx Callback Function                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_PDMA_MasterRx(uint32_t u32Status)
{
    if(u32Status == 0x08)                          /* START has been transmitted and prepare SLA+W */
    {
        I2C_SET_DATA(I2C5, (g_u8DeviceAddr << 1) | 0x00);     /* Write SLA+W to Register I2CDAT */
        I2C_SET_CONTROL_REG(I2C5, I2C_CTL_SI);
    }
    else if(u32Status == 0x18)                     /* SLA+W has been transmitted and ACK has been received */
    {
        g_u8MasterDataLen = 1;
        I2C_SET_DATA(I2C5, g_u8MasterTx_Buffer[g_u8MasterDataLen++]);
        I2C_SET_CONTROL_REG(I2C5, I2C_CTL_SI);
    }
    else if(u32Status == 0x20)                     /* SLA+W has been transmitted and NACK has been received */
    {
        I2C_STOP(I2C5);
        I2C_START(I2C5);
    }
    else if(u32Status == 0x28)                     /* DATA has been transmitted and ACK has been received */
    {
        if(g_u8MasterDataLen <= 2)
        {
            I2C_SET_DATA(I2C5, g_u8MasterTx_Buffer[g_u8MasterDataLen++]);
            I2C_SET_CONTROL_REG(I2C5, I2C_CTL_SI);
        }
        else
        {
            I2C_SET_CONTROL_REG(I2C5, I2C_CTL_STA_SI);
        }
    }
    else if(u32Status == 0x10)                    /* Repeat START has been transmitted and prepare SLA+R */
    {
        I2C_SET_DATA(I2C5, (g_u8DeviceAddr << 1) | 0x01);   /* Write SLA+R to Register I2CDAT */
        I2C_SET_CONTROL_REG(I2C5, I2C_CTL_SI);
    }
    else if(u32Status == 0x40)                    /* SLA+R has been transmitted and ACK has been received */
    {
        sysprintf("   \n");
        I2C_SET_CONTROL_REG(I2C5, I2C_CTL_SI_AA);

        /* Enable I2C5 Master RX */
        I2C5->CTL1 = I2C_CTL1_RXPDMAEN_Msk;
    }
    else if(u32Status == 0x50)                    /* DATA has been received and ACK has been returned */
    {
        /*
           Note:
           During PDMA operation, I2C controller will not occur receive data ACK interrupt
        */
    }
    else if(u32Status == 0x58)                    /* DATA has been received and NACK has been returned */
    {
        I2C_SET_CONTROL_REG(I2C5, I2C_CTL_STO | I2C_CTL_SI);
    }
    else
    {
        /* TO DO */
        sysprintf("Status 0x%x is NOT processed\n", u32Status);
    }
}

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
    CLK_EnableModuleClock(I2C5_MODULE);
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL2_UART0SEL_HXT, CLK_CLKDIV1_UART0(2));
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_EnableModuleClock(GPM_MODULE);
    CLK_EnableModuleClock(PDMA2_MODULE);

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set GPE multi-function pins for UART0 RXD and TXD */
    SYS->GPE_MFPH &= ~(SYS_GPE_MFPH_PE14MFP_Msk | SYS_GPE_MFPH_PE15MFP_Msk);
    SYS->GPE_MFPH |= (SYS_GPE_MFPH_PE14MFP_UART0_TXD | SYS_GPE_MFPH_PE15MFP_UART0_RXD);
    /* Set multi-function pins for I2C4 */
    SYS->GPM_MFPL &= ~(SYS_GPM_MFPL_PM0MFP_Msk | SYS_GPM_MFPL_PM1MFP_Msk);
    SYS->GPM_MFPL |= SYS_GPM_MFPL_PM0MFP_I2C4_SDA | SYS_GPM_MFPL_PM1MFP_I2C4_SCL;
    /* Set multi-function pins for I2C5 */
    SYS->GPM_MFPL &= ~(SYS_GPM_MFPL_PM4MFP_Msk | SYS_GPM_MFPL_PM5MFP_Msk);
    SYS->GPM_MFPL |= SYS_GPM_MFPL_PM4MFP_I2C5_SDA | SYS_GPM_MFPL_PM5MFP_I2C5_SCL;

    /* Lock protected registers */
    SYS_LockReg();
}

void I2C5_Init(void)
{
    /* Open I2C5 module and set bus clock */
    I2C_Open(I2C5, 100000);

    /* Get I2C5 Bus Clock */
    sysprintf("I2C5 clock %d Hz\n", I2C_GetBusClockFreq(I2C5));

    /* Set I2C5 4 Slave Addresses */
    I2C_SetSlaveAddr(I2C5, 0, 0x15, 0);   /* Slave Address : 0x15 */
    I2C_SetSlaveAddr(I2C5, 1, 0x35, 0);   /* Slave Address : 0x35 */
    I2C_SetSlaveAddr(I2C5, 2, 0x55, 0);   /* Slave Address : 0x55 */
    I2C_SetSlaveAddr(I2C5, 3, 0x75, 0);   /* Slave Address : 0x75 */

    /* Set I2C5 4 Slave Addresses Mask */
    I2C_SetSlaveAddrMask(I2C5, 0, 0x01);
    I2C_SetSlaveAddrMask(I2C5, 1, 0x04);
    I2C_SetSlaveAddrMask(I2C5, 2, 0x01);
    I2C_SetSlaveAddrMask(I2C5, 3, 0x04);

    /* Enable I2C5 interrupt */
    I2C_EnableInt(I2C5);
    IRQ_SetHandler((IRQn_ID_t)I2C5_IRQn, I2C5_IRQHandler);
    IRQ_Enable ((IRQn_ID_t)I2C5_IRQn);
}

void I2C4_Init(void)
{
    /* Open I2C4 module and set bus clock */
    I2C_Open(I2C4, 100000);

    /* Get I2C4 Bus Clock */
    sysprintf("I2C4 clock %d Hz\n", I2C_GetBusClockFreq(I2C4));

    /* Set I2C4 4 Slave Addresses */
    I2C_SetSlaveAddr(I2C4, 0, 0x16, 0);   /* Slave Address : 0x16 */
    I2C_SetSlaveAddr(I2C4, 1, 0x36, 0);   /* Slave Address : 0x36 */
    I2C_SetSlaveAddr(I2C4, 2, 0x56, 0);   /* Slave Address : 0x56 */
    I2C_SetSlaveAddr(I2C4, 3, 0x76, 0);   /* Slave Address : 0x76 */

    /* Set I2C4 4 Slave Addresses Mask */
    I2C_SetSlaveAddrMask(I2C4, 0, 0x04);
    I2C_SetSlaveAddrMask(I2C4, 1, 0x02);
    I2C_SetSlaveAddrMask(I2C4, 2, 0x04);
    I2C_SetSlaveAddrMask(I2C4, 3, 0x02);

    /* Enable I2C4 interrupt */
    I2C_EnableInt(I2C4);
    IRQ_SetHandler((IRQn_ID_t)I2C4_IRQn, I2C4_IRQHandler);
    IRQ_Enable ((IRQn_ID_t)I2C4_IRQn);
}

void PDMA_Init(void)
{
    /* Open PDMA Channel */
    PDMA_Open(PDMA, 1 << I2C5_PDMA_TX_CH); // Channel 0 for I2C5 TX
    PDMA_Open(PDMA, 1 << I2C4_PDMA_RX_CH); // Channel 1 for I2C4 RX
    PDMA_Open(PDMA, 1 << I2C5_PDMA_RX_CH); // Channel 2 for I2C5 RX
    PDMA_Open(PDMA, 1 << I2C4_PDMA_TX_CH); // Channel 3 for I2C4 TX
    // Set data width and transfer count
    PDMA_SetTransferCnt(PDMA, I2C5_PDMA_TX_CH, PDMA_WIDTH_8, PDMA_TEST_LENGTH);
    PDMA_SetTransferCnt(PDMA, I2C4_PDMA_RX_CH, PDMA_WIDTH_8, PDMA_TEST_LENGTH);
    PDMA_SetTransferCnt(PDMA, I2C5_PDMA_RX_CH, PDMA_WIDTH_8, PDMA_TEST_LENGTH - 3); // except Slave Address and two bytes Data Address
    PDMA_SetTransferCnt(PDMA, I2C4_PDMA_TX_CH, PDMA_WIDTH_8, PDMA_TEST_LENGTH - 3); // except Slave Address and two bytes Data Address
    //Set PDMA Transfer Address
    PDMA_SetTransferAddr(PDMA,I2C5_PDMA_TX_CH, ((uint32_t)((uint64_t)(&g_u8MasterTx_Buffer[0]))), PDMA_SAR_INC, (uint32_t)((uint64_t)(&(I2C5->DAT))), PDMA_DAR_FIX);
    PDMA_SetTransferAddr(PDMA,I2C4_PDMA_RX_CH, (uint32_t)((uint64_t)(&(I2C4->DAT))), PDMA_SAR_FIX, ((uint32_t)((uint64_t)(&g_u8SlaveRx_Buffer[0]))), PDMA_DAR_INC);
    PDMA_SetTransferAddr(PDMA,I2C5_PDMA_RX_CH, (uint32_t)((uint64_t)(&(I2C5->DAT))), PDMA_SAR_FIX, ((uint32_t)((uint64_t)(&g_u8MasterRx_Buffer[0]))), PDMA_DAR_INC);
    PDMA_SetTransferAddr(PDMA,I2C4_PDMA_TX_CH, ((uint32_t)((uint64_t)(&g_u8SlaveTx_Buffer[0]))), PDMA_SAR_INC, (uint32_t)((uint64_t)(&(I2C4->DAT))), PDMA_DAR_FIX);
    //Select Single Request
    PDMA_SetBurstType(PDMA, I2C5_PDMA_TX_CH, PDMA_REQ_SINGLE, 0);
    PDMA_SetBurstType(PDMA, I2C4_PDMA_RX_CH, PDMA_REQ_SINGLE, 0);
    PDMA_SetBurstType(PDMA, I2C5_PDMA_RX_CH, PDMA_REQ_SINGLE, 0);
    PDMA_SetBurstType(PDMA, I2C4_PDMA_TX_CH, PDMA_REQ_SINGLE, 0);
    // Select basic mode
    PDMA_SetTransferMode(PDMA, I2C5_PDMA_TX_CH, PDMA_I2C5_TX, 0, 0);
    PDMA_SetTransferMode(PDMA, I2C4_PDMA_RX_CH, PDMA_I2C4_RX, 0, 0);
    PDMA_SetTransferMode(PDMA, I2C5_PDMA_RX_CH, PDMA_I2C5_RX, 0, 0);
    PDMA_SetTransferMode(PDMA, I2C4_PDMA_TX_CH, PDMA_I2C4_TX, 0, 0);

    PDMA_EnableInt(PDMA, I2C5_PDMA_TX_CH, PDMA_INT_TRANS_DONE);
    PDMA_EnableInt(PDMA, I2C4_PDMA_RX_CH, PDMA_INT_TRANS_DONE);
    PDMA_EnableInt(PDMA, I2C5_PDMA_RX_CH, PDMA_INT_TRANS_DONE);
    PDMA_EnableInt(PDMA, I2C4_PDMA_TX_CH, PDMA_INT_TRANS_DONE);
    IRQ_SetHandler((IRQn_ID_t)PDMA2_IRQn, PDMA_IRQHandler);
    IRQ_Enable ((IRQn_ID_t)PDMA2_IRQn);
}

void I2C_PDMA(void)
{
    uint32_t i;

    for (i=0; i<PDMA_TEST_LENGTH; i++)
    {
        *(unsigned char volatile *)((u64)&g_u8MasterTx_Buffer[i] | 0x100000000) = i;
        *(unsigned char volatile *)((u64)&g_u8SlaveRx_Buffer[i] | 0x100000000) = 0xff;
    }

    *(unsigned char volatile *)((u64)&g_u8MasterTx_Buffer[0] | 0x100000000) = ((g_u8DeviceAddr << 1) | 0x00);   //1 byte SLV + W
    *(unsigned char volatile *)((u64)&g_u8MasterTx_Buffer[1] | 0x100000000) = 0x00;                             //2 bytes Data address
    *(unsigned char volatile *)((u64)&g_u8MasterTx_Buffer[2] | 0x100000000) = 0x00;

    PDMA_Init();

    /* I2C enter no address SLV mode */
    I2C_SET_CONTROL_REG(I2C4, I2C_CTL_SI | I2C_CTL_AA);
    /* Enable I2C4 PDMA RX mode */
    I2C4->CTL1 = I2C_CTL1_RXPDMAEN_Msk;

    /* I2C4 function to Slave receive/transmit data */
    s_I2C4HandlerFn = I2C_PDMA_SlaveRx;

    PDMA_DONE = 0;

    /* Enable I2C TX */
    I2C5->CTL1 = I2C_CTL1_TXPDMAEN_Msk;

    s_I2C5HandlerFn = (I2C_FUNC)I2C_PDMA_MasterTx;

    /* Send START condition, start the PDMA data transmit */
    I2C_START(I2C5);

    while (!PDMA_DONE);

    /* Disable I2C5 PDMA TX mode */
    I2C5->CTL1 &= ~I2C_CTL1_TXPDMAEN_Msk;
    /* Disable I2C4 PDMA RX mode */
    I2C4->CTL1 &= ~I2C_CTL1_RXPDMAEN_Msk;

    for (i=0; i<PDMA_TEST_LENGTH; i++)
    {
        if(*(unsigned char volatile *)((u64)&g_u8SlaveRx_Buffer[i] | 0x100000000) !=
           *(unsigned char volatile *)((u64)&g_u8MasterTx_Buffer[i] | 0x100000000))
        {
            sysprintf("\n Receive Data Compare Error !!");
            while(1);
        }
        else
        {
            if(i > 2)
                *(unsigned char volatile *)((u64)&g_u8SlaveTx_Buffer[i-3] | 0x100000000) =
                *(unsigned char volatile *)((u64)&g_u8MasterTx_Buffer[i] | 0x100000000);
        }
    }

    /* Test Master RX and Slave TX with PDMA function */

    /* I2C5 function to Master receive data */
    s_I2C5HandlerFn = (I2C_FUNC)I2C_PDMA_MasterRx;
    /* I2C4 function to Slave transmit data */
    s_I2C4HandlerFn = I2C_PDMA_SlaveTx;

    PDMA_DONE = 0;

    /* Send START condition */
    I2C_START(I2C5);

    while (!PDMA_DONE);

    /* Disable I2C5 PDMA RX mode */
    I2C5->CTL1 &= ~I2C_CTL1_RXPDMAEN_Msk;
    /* Disable I2C4 PDMA TX mode */
    I2C4->CTL1 &= ~I2C_CTL1_TXPDMAEN_Msk;

    for (i = 0; i < PDMA_TEST_LENGTH - 3; i++)
    {
        if(*(unsigned char volatile *)((u64)&g_u8MasterRx_Buffer[i] | 0x100000000) !=
           *(unsigned char volatile *)((u64)&g_u8MasterTx_Buffer[i+3] | 0x100000000))
        {
            sysprintf("\n Slave Receive Data Compare Error !!");

            while (1);
        }
    }

    sysprintf("\nI2C PDMA test Pass.\n");
}


int32_t main (void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, IP clock and multi-function I/O */
    SYS_Init();

    /* Init UART to 115200-8n1 for print message */
    UART_Open(UART0, 115200);

    /*
        This sample code sets I2C bus clock to 100kHz. Then, Master accesses Slave with Byte Write
        and Byte Read operations, and check if the read data is equal to the programmed data.
    */

    sysprintf("+-------------------------------------------------------+\n");
    sysprintf("|       I2C Driver Sample Code for PDMA                 |\n");
    sysprintf("|                                                       |\n");
    sysprintf("| I2C Master (I2C5) <---> I2C Slave(I2C4)               |\n");
    sysprintf("+-------------------------------------------------------+\n");

    /* Init I2C5 */
    I2C5_Init();
    /* Init I2C4 */
    I2C4_Init();

    I2C_PDMA();

    while(1);
}
