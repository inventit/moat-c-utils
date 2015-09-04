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

#include <stdio.h>
#include <ctype.h>
#include <servicesync/moat.h>
#include "sseutils.h"

void
SseUtilDebug_Hexdump(sse_int in_category,
		     const sse_char* in_type,
		     const sse_char* in_tag,
		     const sse_char* in_func,
		     sse_int in_line,
		     const sse_pointer in_ptr,
		     sse_size in_len)
{
  const sse_byte* buff = (const sse_byte*)in_ptr;
  sse_int i, j;
  sse_char output[128]; /* MAX 74 bytes */
  sse_int wrote;

  for (i = 0; i < in_len; i += 16) {
    wrote = sprintf(output, "%06x: ", i);
    for (j = 0; j < 16; j++) {
      if (i+j < in_len) {
        wrote += sprintf(output + wrote, "%02x ", buff[i + j]);
      } else {
        wrote += sprintf(output + wrote, "   ");
      }
    }

    wrote += sprintf(output + wrote, " ");

    for (j = 0; j < 16; j++) {
      if (i+j < in_len) {
        wrote += sprintf(output + wrote, "%c", isprint(buff[i+j]) ? buff[i+j] : '.');
      }
    }
    ssep_app_log_print(in_category, "[%s] %s  %s():L%d %s", in_type, in_tag, in_func, in_line, output);
  }

  return;
}
