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


#ifndef MOATUTILS_H_
#define MOATUTILS_H_


SSE_BEGIN_C_DECLS

sse_char * moat_create_notification_id(sse_char *in_pkg_urn, sse_char *in_service_name, sse_char *in_version);
#ifdef SSE_CONFIG_USE_INT64
sse_uint64 moat_get_timestamp_msec(void);
#endif /* SSE_CONFIG_USE_INT64 */
sse_uint32 moat_get_timestamp_sec(void);

SSE_END_C_DECLS

#endif /* MOATUTILS_H_ */
