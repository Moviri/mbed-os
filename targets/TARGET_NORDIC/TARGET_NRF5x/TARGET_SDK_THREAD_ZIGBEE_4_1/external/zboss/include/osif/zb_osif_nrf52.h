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
/* PURPOSE: Platform specific for Nordic nrf52 SoC
*/

#ifndef __ZB_OSIF_NRF52_H
#define __ZB_OSIF_NRF52_H

#include <string.h>             /* memcpy */

#include "zb_types.h"
#include "zb_ringbuffer.h"

#if defined ZB_NRF_ADDITIONAL_OSIF || defined ZB_OSIF_SPI_SLAVE || defined ZB_MACSPLIT_TRANSPORT_USERIAL
#include "zb_osif_nrf52840_adds.h"
#endif

#ifdef ZB_HAVE_SERIAL
#define ZB_HAVE_IOCTX
#endif

#define ZB_VOLATILE
#define ZB_CALLBACK
#define ZB_SDCC_BANKED
#define ZB_KEIL_REENTRANT

/* At ARM all types from 1 to 4 bytes are passed to vararg with casting to 4 bytes */
typedef zb_uint32_t zb_minimal_vararg_t;

/* use macros to be able to redefine */
#ifdef KEIL
#define ZB_MEMCPY(dst, src, size)   memcpy((void*)(dst), (void*)(src), (size))
#define ZB_MEMSET(dst, val, size)   memset((void*)(dst), (val), (size))
#define ZB_MEMMOVE(dst, src, size)  memmove((void*)(dst), (void*)(src), (size))
#define ZB_MEMCMP(ptr1, ptr2, size)	memcmp((void*)(ptr1), (void*)(ptr2), (size))
#else
#define ZB_MEMCPY   memcpy
#define ZB_MEMSET   memset
#define ZB_MEMMOVE  memmove
#define ZB_MEMCMP   memcmp
#endif

#define ZB_BZERO(s,l) ZB_MEMSET((char*)(s), 0, (l))
#define ZB_BZERO2(s) ZB_BZERO(s, 2)

#define ZVUNUSED(v) (void)v

void zb_nrf52_init(void);
void zb_nrf52_general_init(void);
void zb_nrf52_rf_init(void);
void zb_nrf52_abort(void);
void zb_nrf52_trace_cpu_state(void);
void zb_osif_go_idle(void);

#define ZB_PLATFORM_INIT() zb_nrf52_init()

void zb_osif_timer_init(void);
void zb_timer_enable_stop(void);
void zb_nrf52_timer_stop(void);
void zb_nrf52_timer_start(void);
zb_bool_t zb_nrf52_timer_is_on(void);
zb_uint32_t zb_nrf52_timer_get(void);

#ifdef ZB_MACSPLIT_DEVICE
#if defined (NRFX_WDT_ENABLED) && (NRFX_WDT_ENABLED == 1)
#define ZB_KICK_HW_WATCHDOG()    zb_nrf52_wdt_feed()
#else
#define ZB_KICK_HW_WATCHDOG()
#endif
#endif

#ifdef ZB_USE_SLEEP
void zb_nrf52_sleep_init(void);
zb_uint32_t zb_nrf52_sleep(zb_uint32_t sleep_tmo);
#endif /*ZB_USE_SLEEP*/

void zb_nrf52_rf_tx(zb_uint8_t channel, zb_uint8_t *buf, zb_uint8_t len);
void zb_nrf52_rf_setChannel(zb_uint8_t channel);
void zb_nrf52_enable_all_inter(void);
void zb_nrf52_disable_all_inter(void);
zb_uint16_t zb_nrf52_getRssi(void);
void zb_nrf_802154_mac_osif_init(void);

#if defined ZB_TRACE_OVER_USART && defined ZB_TRACE_LEVEL
#define ZB_SERIAL_FOR_TRACE
#endif

