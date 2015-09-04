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
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <servicesync/moat.h>
#include <sseutils.h>

#define TAG "Util:NetInfo"
#define LOG_ERROR(format, ...) MOAT_LOG_ERROR(TAG, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...)  MOAT_LOG_WARN(TAG, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)  MOAT_LOG_INFO(TAG, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) MOAT_LOG_DEBUG(TAG, format, ##__VA_ARGS__)
#define LOG_TRACE(format, ...) MOAT_LOG_TRACE(TAG, format, ##__VA_ARGS__)

sse_int
SseUtilNetInfo_GetInterfaceList(SSESList **out_if_list)
{
  struct ifreq ifr[SSE_UTIL_NETINFO_MAX_INTERFACE_NUM];
  struct ifconf ifc;
  sse_int fd;
  sse_uint i;
  SSESList* list = NULL;

  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd == -1) {
    LOG_ERROR("socket() ... failed with [%s]", strerror(errno));
    return SSE_E_GENERIC;
  }

  ifc.ifc_len = sizeof(ifr);
  ifc.ifc_ifcu.ifcu_buf = (void *)ifr;
  if (ioctl(fd, SIOCGIFCONF, &ifc) == -1) {
    LOG_ERROR("ioctl() ... failed with [%s]", strerror(errno));
    close(fd);
    return SSE_E_GENERIC;
  }

  for (i = 0; i < ifc.ifc_len / sizeof (struct ifreq); i++) {
    list = sse_slist_add(list, sse_string_new(ifr[i].ifr_name));
  }
  *out_if_list = list;
  close(fd);
  return SSE_E_OK;
}

sse_int
SseUtilNetInfo_GetHwAddressCstr(const sse_char *in_ifname,
				sse_char *out_hwaddr,
				sse_size in_length)
{
  sse_int fd;
  struct ifreq ifr;

  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd == -1) {
    LOG_ERROR("socket() ... failed with [%s]", strerror(errno));
    return SSE_E_GENERIC;
  }

  sse_memset(&ifr, 0, sizeof(ifr));
  sse_strcpy(ifr.ifr_name, in_ifname);
  if (ioctl(fd, SIOCGIFHWADDR, &ifr) == -1) {
    LOG_ERROR("ioctl() ... failed with [%s]", strerror(errno));
    close(fd);
    return SSE_E_GENERIC;
  }
  snprintf(out_hwaddr, in_length, "%02x:%02x:%02x:%02x:%02x:%02x",
	  (sse_byte)ifr.ifr_hwaddr.sa_data[0],
	  (sse_byte)ifr.ifr_hwaddr.sa_data[1],
	  (sse_byte)ifr.ifr_hwaddr.sa_data[2],
	  (sse_byte)ifr.ifr_hwaddr.sa_data[3],
	  (sse_byte)ifr.ifr_hwaddr.sa_data[4],
	  (sse_byte)ifr.ifr_hwaddr.sa_data[5]);
  close(fd);
  return SSE_E_OK;
}

sse_int
SseUtilNetInfo_GetHwAddress(const SSEString *in_ifname,
			    SSEString **out_hwaddr)
{
  sse_int err;
  sse_char hwaddr[18];
  sse_char *ifname;

  ifname = sse_strndup(sse_string_get_cstr((SSEString *)in_ifname),
		       sse_string_get_length((SSEString *)in_ifname));
  if (ifname == NULL) {
    LOG_ERROR("sse_strndup() ... failed.");
    return SSE_E_NOMEM;
  }

  err = SseUtilNetInfo_GetHwAddressCstr(ifname, hwaddr, sizeof(hwaddr));
  if (err != SSE_E_OK) {
    sse_free(ifname);
    return err;
  }

  *out_hwaddr = sse_string_new(hwaddr);
  if (*out_hwaddr == NULL) {
    LOG_ERROR("sse_string_new() ... failed.");
    sse_free(ifname);
    return SSE_E_NOMEM;
  }
  sse_free(ifname);
  return SSE_E_OK;  
}

