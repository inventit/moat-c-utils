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


#ifndef SSEBASE64_H_
#define SSEBASE64_H_


SSE_BEGIN_C_DECLS

sse_size sse_base64_get_encoded_length(sse_size in_input_len);
void sse_base64_encode(sse_byte *in_data, sse_size in_input_len, sse_char *out_data);
void sse_base64_decode(sse_char *in_data, sse_size in_len, sse_byte *out_data, sse_size *out_len);

SSE_END_C_DECLS

#endif /* SSEBASE64_H_ */
