/*
 * LEGAL NOTICE
 *
 * Copyright (C) 2015 InventIt Inc. All rights reserved.
 *
 * This source code, product and/or document is protected under licenses
 * restricting its use, copying, distribution, and decompilation.
 * No part of this source code, product or document may be reproduced in
 * any form by any means without prior written authorization of InventIt Inc.
 * and its licensors, if any.
 *
 * InventIt Inc.
 * 9F KOJIMACHI CP BUILDING
 * 4-4-7 Kojimachi, Chiyoda-ku, Tokyo 102-0083
 * JAPAN
 * http://www.yourinventit.com/
 */

#include <stdio.h>
#include <servicesync/moat.h>
#include <sseutils.h>

#define TAG "SampleApp"

// workaround for "failed to dlsym() [./wiced-demo.so: undefined symbol: moat_app_main]"
extern "C" {
  sse_int moat_app_main(sse_int in_argc, sse_char *argv[]);
}

sse_int
moat_app_main(sse_int in_argc, sse_char *argv[])
{
  Moat moat = NULL;
  sse_int err = SSE_E_OK;

  SSESList* list;
  SSESList* l;
  SSESList* tmp;

  err = moat_init(argv[0], &moat);
  if (err != SSE_E_OK) {
    goto error_exit;
  }

  /* setup event handlers, timers, etc */
  /* register models */

  /* Get Interface list */
  err = SseUtilNetInfo_GetInterfaceList(&list);
  if (err != SSE_E_OK) {
    MOAT_LOG_ERROR(TAG, "SseUtilNetInfo_GetInterfaceList() ... failed with [%d]", err);
  }
  l = list;
  while (l) {
    SSEString *ifname = (SSEString*)sse_slist_data(l);
    SSE_UTIL_HEXDUMP_INFO(TAG, sse_string_get_cstr(ifname), sse_string_get_length(ifname));

    /* Get hardware address */
    SSEString *hwaddr = NULL;
    if (SseUtilNetInfo_GetHwAddress(ifname, &hwaddr) == SSE_E_OK) {
      SSE_UTIL_HEXDUMP_INFO(TAG, sse_string_get_cstr(hwaddr), sse_string_get_length(hwaddr));
    }
    sse_string_free(hwaddr, sse_true);

    /* Get IPv4 address */
    SSEString *ipv4addr = NULL;
    if (SseUtilNetInfo_GetIPv4Address(ifname, &ipv4addr) == SSE_E_OK) {
      SSE_UTIL_HEXDUMP_INFO(TAG, sse_string_get_cstr(ipv4addr), sse_string_get_length(ipv4addr));
    }
    sse_string_free(ipv4addr, sse_true);

    /* Get netmask */
    SSEString *netmask = NULL;
    if (SseUtilNetInfo_GetIPv4Netmask(ifname, &netmask) == SSE_E_OK) {
      SSE_UTIL_HEXDUMP_INFO(TAG, sse_string_get_cstr(netmask), sse_string_get_length(netmask));
    }
    sse_string_free(netmask, sse_true);

    /* Get IPv6 address */
    SSEString *ipv6addr = NULL;
    if (SseUtilNetInfo_GetIPv6Address(ifname, &ipv6addr) == SSE_E_OK) {
      SSE_UTIL_HEXDUMP_INFO(TAG, sse_string_get_cstr(ipv6addr), sse_string_get_length(ipv6addr));
    }
    sse_string_free(ipv6addr, sse_true);

    l = sse_slist_next(l);
  }

  /* Cleanup*/
  l = list;
  while (l) {
    tmp = l;
    l = sse_slist_unlink(l, tmp);
    sse_string_free((SSEString*)sse_slist_data(tmp), sse_true);
    sse_slist_free(tmp);
  }

  err = moat_run(moat);
  if (err != SSE_E_OK) {
    goto error_exit;
  }

  /* unregister models */
  /* teardown */

  moat_destroy(moat);
  return SSE_E_OK;

 error_exit:
  if (moat != NULL) {
    moat_destroy(moat);
  }
  return err;
}
