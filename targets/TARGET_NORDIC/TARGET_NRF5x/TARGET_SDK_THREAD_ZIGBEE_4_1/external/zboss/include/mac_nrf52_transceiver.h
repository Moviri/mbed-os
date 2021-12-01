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
/* PURPOSE: MAC definitions NRF52840 SoC.
*/
#ifndef MAC_NRF52840_TRANSCEIVER_H
#define MAC_NRF52840_TRANSCEIVER_H 1

/* mac_nrf52840_driver.c part */
void mac_nrf52_transmitted_raw(zb_uint8_t * p_ack);
void mac_nrf52_transmit_failed(zb_uint8_t error);
void mac_nrf52_received_timestamp_raw(zb_uint8_t * p_data, zb_int8_t power, zb_uint8_t lqi, zb_uint32_t time);

/* zb_nrf52840_transceiver.c part */
void zb_transceiver_hw_init(void);
void zb_transceiver_update_long_mac(zb_ieee_addr_t long_mac);
void zb_transceiver_start_get_rssi(zb_uint8_t scan_duration_bi);
void zb_transceiver_set_promiscuous_mode(zb_bool_t enabled);
void zb_transceiver_set_rx_on_off(int rx_on);
zb_bool_t zb_transceiver_is_idle(void);
zb_bool_t zb_transceiver_transmit(zb_uint8_t wait_type, zb_time_t tx_at, zb_uint8_t * tx_buf, zb_uint8_t current_channel);
void zb_transceiver_buffer_free(zb_uint8_t * p_buf);
zb_bool_t zb_transceiver_set_pending_bit(zb_uint8_t *addr, zb_bool_t value, zb_bool_t extended);
zb_bool_t zb_transceiver_get_pending_bit(zb_uint8_t *p_psdu);

#endif /* MAC_NRF52840_TRANSCEIVER_H */
