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

#ifndef SSESTRING_H_
#define SSESTRING_H_


SSE_BEGIN_C_DECLS

typedef struct _SSEString		SSEString;

SSEString * sse_string_alloc(sse_size in_size);
SSEString * sse_string_new(sse_char *in_str);
SSEString * sse_string_new_with_length(sse_char *in_str, sse_size len);
SSEString * sse_string_new_from_array(sse_char *in_str_arr[], sse_int in_arr_size, sse_char *in_delim);
SSEString * sse_string_clone(SSEString *self);
sse_char * sse_string_free(SSEString *self, sse_bool in_free_cont);

sse_int sse_string_set_cstr(SSEString *self, sse_char *in_str);

sse_int sse_string_insert_cstr(SSEString *self, sse_size in_pos, sse_char *in_str);
sse_int sse_string_insert_char(SSEString *self, sse_size in_pos, sse_char in_c);
sse_int sse_string_insert_with_length(SSEString *self, sse_size in_pos, sse_char *in_str, sse_ssize in_len);
sse_int sse_string_insert_with_delimiter(SSEString *self, sse_size in_pos, sse_char *in_str, sse_char *in_delim);
sse_int sse_string_concat_cstr(SSEString *self, sse_char *in_str);
sse_int sse_string_concat_char(SSEString *self, sse_char in_c);
sse_int sse_string_concat_with_length(SSEString *self, sse_char *in_str, sse_size in_len);
sse_int sse_string_concat_with_delimiter(SSEString *self, sse_char *in_str, sse_char *in_delim);
sse_int sse_string_prepend_cstr(SSEString *self, sse_char *in_str);
sse_int sse_string_prepend_char(SSEString *self, sse_char in_c);
sse_int sse_string_prepend_with_length(SSEString *self, sse_char *in_str, sse_size in_len);
sse_int sse_string_prepend_with_delimiter(SSEString *self, sse_char *in_str, sse_char *in_delim);

void sse_string_remove(SSEString *self, sse_size in_pos, sse_ssize in_len);
void sse_string_truncate(SSEString *self, sse_size in_len);
sse_bool sse_string_equals(SSEString *self, SSEString *in_string);
sse_bool sse_string_equals_cstr(SSEString *self, sse_char *in_str);
sse_char * sse_string_get_cstr(SSEString *self);
sse_size sse_string_get_length(SSEString *self);
sse_size sse_string_get_buffer_size(SSEString *self);
void sse_string_to_lower_case(SSEString *string);
void sse_string_to_upper_case(SSEString *string);

SSESList * sse_string_split(SSEString *str, sse_char *in_delim, sse_int in_max);
sse_ssize sse_string_index_of(SSEString *self, sse_char *in_str, sse_size in_from_pos);
sse_ssize sse_string_replace(SSEString *self, sse_char *in_old, sse_char *in_new, sse_size in_from_pos);
sse_int sse_string_replace_all(SSEString *str, sse_char *in_old, sse_char *in_new, sse_size in_from_pos);

SSE_END_C_DECLS

#endif /* SSESTRING_H_ */

