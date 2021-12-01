/* ZBOSS Zigbee software protocol stack
 *
 * Copyright (c) 2012-2020 DSR Corporation, Denver CO, USA.
 * http://www.dsr-zboss.com
 * http://www.dsr-corporation.com
 * All rights reserved.
 *
 * This is unpublished proprietary source code of DSR Corporation
 * The copyright notice does not evidence any actual or intended
 * publication of such source code.
 *
 * ZBOSS is a registered trademark of Data Storage Research LLC d/b/a DSR
 * Corporation
 *
 * Commercial Usage
 * Licensees holding valid DSR Commercial licenses may use
 * this file in accordance with the DSR Commercial License
 * Agreement provided with the Software or, alternatively, in accordance
 * with the terms contained in a written agreement between you and
 * DSR.
 */
/* PURPOSE: Configuration file: configuration- and platform-specific definitions,
constants etc.
*/

#ifndef ZB_PLATFORM_CONFIG_H
#define ZB_PLATFORM_CONFIG_H 1

/**
   ZB_CONFIG_xxxxxx must be defined in build options.
*/

/***************************ARM Cortex platforms*****************************/

#ifndef ZB_DEVICE_RESET_BUTTON
  #define ZB_DEVICE_RESET_BUTTON 0
#endif

#ifdef ZB_CONFIG_NRF52
#define ZB_PLATFORM_NRF52
#define ZB_NRF_52
#endif  /* ZB_CONFIG_NRF52 */

#ifdef ZB_NRF_52
#define ZB_MAC_HARDWARE_PB_MATCHING
#define ZB_LITTLE_ENDIAN
#define ZB_NEED_ALIGN
#define ZB_NO_SYSTEST_SUPPORT
#define ZB_USE_NVRAM
#define ZB_SOFT_SECURITY        /* temporary; switch to hw later */
#define ZB_HW_ZB_AES128
#define USE_ASYNC_UART
#define ZB_INTERRUPT_SAFE_CALLBACKS

#ifndef ZB_IOBUF_POOL_SIZE
#define ZB_IOBUF_POOL_SIZE  40
#endif /* ZB_IOBUF_POOL_SIZE */

#ifndef ZB_SCHEDULER_Q_SIZE
#define ZB_SCHEDULER_Q_SIZE 30
#endif /* ZB_SCHEDULER_Q_SIZE */

#define ZB_ENABLE_HA
#define ZB_BDB_MODE
/* #define ZB_DISTRIBUTED_SECURITY_ON */

/* trace */
#ifdef ZB_TRACE_LEVEL
#define ZB_TRACE_OVER_USART
#define ZB_TRACE_TO_PORT
#define ZB_BINARY_TRACE
#define ZB_TRACE_FROM_INTR
#define ZB_TRAFFIC_DUMP_ON
#define ZB_TRAF_DUMP_V2
#ifndef ZB_TRACE_OVER_JTAG
#define ZB_HAVE_SERIAL
#endif  /* not over jtag */
#endif  /* if trace */

/* our MAC */
#define ZB_USE_NORDIC_RADIO_DRIVER
/*#define ZB_OSIF_CONFIGURABLE_TX_POWER stuff from r21 branch. Removed. */
#define ZB_MAC_CONFIGURABLE_TX_POWER
#define ZB_MANUAL_ACK
#define ZB_MAC_MANUAL_DUPS
#define ZB_MANUAL_ACK_TIMEOUT
#define ZB_AUTO_ACK_TX
#define MAC_ACK_PARSE_BY_INTR
#define ZB_MAC_AUTO_ACK_RECV
#define ZB_MAC_SINGLE_PACKET_IN_FIFO
#define ZB_MAC_PENDING_BIT_SOURCE_MATCHING
//#define MAC_AUTO_DELAY_IN_MAC_GP_SEND
#define MAC_AUTO_GPDF_RETX
#define ZB_MAC_RX_QUEUE_CAP 5
#ifndef ZB_NVRAM_BUF_SIZE
#define ZB_NVRAM_BUF_SIZE 256
#endif
#ifndef ZB_NVRAM_ENABLE_VER_MIGRATION
#define ZB_NO_NVRAM_VER_MIGRATION
#endif /* ZB_NVRAM_ENABLE_VER_MIGRATION */
#define ZB_NVRAM_RADIO_OFF_DURING_TRANSACTION
/* Enable Control4 Network support */
#define ZB_CONTROL4_NETWORK_SUPPORT
#ifndef ZB_CONFIGURABLE_MEM
#define ZB_CHILD_HASH_TABLE_SIZE (((ZB_IEEE_ADDR_TABLE_SIZE + ZB_IEEE_ADDR_TABLE_SIZE / 3) + 31) / 32 * 32)
#endif /* ZB_CONFIGURABLE_MEM */
#endif /* ZB_NRF_52 */

#if defined(ZB_SUB_GHZ)
#if defined(ZB_SUB_GHZ_EU1)
#define ZB_TRANSCEIVER_MAX_CHANNEL_NUMBER     34
#define ZB_TRANSCEIVER_START_CHANNEL_NUMBER   0

