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

#ifndef __SSE_UTIL_HEXDUMP_H__
#define __SSE_UTIL_HEXDUMP_H__

SSE_BEGIN_C_DECLS

/**
 * @breaf   Hexdump
 *
 * This API dumps data with hex and ascii.
 *
 * @param [in] in_category  Log category (See "enum sse_log_level"
 * @param [in] in_tag       Tag of log category (See "SSE_LOG_LABEL_xxx" in "include/servicesync/sse/ssebase.h".)
 * @param [in] in_func      Function name where log was printed.
 * @param [in] in_line      Line number where log was printed.
 * @param [in] in_ptr       Address of data should be dumped.
 * @param [in] in_len       Length of the data.
 *
 * @return None
 */
void
SseUtilDebug_Hexdump(sse_int in_category,
		     const sse_char* in_type,
		     const sse_char* in_tag,
		     const sse_char* in_func,
		     sse_int in_line,
		     const sse_pointer in_ptr,
		     sse_size in_len);

#define SSE_UTIL_HEXDUMP_ERROR(tag, ptr, len) SseUtilDebug_Hexdump(SSE_LOG_LEVEL_ERROR, SSE_LOG_LABEL_ERROR, tag, __FUNCTION__, __LINE__, ptr, len)
#define SSE_UTIL_HEXDUMP_WARN(tag, ptr, len)  SseUtilDebug_Hexdump(SSE_LOG_LEVEL_WARN,  SSE_LOG_LABEL_WARN,  tag, __FUNCTION__, __LINE__, ptr, len)
#define SSE_UTIL_HEXDUMP_INFO(tag, ptr, len)  SseUtilDebug_Hexdump(SSE_LOG_LEVEL_INFO,  SSE_LOG_LABEL_INFO,  tag, __FUNCTION__, __LINE__, ptr, len)
#define SSE_UTIL_HEXDUMP_DEBUG(tag, ptr, len) SseUtilDebug_Hexdump(SSE_LOG_LEVEL_DEBUG, SSE_LOG_LABEL_DEBUG, tag, __FUNCTION__, __LINE__, ptr, len)

SSE_END_C_DECLS

#endif /* __SSE_UTIL_HEXDUMP_H__ */
