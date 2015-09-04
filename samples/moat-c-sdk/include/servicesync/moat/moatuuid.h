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


#ifndef MOATUUID_H_
#define MOATUUID_H_


SSE_BEGIN_C_DECLS

#define MOAT_UUID_LEN           (16)
#define MOAT_UUID_STRING_LEN    (36)
#define MOAT_UUID_STRING_BUF_SIZE   (MOAT_UUID_STRING_LEN + 1)

typedef struct MoatUUID_ MoatUUID;
struct MoatUUID_ {
  sse_byte UUID[MOAT_UUID_LEN];
};

sse_char * moat_uuid_create_string(void);
sse_char * moat_uuid_create_string_time(void);
sse_char * moat_uuid_create_string_random(void);
void moat_uuid_generate(MoatUUID *out_uuid);
void moat_uuid_generate_time(MoatUUID *out_uuid);
void moat_uuid_generate_random(MoatUUID *out_uuid);
void moat_uuid_to_string(MoatUUID *in_uuid, sse_char *out_buffer);

SSE_END_C_DECLS

#endif /* MOATUUID_H_ */
