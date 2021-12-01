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
/*  PURPOSE: Vendor configuration for Nordic nrf52
*/

/*

 @note This file configures behavior and some size constants when
 compiling ZBOSS libraries. If some definition changed here, ZBOSS
 libraries must be recompiled. Changing this file contents without
 recompiling ZBOSS libraries causes memory structures inconsistence.

 DO NOT CHANGE THAT FILE INCLUDED INTO BINARY SDK BUILD!
*/
#ifndef ZB_VENDOR_CFG_NRF52_NORDIC_SDK_H
#define ZB_VENDOR_CFG_NRF52_NORDIC_SDK_H 1

/**
   NRF52 radio from Nordic
*/

#define ZB_CONFIG_NRF52

#define ZB_PLATFORM_NRF52

#define ZB_BDB_MODE
#define ZB_DISTRIBUTED_SECURITY_ON

#define ZB_NO_SYSTEST_SUPPORT

#define ZB_ALL_DEVICE_SUPPORT

#define ZB_REJOIN_BACKOFF

#define ZB_ENABLE_ZGP

//#define ZB_CHECK_OOM_STATUS
//#define ZB_SEND_OOM_STATUS

#define ZB_LOW_SECURITY_MODE
#define ZB_APS_USER_PAYLOAD
#define ZB_PROMISCUOUS_MODE
#define ZB_PRODUCTION_CONFIG
#define ZB_SECURITY_INSTALLCODES

#define ZB_DONT_NEED_TRACE_FILE_ID

/* Use NRF_LOG subsystem for ZBOSS trace.
   If disabled, ZBOSS will init and use UART for binary trace. */
#define ZB_NRF_TRACE

/* Use NRF fstorage for NVRAM operations. */
#define ZB_NRF_USE_FSTORAGE

/* Let device select flash memory address based on configuration. */
#define ZB_NVRAM_FLASH_AUTO_ADDRESS

/* Compile-time memory configuration: hard-coded parameters.

   DO NOT CHANGE IT WHEN COMPILING APPLICATIONS USING PRE-COMPILED ZBOSS LIBRARIES!
 */
#define ZB_PANID_TABLE_SIZE 28
#define ZB_DEV_MANUFACTORER_TABLE_SIZE 32
#define ZB_BUF_Q_SIZE 8
#define ZDO_TRAN_TABLE_SIZE 16
#define ZB_APS_ENDPOINTS_IN_GROUP_TABLE 8
#define ZB_NWK_BTR_TABLE_SIZE 16
#define ZB_NWK_BRR_TABLE_SIZE 16
#define ZB_MAX_EP_NUMBER 6
#ifndef ZB_ED_ROLE
#define ZB_APS_SRC_BINDING_TABLE_SIZE 16
#define ZB_APS_DST_BINDING_TABLE_SIZE 32
#else
#define ZB_APS_SRC_BINDING_TABLE_SIZE 8
#define ZB_APS_DST_BINDING_TABLE_SIZE 16
#endif  /* ZB_ED_ROLE */
#ifndef ZB_ED_ROLE
#define ZB_APS_GROUP_TABLE_SIZE 16
#else
#define ZB_APS_GROUP_TABLE_SIZE 8
#endif
#define ZB_ZGP_SINK_TBL_SIZE 24
#define ZB_ZGP_PROXY_TBL_SIZE 5

#define ZB_NRF52_RADIO_STATISTICS

#define ZB_CONFIGURABLE_MEM

/* Default memory storage configuration - to be used if user does not include any of zb_mem_config_xxx.h */
#ifdef ZB_CONFIG_DEFAULT_KERNEL_DEFINITION

#ifndef ZB_ED_ROLE
/* Note that by dafault ZC/ZR ZBOSS library uses ZC memory
 * configuration which is biffer than for ZR (more space for TCLK) */
#define ZB_CONFIG_ROLE_ZC
#else
/* If this is ZED-only ZBOSS library, let's use ZED-specific memory configuration. */
#define ZB_CONFIG_ROLE_ZED
#endif

#define ZB_CONFIG_OVERALL_NETWORK_SIZE 128

#define ZB_CONFIG_HIGH_TRAFFIC

#define ZB_CONFIG_APPLICATION_COMPLEX

#endif  /*  ZB_CONFIG_DEFAULT_KERNEL_DEFINITION */

#define APS_FRAGMENTATION 

#define ZB_NWK_BLACKLIST
#ifndef ZB_NWK_BLACKLIST_SIZE
#define ZB_NWK_BLACKLIST_SIZE 16
#endif

#define ZB_BDB_ENABLE_FINDING_BINDING

/* ----------------- WWAH ---------------------- */

#define ZB_R22_MULTIMAC
#define ZB_ZCL_SUPPORT_CLUSTER_WWAH
#define ZB_ZCL_ENABLE_WWAH_CLIENT
#define ZB_ZCL_ENABLE_WWAH_SERVER
#if (defined ZB_ZCL_SUPPORT_CLUSTER_WWAH && defined ZB_ZCL_ENABLE_WWAH_SERVER)
#define ZB_NWK_RETRY_COUNT
#define ZB_BEACON_SURVEY
#define ZB_PARENT_CLASSIFICATION
#endif
//#if (defined ZB_ZCL_SUPPORT_CLUSTER_WWAH && defined ZB_ZCL_ENABLE_WWAH_CLIENT)
#if 0
/* Just for testing purpose:  */
#define ZB_ENABLE_INTER_PAN_EXCHANGE
#define ZB_ENABLE_ZLL
#define ZB_BDB_TOUCHLINK
#endif

#endif /* ZB_VENDOR_CFG_NRF52_NORDIC_SDK_H */
