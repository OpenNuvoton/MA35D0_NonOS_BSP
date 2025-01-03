/**************************************************************************//**
 * @file     main.c
 *
 * @brief    Demonstrate LCD Framebuffer display.
 *
 * @copyright (C) 2023 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "NuMicro.h"
#include "displib.h"

#define DDR_ADR_FRAMEBUFFER   0x88000000UL

//#define DISPLAY_ARGB8
//#define DISPLAY_RGB565
#define DISPLAY_YUV422
//#define DISPLAY_NV12

extern uint32_t ImageARGB8DataBase, ImageARGB8DataLimit, ImageRGB565DataBase, ImageRGB565DataLimit, ImageYUV422DataBase, ImageYUV422DataLimit, ImageNV12DataBase, ImageNV12DataLimit;

/* LCD attributes 1024x600 */
DISP_LCD_INFO LcdPanelInfo =
{
    /* Panel Resolution */
    1024,
    600,
    /* DISP_LCD_TIMING */
    {
        51000000,
        1024,
        1,
        160,
        160,
        600,
        1,
        23,
        12,
        ePolarity_Positive,
        ePolarity_Positive
    },
    /* DISP_PANEL_CONF */
    {
        eDPIFmt_D24,
        ePolarity_Positive,
        ePolarity_Positive,
        ePolarity_Positive
    },
};

void SYS_Init(void)
{
    uint32_t pixelclk;

    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable IP clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Select IP clock source */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL2_UART0SEL_HXT, CLK_CLKDIV1_UART0(1));

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();

    /* Set GPE multi-function pins for UART0 RXD and TXD */
    SYS->GPE_MFPH &= ~(SYS_GPE_MFPH_PE14MFP_Msk | SYS_GPE_MFPH_PE15MFP_Msk);
    SYS->GPE_MFPH |= (SYS_GPE_MFPH_PE14MFP_UART0_TXD | SYS_GPE_MFPH_PE15MFP_UART0_RXD);

}

void DISP_Open(void)
{
    /* Set EPLL/2 as DISP Core Clock source */
    DISP_EnableDCUClk();

    /* Waiting EPLL ready */
    CLK_WaitClockReady(CLK_STATUS_STABLE_EPLL);

    /* Enable DISP Core Clock */
    CLK_EnableModuleClock(DCU_MODULE);

    /* Select DISP Core Clock source */
    CLK_SetModuleClock(DCU_MODULE, CLK_CLKSEL0_DCUSEL_EPLL_DIV2, 0);

    /* Select DISP pixel clock source to VPLL */
    DISP_GeneratePixelClk(LcdPanelInfo.sLcdTiming.u32PCF);

    /* Set multi-function pins for LCD Display Controller */
    SYS->GPG_MFPH = SYS->GPG_MFPH & ~(SYS_GPG_MFPH_PG8MFP_Msk | SYS_GPG_MFPH_PG9MFP_Msk | SYS_GPG_MFPH_PG10MFP_Msk) |
                    (SYS_GPG_MFPH_PG8MFP_LCM_VSYNC | SYS_GPG_MFPH_PG9MFP_LCM_HSYNC | SYS_GPG_MFPH_PG10MFP_LCM_CLK);
    SYS->GPK_MFPL = SYS->GPK_MFPL & ~(SYS_GPK_MFPL_PK4MFP_Msk) | SYS_GPK_MFPL_PK4MFP_LCM_DEN;

    SYS->GPI_MFPH = SYS->GPI_MFPH & ~(SYS_GPI_MFPH_PI8MFP_Msk | SYS_GPI_MFPH_PI9MFP_Msk | SYS_GPI_MFPH_PI10MFP_Msk | SYS_GPI_MFPH_PI11MFP_Msk |
                     SYS_GPI_MFPH_PI12MFP_Msk | SYS_GPI_MFPH_PI13MFP_Msk | SYS_GPI_MFPH_PI14MFP_Msk | SYS_GPI_MFPH_PI15MFP_Msk);
    SYS->GPI_MFPH |= (SYS_GPI_MFPH_PI8MFP_LCM_DATA0 | SYS_GPI_MFPH_PI9MFP_LCM_DATA1 | SYS_GPI_MFPH_PI10MFP_LCM_DATA2 |
                      SYS_GPI_MFPH_PI11MFP_LCM_DATA3 | SYS_GPI_MFPH_PI12MFP_LCM_DATA4 | SYS_GPI_MFPH_PI13MFP_LCM_DATA5 | SYS_GPI_MFPH_PI14MFP_LCM_DATA6 | SYS_GPI_MFPH_PI15MFP_LCM_DATA7);
    SYS->GPH_MFPL = SYS->GPH_MFPL & ~(SYS_GPH_MFPL_PH0MFP_Msk | SYS_GPH_MFPL_PH1MFP_Msk | SYS_GPH_MFPL_PH2MFP_Msk | SYS_GPH_MFPL_PH3MFP_Msk |
                    SYS_GPH_MFPL_PH4MFP_Msk | SYS_GPH_MFPL_PH5MFP_Msk | SYS_GPH_MFPL_PH6MFP_Msk | SYS_GPH_MFPL_PH7MFP_Msk);
    SYS->GPH_MFPL |= (SYS_GPH_MFPL_PH0MFP_LCM_DATA8 | SYS_GPH_MFPL_PH1MFP_LCM_DATA9 | SYS_GPH_MFPL_PH2MFP_LCM_DATA10 | SYS_GPH_MFPL_PH3MFP_LCM_DATA11 |
                      SYS_GPH_MFPL_PH4MFP_LCM_DATA12 | SYS_GPH_MFPL_PH5MFP_LCM_DATA13 | SYS_GPH_MFPL_PH6MFP_LCM_DATA14 | SYS_GPH_MFPL_PH7MFP_LCM_DATA15);

    SYS->GPC_MFPH = SYS->GPC_MFPH & ~(SYS_GPC_MFPH_PC12MFP_Msk | SYS_GPC_MFPH_PC13MFP_Msk | SYS_GPC_MFPH_PC14MFP_Msk | SYS_GPC_MFPH_PC15MFP_Msk);
    SYS->GPC_MFPH |= (SYS_GPC_MFPH_PC12MFP_LCM_DATA16 | SYS_GPC_MFPH_PC13MFP_LCM_DATA17 | SYS_GPC_MFPH_PC14MFP_LCM_DATA18 | SYS_GPC_MFPH_PC15MFP_LCM_DATA19);
    SYS->GPH_MFPH = SYS->GPH_MFPH & ~(SYS_GPH_MFPH_PH12MFP_Msk | SYS_GPH_MFPH_PH13MFP_Msk | SYS_GPH_MFPH_PH14MFP_Msk | SYS_GPH_MFPH_PH15MFP_Msk);
    SYS->GPH_MFPH |= (SYS_GPH_MFPH_PH12MFP_LCM_DATA20 | SYS_GPH_MFPH_PH13MFP_LCM_DATA21 | SYS_GPH_MFPH_PH14MFP_LCM_DATA22 | SYS_GPH_MFPH_PH15MFP_LCM_DATA23);
}

