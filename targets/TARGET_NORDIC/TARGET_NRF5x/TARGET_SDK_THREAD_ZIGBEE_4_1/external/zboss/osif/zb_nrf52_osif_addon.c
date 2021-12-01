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
/* PURPOSE: Additional source file for compatibility with nRF SDK
*/

#define ZB_TRACE_FILE_ID 28287
#include "zboss_api.h"

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
#pragma GCC diagnostic ignored "-Wdeclaration-after-statement"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
#include "nrf_802154_const.h"
#include "nrf_802154.h"
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
#include "mac_nrf52_transceiver.h"
#include "mac_features/ack_generator/nrf_802154_ack_data.h"
#if !defined NRF_802154_FRAME_TIMESTAMP_ENABLED || !NRF_802154_FRAME_TIMESTAMP_ENABLED
#error Must define NRF_802154_FRAME_TIMESTAMP_ENABLED!
#endif

/* Headers from zb_nrf52_common.c */
#include <stdlib.h>
#include "zb_nrf52_internal.h"
#include "zb_nrf52_zboss_deps.h"
#include "sdk_config.h"

#if !defined SOFTDEVICE_PRESENT
#include <stdbool.h>

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
#pragma GCC diagnostic ignored "-Wdeclaration-after-statement"
#endif

#include "nrf_ecb.h"

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#else
#include "nrf_fstorage.h"
#endif /* SOFTDEVICE_PRESENT */

#include "nrf_log_ctrl.h"
#include "nrf_log.h"

#include "nrf_pwr_mgmt.h"

#if defined(MULTIPROTOCOL_802154_CONFIG_PRESENT)
#include "multiprotocol_802154_config.h"
#endif

#if defined(NRF_LOG_BACKEND_UART_ENABLED) && NRF_LOG_BACKEND_UART_ENABLED
#include "nrf_log_backend_uart.h"
#include "nrf_drv_uart.h"
extern nrf_drv_uart_t m_uart;
#endif

zb_bool_t zb_transceiver_get_pending_bit(zb_uint8_t *p_psdu)
{
  return nrf_802154_ack_data_pending_bit_should_be_set(p_psdu) ? ZB_TRUE : ZB_FALSE;
}

void zb_nrf52_trace_cpu_state(void)
{
  zb_uint32_t r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12;
  zb_uint32_t *sp;
  /* __StackTop defined in nrf5x_common.ld and declared in Nordic_SDK_16_0_0_21/components/libraries/util/app_util.h:83:17 */
  zb_uint32_t *stack_top = (zb_uint32_t *)&__StackTop;
  /* it seems that r7 is used as a frame pointer and it points to the frame of zb_nrf52_print_assert_info(),
     not to the frame where assert occured */
  __asm__ ("str r0, %[r0]\n"
           "str r1, %[r1]\n"
           "str r2, %[r2]\n"
           "str r3, %[r3]\n"
           "str r4, %[r4]\n"
           "str r5, %[r5]\n"
           "str r6, %[r6]\n"
           "str r7, %[r7]\n"
           "str r8, %[r8]\n"
           "str r9, %[r9]\n"
           "str r10, %[r10]\n"
           "str r11, %[r11]\n"
           "str r12, %[r12]\n"
           : [r0]   "=m" (r0),
             [r1]   "=m" (r1),
             [r2]   "=m" (r2),
             [r3]   "=m" (r3),
             [r4]   "=m" (r4),
             [r5]   "=m" (r5),
             [r6]   "=m" (r6),
             [r7]   "=m" (r7),
             [r8]   "=m" (r8),
             [r9]   "=m" (r9),
             [r10]  "=m" (r10),
             [r11]  "=m" (r11),
             [r12]  "=m" (r12)
    );
  __asm__("str sp, %[sp]\n"
          : [sp]   "=m" (sp)
    );

  TRACE_MSG(TRACE_ERROR, "CPU INFO:", (FMT__0));

  TRACE_MSG(TRACE_ERROR,
            "r0: 0x%x, r1: 0x%x, r2: 0x%x, r3: 0x%x, r4: 0x%x, r5: 0x%x, r6: 0x%x, r7: 0x%x, r8: 0x%x, r9: 0x%x, r10: 0x%x, r11: 0x%x, r12: 0x%x, sp: 0x%x",
            (FMT__D_D_D_D_D_D_D_D_D_D_D_D_D_D,
             r0, r1, r2, r3, r4, r5, r6, r7, r8, r9,
             r10, r11, r12, sp));

  TRACE_MSG(TRACE_ERROR, "Stack: ", (FMT__0));

  stack_top--;
  while (stack_top >= sp)
  {
    TRACE_MSG(TRACE_ERROR, "0x%x: 0x%x", (FMT__D_D, stack_top, *stack_top));
    stack_top--;
  }
}