#define ZB_START_HW_TIMER()    zb_nrf52_timer_start()
#define ZB_STOP_HW_TIMER()     zb_nrf52_timer_stop()
#define ZB_CHECK_TIMER_IS_ON() zb_nrf52_timer_is_on()

#ifndef ZB_MACSPLIT_TRANSPORT_USERIAL
#define ZB_TRANSPORT_NONBLOCK_ITERATION() 0
#else
#define ZB_TRANSPORT_NONBLOCK_ITERATION() (zb_macsplit_transport_recv_bytes(),0)
#endif

#ifdef ZB_STACK_REGRESSION_TESTING_API

#define ZB_ENABLE_ALL_INTER()                            \
  ZB_REGRESSION_TESTS_API().osif_interrupts_balance++; \
  zb_nrf52_enable_all_inter()
#define ZB_DISABLE_ALL_INTER()                           \
  ZB_REGRESSION_TESTS_API().osif_interrupts_balance--; \
  zb_nrf52_disable_all_inter()

#else

#define ZB_ENABLE_ALL_INTER()  zb_nrf52_enable_all_inter()
#define ZB_DISABLE_ALL_INTER() zb_nrf52_disable_all_inter()

#endif  /* ZB_STACK_REGRESSION_TESTING_API */

#define ZB_OSIF_GLOBAL_LOCK()         ZB_DISABLE_ALL_INTER()
#define ZB_OSIF_GLOBAL_UNLOCK()       ZB_ENABLE_ALL_INTER()

#define ZB_ABORT zb_nrf52_abort

#define ZB_TRACE_CPU_STATE zb_nrf52_trace_cpu_state

#define ZB_GO_IDLE() zb_osif_go_idle()

#if defined ZB_HAVE_SERIAL

#define SERIAL_BUF_SIZE 256

/*Declare ring buffer for UART/USART operations */
ZB_RING_BUFFER_DECLARE(serial_iobuf, zb_uint8_t, SERIAL_BUF_SIZE);

/*User UART interrupt handler definition*/
typedef void (*zb_osif_uart_user_irq_handler)(void);

/*UART Receive data callback function definition*/
typedef void (*zb_osif_uart_byte_received_cb_t)(zb_uint8_t byte);


typedef struct zb_serial_ctx_s
{
  /* 04/02/2018 EE CR:MINOR Move it after tx_buf_cap: align big-to-small struct members. */
  zb_uint8_t    inited;
#if !defined ZB_NRF_TRACE
  serial_iobuf_t   tx_buf;
  zb_byte_array_t* p_tx_buf;
  zb_ushort_t      tx_buf_cap;
  zb_uint8_t       tx_in_progress;
  zb_osif_uart_byte_received_cb_t byte_received_cb;
#endif /* !defined ZB_NRF_TRACE || defined ZB_MACSPLIT_TRANSPORT_USERIAL */
#ifdef ZB_MACSPLIT_TRANSPORT_SERIAL
  uart_transport_rx_rb_t rx_data_rb;
#endif
#if defined ZB_MACSPLIT_TRANSPORT_USERIAL
#endif /* defined ZB_MACSPLIT_TRANSPORT_USERIAL */
}
zb_serial_ctx_t;

typedef struct zb_userial_ctx_s
{
  serial_iobuf_t   tx_buf;
  zb_uint8_t       inited;
  zb_uint8_t       open;
  zb_uint8_t       tx_in_progress;
  zb_osif_uart_byte_received_cb_t byte_received_cb;
}
zb_userial_ctx_t;

typedef struct zb_io_ctx_s
{
  zb_serial_ctx_t serial_ctx;
#if defined ZB_HAVE_USERIAL || defined ZB_MACSPLIT_TRANSPORT_USERIAL
  zb_userial_ctx_t userial_ctx;
#endif
#ifdef ZB_OSIF_SPI_SLAVE
  zb_osif_spi_slave_ctx_t spi_ctx;
#endif /* ZB_OSIF_SPI_SLAVE */
}
zb_io_ctx_t;