int main(void)
{
    uint32_t file_size;

    /* Init System, IP clock and multi-function I/O
       In the end of SYS_Init() will issue SYS_LockReg()
       to lock protected register. If user want to write
       protected register, please issue SYS_UnlockReg()
       to unlock protected register if necessary */
    SYS_Init();

    /* Init UART to 115200-8n1 for print message */
    UART_Open(UART0, 115200);

    /* Open DISP IP Clock and set multi-function pins */
    DISP_Open();

    /* Assign the highest AXI port priority to Display */
    DISPLIB_DDR_AXIPort_Priority();

    sysprintf("\n+------------------------------------------------------------------------+\n");
    sysprintf("|                This sample code show image under LCD Panel             |\n");
#ifdef DISPLAY_ARGB8
    sysprintf("|                Image format is ARGB8                                   |\n");
#endif
#ifdef DISPLAY_RGB565
    sysprintf("|                Image format is RGB565                                  |\n");
#endif
#ifdef DISPLAY_YUV422
    sysprintf("|                Image format is YUV422                                  |\n");
#endif
#ifdef DISPLAY_NV12
    sysprintf("|                Image format is NV12                                    |\n");
#endif
    sysprintf("+------------------------------------------------------------------------+\n");

    /* Configure display attributes of LCD panel */
    DISPLIB_LCDInit(LcdPanelInfo);

#ifdef DISPLAY_ARGB8
    file_size = ptr_to_u32(&ImageARGB8DataLimit) - ptr_to_u32(&ImageARGB8DataBase);
    /* Prepare DISP Framebuffer image */
    memcpy((void *)(nc_ptr(DDR_ADR_FRAMEBUFFER)), (const void *)(nc_ptr(&ImageARGB8DataBase)), file_size);

    /* Configure DISP Framebuffer settings  */
    DISPLIB_SetFBConfig(eFBFmt_A8R8G8B8, LcdPanelInfo.u32ResolutionWidth, LcdPanelInfo.u32ResolutionHeight, DDR_ADR_FRAMEBUFFER);
#endif

#ifdef DISPLAY_RGB565
    file_size = ptr_to_u32(&ImageRGB565DataLimit) - ptr_to_u32(&ImageRGB565DataBase);
    /* Prepare DISP Framebuffer image */
    memcpy((void *)(nc_ptr(DDR_ADR_FRAMEBUFFER)), (const void *)(nc_ptr(&ImageRGB565DataBase)), file_size);

    /* Configure DISP Framebuffer settings  */
    DISPLIB_SetFBConfig(eFBFmt_R5G6B5, LcdPanelInfo.u32ResolutionWidth, LcdPanelInfo.u32ResolutionHeight, DDR_ADR_FRAMEBUFFER);
#endif

#ifdef DISPLAY_YUV422
    file_size = ptr_to_u32(&ImageYUV422DataLimit) - ptr_to_u32(&ImageYUV422DataBase);
    /* Prepare DISP Framebuffer image */
    memcpy((void *)(nc_ptr(DDR_ADR_FRAMEBUFFER)), (const void *)(nc_ptr(&ImageYUV422DataBase)), file_size);

    /* Configure DISP Framebuffer settings  */
    DISPLIB_SetFBConfig(eFBFmt_YUY2, LcdPanelInfo.u32ResolutionWidth, LcdPanelInfo.u32ResolutionHeight, DDR_ADR_FRAMEBUFFER);
#endif

#ifdef DISPLAY_NV12
    file_size = ptr_to_u32(&ImageNV12DataLimit) - ptr_to_u32(&ImageNV12DataBase);
    /* Prepare DISP Framebuffer image */
    memcpy((void *)(nc_ptr(DDR_ADR_FRAMEBUFFER)), (const void *)(nc_ptr(&ImageNV12DataBase)), file_size);

    /* Configure DISP Framebuffer settings  */
    DISPLIB_SetFBConfig(eFBFmt_NV12, LcdPanelInfo.u32ResolutionWidth, LcdPanelInfo.u32ResolutionHeight, DDR_ADR_FRAMEBUFFER);
#endif

    /* Start to display */
    DISPLIB_EnableOutput(eLayer_Video);

    while(1);
}
