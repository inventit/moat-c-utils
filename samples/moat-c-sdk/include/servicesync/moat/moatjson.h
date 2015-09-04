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


#ifndef MOATJSON_H_
#define MOATJSON_H_


SSE_BEGIN_C_DECLS

sse_int moat_json_string_to_moat_value(sse_char *in_json, sse_size in_len, MoatValue **out_val, sse_char **out_err_msg);
sse_int moat_json_file_to_moat_value(sse_char *in_path, MoatValue **out_val, sse_char **out_err_msg);

sse_int moat_json_string_to_moat_object(sse_char *in_json, sse_size in_len, MoatObject **out_obj, sse_char **out_err_msg);
sse_int moat_json_file_to_moat_object(sse_char *in_path, MoatObject **out_obj, sse_char **out_err_msg);

SSE_END_C_DECLS

#endif /* MOATJSON_H_ */
