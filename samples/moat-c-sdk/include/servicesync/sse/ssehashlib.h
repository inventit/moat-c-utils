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


#ifndef SSEHASHLIB_H_
#define SSEHASHLIB_H_


SSE_BEGIN_C_DECLS

#define BITS_PER_BYTE           (8)

#define SHL_WORD32_BITS         (32)
#define SHL_WORD32_BYTES        (SHL_WORD32_BITS / BITS_PER_BYTE)
#define SHL_WORD32_BLOCK_BITS   (512)
#define SHL_WORD32_BLOCK_BYTES  (SHL_WORD32_BLOCK_BITS / BITS_PER_BYTE)
#define SHL_WORD32_LENGTH_BITS  (64)
#define SHL_WORD32_LENGTH_BYTES (SHL_WORD32_LENGTH_BITS / BITS_PER_BYTE)

#define MD5_MD_BITS       (128)
#define MD5_MD_WORDS      (MD5_MD_BITS / SHL_WORD32_BITS)
#define MD5_MD_BYTES      (MD5_MD_BITS / BITS_PER_BYTE)

#define SHA1_MD_BITS      (160)
#define SHA1_MD_WORDS     (SHA1_MD_BITS / SHL_WORD32_BITS)
#define SHA1_MD_BYTES     (SHA1_MD_BITS / BITS_PER_BYTE)

#define SHA256_MD_BITS    (256)
#define SHA256_MD_WORDS   (SHA256_MD_BITS / SHL_WORD32_BITS)
#define SHA256_MD_BYTES   (SHA256_MD_BITS / BITS_PER_BYTE)

typedef struct SSEHlWord32Context_ SSEHlWord32Context;
struct SSEHlWord32Context_ {
  sse_int State;
  sse_uint32 CurrentBytes;
  sse_uint32 Bits[2];
  sse_byte Block[SHL_WORD32_BLOCK_BYTES];
};

typedef struct SSEMd5Context_ SSEMd5Context;
struct SSEMd5Context_ {
  SSEHlWord32Context BaseContext;
  sse_uint32 MessageDigest[MD5_MD_WORDS];
};

typedef struct SSESha1Context_ SSESha1Context;
struct SSESha1Context_ {
  SSEHlWord32Context BaseContext;
  sse_uint32 MessageDigest[SHA1_MD_WORDS];
};

typedef struct SSESha256Context_ SSESha256Context;
struct SSESha256Context_ {
  SSEHlWord32Context BaseContext;
  sse_uint32 MessageDigest[SHA256_MD_WORDS];
};

void sse_hashlib_md5(sse_byte *in_data, sse_size in_size, sse_byte *out_md);
void sse_hashlib_md5_init(SSEMd5Context *in_md5);
void sse_hashlib_md5_update(SSEMd5Context *in_md5, sse_byte *in_data, sse_size in_size);
void sse_hashlib_md5_fini(SSEMd5Context *in_md5, sse_byte *out_md);

void sse_hashlib_sha1(sse_byte *in_data, sse_size in_size, sse_byte *out_md);
void sse_hashlib_sha1_init(SSESha1Context *in_ctx);
void sse_hashlib_sha1_update(SSESha1Context *in_ctx, sse_byte *in_data, sse_size in_size);
void sse_hashlib_sha1_fini(SSESha1Context *in_ctx, sse_byte *out_md);

void sse_hashlib_sha256(sse_byte *in_data, sse_size in_size, sse_byte *out_md);
void sse_hashlib_sha256_init(SSESha256Context *in_ctx);
void sse_hashlib_sha256_update(SSESha256Context *in_ctx, sse_byte *in_data, sse_size in_size);
void sse_hashlib_sha256_fini(SSESha256Context *in_ctx, sse_byte *out_md);


SSE_END_C_DECLS

#endif /* SSEHASHLIB_H_ */
