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

#ifndef __SSE_UTIL_NETINFO_H__
#define __SSE_UTIL_NETINFO_H__

SSE_BEGIN_C_DECLS

#define SSE_UTIL_NETINFO_MAX_INTERFACE_NUM (4)

/**
 * @brief Get Interface List
 *
 * This function gets network interfaces list. (e.g. [ 'lo', 'eth0' ])
 *
 * @param [out] out_if_list List of network interfaces. Data type of element is SSEString.
 *
 * @retval SSE_E_OK Success
 * @retval others Failure
 */
sse_int
SseUtilNetInfo_GetInterfaceList(SSESList **out_if_list);

/**
 * @brief Get hardware address
 *
 * This function gets hardware address (MAC address) of specified interface.
 *
 * @param [in]  in_ifname  Interface name
 * @param [out] out_hwaddr Hardware address
 * @param [in]  in_length  Size of out_hwaddr. Must be larger than 18 byte.
 *
 * @retval SSE_E_OK Success
 * @retval others Failure
 */
sse_int
SseUtilNetInfo_GetHwAddressCstr(const sse_char *in_ifname,
				sse_char *out_hwaddr,
				sse_size in_length);

/**
 * @brief Get hardware address
 *
 * This function is wrapper function of SseUtilNetInfo_GetHwAddress()
 * using SSEString.
 *
 * @param [in]  in_ifname  Interface name
 * @param [out] out_hwaddr Hardware address
 *
 * @retval SSE_E_OK Success
 * @retval others Failure
 */
sse_int
SseUtilNetInfo_GetHwAddress(const SSEString *in_ifname,
			    SSEString **out_hwaddr);

/**
 * @brief Get IPv4 address
 *
 * This function gets IPv4 address of specified interface.
 *
 * @param [in]  in_ifname  Interface name
 * @param [out] out_ipaddr Buffer for IPv4 address. Must be NI_MAXHOST and more.
 * @param [in]  in_length  Size of out_ipaddr
 *
 * @retval SSE_E_OK Success
 * @retval others Failure
 */
sse_int
SseUtilNetInfo_GetIPv4AddressCstr(const sse_char *in_ifname,
				  sse_char *out_ipaddr,
				  sse_size in_length);

/**
 * @brief Get IPv4 address
 *
 * This function is wapper function of SseUtilNetInfo_GetIPv4AddressCstr()
 * using SSEString.
 * SSEString instance will be allocated in this function, so caller side
 * has to release out_ipaddr.
 *
 * @param [in]  in_ifname   Interface name
 * @param [out] out_ipaddr  IPv4 address
 *
 * @retval SSE_E_OK Success
 * @retval others Failure
 */
sse_int
SseUtilNetInfo_GetIPv4Address(const SSEString *in_ifname,
			      SSEString **out_ipaddr);

/**
 * @brief Get IPv6 address
 *
 * This function gets IPv6 address of specified interface.
 *
 * @param [in]  in_ifname  Interface name
 * @param [out] out_ipaddr IPv6 address. Must be NI_MAXHOST and more.
 * @param [in]  in_length  Size of out_ipaddr
 *
 * @retval SSE_E_OK Success
 * @retval others Failure
 */
sse_int
SseUtilNetInfo_GetIPv6AddressCstr(const sse_char *in_ifname,
				  sse_char *out_ipaddr,
				  sse_size in_length);

/**
 * @brief Get IPv6 address
 *
 * This function is wapper function of SseUtilNetInfo_GetIPv6AddressCstr()
 * using SSEString.
 * SSEString instance will be allocated in this function, so caller side
 * has to release out_ipaddr.
 *
 * @param [in]  in_ifname  Interface name
 * @param [out] out_ipaddr IPv6 address
 *
 * @retval SSE_E_OK Success
 * @retval others Failure
 */
sse_int
SseUtilNetInfo_GetIPv6Address(const SSEString *in_ifname,
			      SSEString **out_ipaddr);


/**
 * @brief Get netmask
 *
 * This function gets netmask of specified interface.
 *
 * @param [in]  in_ifname   Interface name
 * @param [out] out_netmask Buffer for netmask. Must be NI_MAXHOST and more.
 * @param [in]  in_length   Size of out_netmask
 *
 * @retval SSE_E_OK Success
 * @retval others Failure
 */
sse_int
SseUtilNetInfo_GetIPv4NetmaskCstr(const sse_char *in_ifname,
				  sse_char *out_netmask,
				  sse_size in_length);

/**
 * @brief Get netmask
 *
 * This function is wapper function of SseUtilNetInfo_GetIPv4NetmaskCstr()
 * using SSEString.
 * SSEString instance will be allocated in this function, so caller side
 * has to release out_ipaddr.
 *
 * @param [in]  in_ifname   Interface name
 * @param [out] out_netmask Netmask
 *
 * @retval SSE_E_OK Success
 * @retval others Failure
 */
sse_int
SseUtilNetInfo_GetIPv4Netmask(const SSEString *in_ifname,
			      SSEString **out_netmask);

SSE_END_C_DECLS

#endif // __SSE_UTIL_NETINFO_H__