static sse_int
SseUtilNetInfo_GetIPAddressCstr(const sse_char *in_ifname,
				sse_char *out_ipaddr,
				sse_size in_length,
				sse_int in_family)
{
  struct ifaddrs *ifaddr;
  struct ifaddrs *i;
  sse_int family;
  sse_int s;
  
  if (in_ifname == NULL) {
    LOG_ERROR("in_ifname is NULL.");
    return SSE_E_INVAL;
  }
  if (out_ipaddr == NULL) {
    LOG_ERROR("out_ipaddr is NULL.");
    return SSE_E_INVAL;
  }
  if (in_length < NI_MAXHOST) {
    LOG_ERROR("in_length must be NI_MAXHOST(%d) and over.", NI_MAXHOST);
    return SSE_E_INVAL;
  }

  if (getifaddrs(&ifaddr) == -1) {
    LOG_ERROR("getifaddrs() ... failed with [%s]", strerror(errno));
    return SSE_E_GENERIC;
  }

  for (i = ifaddr; i != NULL; i = i->ifa_next) {
    family = i->ifa_addr->sa_family;
    if ((sse_strcmp(i->ifa_name, in_ifname) == 0) && (family == in_family)) {
      s = getnameinfo(i->ifa_addr,
		      (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
		      out_ipaddr, in_length, NULL, 0, NI_NUMERICHOST);
      if (s != 0) {
	LOG_ERROR("getnameinfo() ... failed with [%s]", strerror(errno));
	freeifaddrs(ifaddr);
	return SSE_E_GENERIC;
      }
      break;
    }
  }
  freeifaddrs(ifaddr);
  return SSE_E_OK;
}

sse_int
SseUtilNetInfo_GetIPv4AddressCstr(const sse_char *in_ifname,
				  sse_char *out_ipaddr,
				  sse_size in_length)
{
  return SseUtilNetInfo_GetIPAddressCstr(in_ifname, out_ipaddr, in_length, AF_INET);
}

sse_int
SseUtilNetInfo_GetIPv4Address(const SSEString *in_ifname,
			      SSEString **out_ipaddr)
{
  sse_int err;
  sse_char *ifname;
  sse_char ipaddr[NI_MAXHOST];

  if (in_ifname == NULL || out_ipaddr == NULL) {
    LOG_ERROR("Invalid argument.");
    return SSE_E_INVAL;
  }

  ifname = sse_strndup(sse_string_get_cstr((SSEString *)in_ifname),
		       sse_string_get_length((SSEString *)in_ifname));
  if (ifname == NULL) {
    LOG_ERROR("sse_strndup() ... failed.");
    return SSE_E_NOMEM;
  }

  err = SseUtilNetInfo_GetIPv4AddressCstr(ifname, ipaddr, sizeof(ipaddr));
  if (err != SSE_E_OK) {
    sse_free(ifname);
    return err;
  }

  *out_ipaddr = sse_string_new(ipaddr);
  if (*out_ipaddr == NULL) {
    LOG_ERROR("sse_string_new() ... failed.");
    sse_free(ifname);
    return SSE_E_NOMEM;
  }

  sse_free(ifname);
  return SSE_E_OK;
}

sse_int
SseUtilNetInfo_GetIPv6AddressCstr(const sse_char *in_ifname,
				  sse_char *out_ipaddr,
				  sse_size in_length)
{
  return SseUtilNetInfo_GetIPAddressCstr(in_ifname, out_ipaddr, in_length, AF_INET6);
}

sse_int
SseUtilNetInfo_GetIPv6Address(const SSEString *in_ifname,
			      SSEString **out_ipaddr)
{
 sse_int err;
  sse_char ipaddr[NI_MAXHOST];
  sse_char *ifname;

  if (in_ifname == NULL || out_ipaddr == NULL) {
    LOG_ERROR("Invalid argument.");
    return SSE_E_INVAL;
  }

  ifname = sse_strndup(sse_string_get_cstr((SSEString *)in_ifname),
		       sse_string_get_length((SSEString *)in_ifname));
  if (ifname == NULL) {
    LOG_ERROR("sse_strndup() ... failed.");
    return SSE_E_NOMEM;
  }

  err = SseUtilNetInfo_GetIPv6AddressCstr(ifname, ipaddr, sizeof(ipaddr));
  if (err != SSE_E_OK) {
    sse_free(ifname);
    return err;
  }

  *out_ipaddr = sse_string_new(ipaddr);
  if (*out_ipaddr == NULL) {
    LOG_ERROR("sse_string_new() ... failed.");
    sse_free(ifname);
    return SSE_E_NOMEM;
  }
  sse_free(ifname);
  return SSE_E_OK;
}

sse_int
SseUtilNetInfo_GetIPv4NetmaskCstr(const sse_char *in_ifname,
				  sse_char *out_netmask,
				  sse_size in_length)
{
  struct ifaddrs *ifaddr;
  struct ifaddrs *i;
  sse_int family;
  sse_int s;

  if (in_ifname == NULL) {
    LOG_ERROR("in_ifname is NULL.");
    return SSE_E_INVAL;
  }
  if (out_netmask == NULL) {
    LOG_ERROR("out_netmask is NULL.");
    return SSE_E_INVAL;
  }
  if (in_length < NI_MAXHOST) {
    LOG_ERROR("in_length must be NI_MAXHOST(%d) and over.", NI_MAXHOST);
    return SSE_E_INVAL;
  }

  if (getifaddrs(&ifaddr) == -1) {
    LOG_ERROR("getifaddrs() ... failed with [%s]", strerror(errno));
    return SSE_E_GENERIC;
  }

  for (i = ifaddr; i != NULL; i = i->ifa_next) {
    family = i->ifa_addr->sa_family;
    if ((sse_strcmp(i->ifa_name, in_ifname) == 0) && (family == AF_INET)) {
      s = getnameinfo(i->ifa_netmask,
		      (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
		      out_netmask, in_length, NULL, 0, NI_NUMERICHOST);
      if (s != 0) {
	LOG_ERROR("getnameinfo() ... failed with [%s]", strerror(errno));
	freeifaddrs(ifaddr);
	return SSE_E_GENERIC;
      }
      break;
    }
  }
  freeifaddrs(ifaddr);
  return SSE_E_OK;
}

sse_int
SseUtilNetInfo_GetIPv4Netmask(const SSEString *in_ifname,
			      SSEString **out_netmask)
{
  sse_int err;
  sse_char netmask[NI_MAXHOST];
  sse_char *ifname;

  if (in_ifname == NULL || out_netmask == NULL) {
    LOG_ERROR("Invalid argument.");
    return SSE_E_INVAL;
  }

  ifname = sse_strndup(sse_string_get_cstr((SSEString *)in_ifname),
		       sse_string_get_length((SSEString *)in_ifname));
  if (ifname == NULL) {
    LOG_ERROR("sse_strndup() ... failed.");
    return SSE_E_NOMEM;
  }

  err = SseUtilNetInfo_GetIPv4NetmaskCstr(ifname, netmask, sizeof(netmask));
  if (err != SSE_E_OK) {
    sse_free(ifname);
    return err;
  }

  *out_netmask = sse_string_new(netmask);
  if (*out_netmask == NULL) {
    LOG_ERROR("sse_string_new() ... failed.");
    sse_free(ifname);
    return SSE_E_NOMEM;
  }
  sse_free(ifname);
  return SSE_E_OK;
}

