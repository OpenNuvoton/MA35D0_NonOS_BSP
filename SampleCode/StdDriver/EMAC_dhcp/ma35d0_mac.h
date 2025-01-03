/*************************************************************************//**
 * @file     ma35d0_mac.h
 * @brief    Packet processor header file for MA35D0
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright(C) 2023 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __MA35D0_MAC_H__
#define __MA35D0_MAC_H__

#include "NuMicro.h"
#include <string.h>
#include <stdio.h>

/******************************************************************************
 * User configuration
 * Numaker HMI MA35D0 :
 *  - EMACINTF0 + RMII_100M
 *  - EMACINTF1 + RMII_100M
 ******************************************************************************/
#define EMAC_INTF    EMACINTF1

#define EMAC_MODE    RMII_100M // mii mode supported by local PHY

#define DEFAULT_MAC0_ADDRESS {0x00, 0x11, 0x22, 0x33, 0x44, 0x55}
#define DEFAULT_MAC1_ADDRESS {0x00, 0x11, 0x22, 0x33, 0x44, 0x66}

/******************************************************************************
 * Functions
 ******************************************************************************/
s32 EMAC_setup_tx_desc_queue(EMACdevice *emacdev, u32 no_of_desc, u32 desc_mode);
s32 EMAC_setup_rx_desc_queue(EMACdevice *emacdev, u32 no_of_desc, u32 desc_mode);
s32 EMAC_set_mode(EMACdevice *emacdev);
s32 EMAC_open(int intf, int mode);
s32 EMAC_register_interrupt(int intf);
void EMAC_giveup_rx_desc_queue(EMACdevice *emacdev, u32 desc_mode);
void EMAC_giveup_tx_desc_queue(EMACdevice *emacdev, u32 desc_mode);
s32 EMAC_close(int intf);
s32 EMAC_xmit_frames(struct sk_buff *skb, int intf, u32 offload_needed, u32 ts);
void EMAC_handle_transmit_over(int intf);
void EMAC_handle_received_data(int intf);
static void EMAC_powerup_mac(EMACdevice *emacdev);
static void EMAC_powerdown_mac(EMACdevice *emacdev);
void EMAC_int_handler0(void);
void EMAC_int_handler1(void);

extern EMACdevice EMACdev[];
extern u8 mac_addr0[];
extern u8 mac_addr1[];
extern struct sk_buff txbuf[];
extern struct sk_buff rxbuf[];

#endif /* __MA35D0_MAC_H__ */
