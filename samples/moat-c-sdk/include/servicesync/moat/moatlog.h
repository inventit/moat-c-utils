/*
 * LEGAL NOTICE
 *
 * Copyright (C) 2013 InventIt Inc. All rights reserved.
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


#ifndef MOATLOG_H_
#define MOATLOG_H_


SSE_BEGIN_C_DECLS

void ssep_app_log_print(sse_int level, const sse_char *format, ...);
#define MOAT_LOG(level, label, tag, format, ...)   ssep_app_log_print(level, "[" label "] " tag " %s():L%d " format, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define MOAT_LOG_ERROR(tag, format, ...) MOAT_LOG(SSE_LOG_LEVEL_ERROR, SSE_LOG_LABEL_ERROR, tag, format, ##__VA_ARGS__)
#define MOAT_LOG_WARN(tag, format, ...)  MOAT_LOG(SSE_LOG_LEVEL_WARN, SSE_LOG_LABEL_WARN, tag, format, ##__VA_ARGS__)
#define MOAT_LOG_INFO(tag, format, ...)  MOAT_LOG(SSE_LOG_LEVEL_INFO, SSE_LOG_LABEL_INFO, tag, format, ##__VA_ARGS__)
#define MOAT_LOG_DEBUG(tag, format, ...) MOAT_LOG(SSE_LOG_LEVEL_DEBUG, SSE_LOG_LABEL_DEBUG, tag, format, ##__VA_ARGS__)
#define MOAT_LOG_TRACE(tag, format, ...) MOAT_LOG(SSE_LOG_LEVEL_TRACE, SSE_LOG_LABEL_TRACE, tag, format, ##__VA_ARGS__)

SSE_END_C_DECLS

#endif /* MOATLOG_H_ */
