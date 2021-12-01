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
/* PURPOSE: Platform specific for NRF52 SoC.
*/

#define ZB_TRACE_FILE_ID 36508
#include "zboss_api.h"
#include "zb_nrf52_internal.h"
#include "zb_nrf52_zboss_deps.h"


#if defined ZB_NRF_TRACE
/* NRF Log subsystem */

#define NRF_LOG_MODULE_NAME zboss

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
#include "nrf_log.h"

NRF_LOG_MODULE_REGISTER();
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

void zb_osif_serial_init()
{
#ifdef ZB_ASYNC_TRACE_CONTROL
  static zb_bool_t backend_inited = ZB_FALSE;
#endif

  if (!(zb_osif_is_serial_init()) && (
#ifdef ZB_TRACE_LEVEL
        (g_trace_level != 0)
#else
        0
#endif
        ||
#ifdef ZB_TRAFFIC_DUMP_ON
        (g_traf_dump != 0)
#else
        0
#endif
        ))
  {
#ifdef ZB_ASYNC_TRACE_CONTROL
    if (!backend_inited)
    {
      APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
      NRF_LOG_DEFAULT_BACKENDS_INIT();
      backend_inited = ZB_TRUE;
    }
#endif
    zb_osif_set_serial_init();
  }
}

static zb_uint8_t buf8[8];
static zb_uint_t buffered;

void zb_osif_serial_put_bytes(const zb_uint8_t *buf, zb_short_t len)
{
  /* Try to fill hex dump by 8-bytes dumps */

  if (!(zb_osif_is_serial_init()))
  {
    return;
  }

  while (len)
  {
    zb_int_t n = 8 - buffered;
    if (n > len)
    {
      n = len;
    }
    ZB_MEMCPY(buf8 + buffered, buf, n);
    buffered += n;
    buf += n;
    len -= n;
    if (buffered == 8)
    {
      NRF_LOG_HEXDUMP_INFO(buf8, 8);
      buffered = 0;
    }
  } /* while */
}

#ifdef ZB_NRF_TRACE_RX_ENABLE
/*Function set UART RX callback function*/
void zb_osif_set_uart_byte_received_cb(zb_osif_uart_byte_received_cb_t cb)
{
    NRF_LOG_DEFAULT_BACKENDS_SET_RX_CB(cb);
}
#endif /*ZB_NRF_TRACE_RX_ENABLE*/

void zb_osif_serial_flush()
{
  if (buffered)
  {
    NRF_LOG_HEXDUMP_INFO(buf8, buffered);
    buffered = 0;
  }
  UNUSED_RETURN_VALUE(NRF_LOG_PROCESS());
}

#endif /* ZB_TRACE_LEVEL && defined ZB_NRF_TRACE */