#elif defined(ZB_SUB_GHZ_EU2)
#define ZB_TRANSCEIVER_MAX_CHANNEL_NUMBER     17
#define ZB_TRANSCEIVER_START_CHANNEL_NUMBER   0

#elif defined(ZB_SUB_GHZ_EU3)
#define ZB_TRANSCEIVER_MAX_CHANNEL_NUMBER     17
#define ZB_TRANSCEIVER_START_CHANNEL_NUMBER   0

#elif defined(ZB_SUB_GHZ_US)
#define ZB_TRANSCEIVER_MAX_CHANNEL_NUMBER     10
#define ZB_TRANSCEIVER_START_CHANNEL_NUMBER   0
#elif defined ZB_SUB_GHZ_JP
#error "FixMe VS"
//#define ZB_TRANSCEIVER_MAX_CHANNEL_NUMBER     10
//#define ZB_TRANSCEIVER_START_CHANNEL_NUMBER   0
#elif defined ZB_SUB_GHZ_CN
#error "FixMe VS"
//#define ZB_TRANSCEIVER_MAX_CHANNEL_NUMBER     10
//#define ZB_TRANSCEIVER_START_CHANNEL_NUMBER   0
#else
#error "define Region!! (EU or US or JP or CN)"
#endif
#endif  /* ZB_SUB_GHZ */

#ifdef ZB_SUB_GHZ
#if defined(ZB_SUB_GHZ_EU1)    /* European countries and Russia */
//#define ZB_TRANSCEIVER_ALL_CHANNELS_MASK   0x00000001 /* 0000.0000 0000.0000 0000.0000 0000.0001 */
#error "Define ZB_TRANSCEIVER_ALL_CHANNELS_MASK for EU1"
#elif defined(ZB_SUB_GHZ_EU2)    /* European countries and Russia */
#define ZB_TRANSCEIVER_ALL_CHANNELS_MASK   0x0001FFFF /* 0000.0000 0000.0001 1111.1111 1111.1111 */
#elif defined(ZB_SUB_GHZ_EU3)    /* European countries and Russia */
#define ZB_TRANSCEIVER_ALL_CHANNELS_MASK   0x0001FFFF /* 0000.0000 0000.0001 1111.1111 1111.1111 */
#elif defined(ZB_SUB_GHZ_US)  /* US channels */
//#define ZB_TRANSCEIVER_ALL_CHANNELS_MASK   0x000007FE /* 0000.0000 0000.0000 0000.0111 1111.1110 */
#error "Define ZB_TRANSCEIVER_ALL_CHANNELS_MASK for USA"
#elif defined(ZB_SUB_GHZ_JP)
#error "Define ZB_TRANSCEIVER_ALL_CHANNELS_MASK for Japan"
#elif defined(ZB_SUB_GHZ_CN)
#error "Define ZB_TRANSCEIVER_ALL_CHANNELS_MASK for China"
#endif
#else
/* #define ZB_TRANSCEIVER_ALL_CHANNELS_MASK   (0xffff << 11) */ /* 0000.0111 1111.1111 1111.1000 0000.0000*/
/* C51 doesn't like long shifts, it just cut last two bytes. (11-26) */
/* TODO: Remove old subgig definitions */
#ifdef ZB_R22_MULTIMAC
#define ZB_TRANSCEIVER_ALL_CHANNELS_MASK   0x07FFF800     /* 11-26 for compatibility with 2.4Ghz*/
#define ZB_TRANSCEIVER_ALL_CHANNELS_MASK_PAGE28 0xE7FFFFFF
#define ZB_TRANSCEIVER_ALL_CHANNELS_MASK_PAGE29 0xE80001FF
#define ZB_TRANSCEIVER_ALL_CHANNELS_MASK_PAGE30 0xF7FFFFFF
#define ZB_TRANSCEIVER_ALL_CHANNELS_MASK_PAGE31 0xFFFFFFFF
#else
#define ZB_TRANSCEIVER_ALL_CHANNELS_MASK   0x07FFF800 /* 0000.0111 1111.1111 1111.1000 0000.0000*/
#endif
#endif /*ZB_SUB_GHZ*/

/***********************************************************************/
/*************************** MAC SECTION********************************/
/***********************************************************************/

/* ZB_DEBUG_ENLARGE_TIMEOUT is used as multiplier for debug build */
#if   defined ZB_NSNG
#define ZB_DEBUG_ENLARGE_TIMEOUT 1
#else
#define ZB_DEBUG_ENLARGE_TIMEOUT 1
#endif  /* ZB_NS_BUILD */

/* gpTxOffset should be calibrated for each HW/SW variant.
 * The reason is that ZB_GPD_TX_OFFSET_US is used from ZGP stub during Green Power frame transmitting,
 * and the actual TX time is not known at this level due to delays of MAC layer or transceiver.
 */
#define ZB_GPD_TX_OFFSET_US                                   \
  (ZB_MILLISECONDS_TO_USEC(ZB_GPD_RX_OFFSET_MS))

#endif /* ZB_PLATFORM_CONFIG_H */