#if defined ZB_SERIAL_FOR_TRACE
void zb_osif_serial_flush(void);
#if defined ZB_NRF_TRACE && (defined ZB_ASYNC_TRACE_CONTROL || defined ZB_MULTI_TEST)
#define ZB_OSIF_SERIAL_FLUSH()    zb_osif_serial_flush()
#else
#define ZB_OSIF_SERIAL_FLUSH()
#endif /*defined ZB_NRF_TRACE && (defined ZB_ASYNC_TRACE_CONTROL || defined ZB_MULTI_TEST)*/
#endif /*defined ZB_SERIAL_FOR_TRACE*/

void zb_osif_userial_poll(void);
void zb_osif_userial_put_bytes(zb_uint8_t *buf, zb_short_t len);
void zb_osif_userial_init(void);
void zb_osif_set_userial_byte_received_cb(zb_osif_uart_byte_received_cb_t cb);
zb_bool_t zb_osif_userial_is_open(void);

#endif  /* ZB_HAVE_SERIAL */

#ifdef ZB_USE_OSIF_OTA_ROUTINES
#define OTA_HEADER_SIZE 56      /* sizeof(zb_zcl_ota_upgrade_file_header_t), no optional fields */
#define OTA_IMAGE_BLOCK_DATA_SIZE_MAX 60 /* from zb_zcl_ota_upgrade.h */

typedef struct zb_osif_flash_device_s {
  zb_uint32_t base_address;
  zb_uint32_t img_srv_address;
  /* TODO: these fields are used only for OTA server, may put it under some define... */
  zb_uint8_t ota_file_header[OTA_HEADER_SIZE];
  zb_uint8_t transmit_buffer[OTA_IMAGE_BLOCK_DATA_SIZE_MAX];
} zb_osif_flash_device_t;
#endif

/** @addtogroup special_nordic_functions
 * @{
 */

/**
 * @brief Read IEEE long address from FICR registers.
 *
 * @param[out] ieee_eui64  Pointer to a long address structure to be filled.
 *
 */

void zb_osif_get_ieee_eui64(zb_ieee_addr_t ieee_eui64);
/**
 * @}
 */
#if defined ZB_NRF52_RADIO_STATISTICS
typedef struct zb_nrf52_radio_stats_s
{
  zb_uint32_t rx_successful;
  zb_uint32_t rx_err_none; /* Error Code: 0x00 */
  zb_uint32_t rx_err_invalid_frame; /* Error Code: 0x01 */
  zb_uint32_t rx_err_invalid_fcs; /* Error Code: 0x02 */
  zb_uint32_t rx_err_invalid_dest_addr; /* Error Code: 0x03 */
  zb_uint32_t rx_err_runtime; /* Error Code: 0x04 */
  zb_uint32_t rx_err_timeslot_ended; /* Error Code: 0x05 */
  zb_uint32_t rx_err_aborted; /* Error Code: 0x06 */

  zb_uint32_t tx_successful;
  zb_uint32_t tx_err_none; /* Error Code: 0x00 */
  zb_uint32_t tx_err_busy_channel; /* Error Code: 0x01 */
  zb_uint32_t tx_err_invalid_ack; /* Error Code: 0x02 */
  zb_uint32_t tx_err_no_mem; /* Error Code: 0x03 */
  zb_uint32_t tx_err_timeslot_ended; /* Error Code: 0x04 */
  zb_uint32_t tx_err_no_ack; /* Error Code: 0x05 */
  zb_uint32_t tx_err_aborted; /* Error Code: 0x06 */
  zb_uint32_t tx_err_timeslot_denied; /* Error Code: 0x07 */

} zb_nrf52_radio_stats_t;

zb_nrf52_radio_stats_t* zb_nrf52_get_radio_stats(void);
#endif /* defined ZB_NRF52_RADIO_STATISTICS */

#endif /* __ZB_OSIF_NRF52_H */
