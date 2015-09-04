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


#ifndef MOATURL_H_
#define MOATURL_H_


SSE_BEGIN_C_DECLS

enum moat_url_scheme_type_ {
  MOAT_URL_SCHEME_UNKNOWN = 0,
  MOAT_URL_SCHEME_HTTP,
  MOAT_URL_SCHEME_HTTPS,
  MOAT_URL_SCHEMEs
};

typedef struct Url_ MoatUrl;

MoatUrl * moat_url_new(sse_char *in_url, sse_size in_url_len);
void moat_url_free(MoatUrl *self);
sse_int moat_url_parse(MoatUrl *self);
sse_int moat_url_add_query(MoatUrl *self, sse_char *in_key, sse_size in_key_len, sse_char *in_value, sse_size in_value_len);
sse_int moat_url_get_scheme(MoatUrl *self);
sse_char * moat_url_get_url(MoatUrl *self);
sse_int moat_url_peek_host(MoatUrl *self, sse_char **out_host, sse_size *out_len);
sse_int moat_url_peek_port(MoatUrl *self, sse_char **out_port, sse_size *out_len);
sse_int moat_url_peek_host_with_port(MoatUrl *self, sse_char **out_host, sse_size *out_len);
sse_int moat_url_get_port(MoatUrl *self, sse_uint16 *out_port);
sse_int moat_url_peek_path(MoatUrl *self, sse_char **out_path, sse_size *out_len);
sse_int moat_url_peek_request_url(MoatUrl *self, sse_char **out_url, sse_size *out_len);
sse_int moat_url_peek_query(MoatUrl *self, sse_char **out_query, sse_size *out_len);
sse_int moat_url_peek_fragment(MoatUrl *self, sse_char **out_fragment, sse_size *out_len);
sse_int moat_url_peek_user_info(MoatUrl *self, sse_char **out_user_info, sse_size *out_len);


SSE_END_C_DECLS

#endif /* MOATURL_H_ */
