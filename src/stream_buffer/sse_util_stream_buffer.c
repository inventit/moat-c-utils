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
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <servicesync/moat.h>
#include <sseutils.h>

#define TAG "Util:StreamBuffer"
#define LOG_ERROR(format, ...) MOAT_LOG_ERROR(TAG, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...)  MOAT_LOG_WARN(TAG, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)  MOAT_LOG_INFO(TAG, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) MOAT_LOG_DEBUG(TAG, format, ##__VA_ARGS__)
#define LOG_TRACE(format, ...) MOAT_LOG_TRACE(TAG, format, ##__VA_ARGS__)

TSseUtilStreamBuffer*
SseUtilStreamBuffer_New(sse_uint in_size)
{
  TSseUtilStreamBuffer* self;
  self = sse_zeroalloc(sizeof(TSseUtilStreamBuffer));
  if (self == NULL) {
    LOG_ERROR("sse_zeroalloc() ... failed.");
    return NULL;
  }
  if (TSseUtilStreamBuffer_Initialize(self, in_size) != SSE_E_OK) {
    sse_free(self);
    return NULL;
  }
  return self;
}

void
TSseUtilStreamBuffer_Delete(TSseUtilStreamBuffer* self)
{
  if (self == NULL) {
    LOG_WARN("Invalid argument. self=[%p]", self);
    return;
  }
  TSseUtilStreamBuffer_Finalize(self);
  sse_free(self);
}

sse_int
TSseUtilStreamBuffer_Initialize(TSseUtilStreamBuffer* self, sse_uint in_size)
{
  if (self == NULL) {
    LOG_ERROR("Invalid argument. self=[%p]", self);
    return SSE_E_INVAL;
  }

  self->fBuffer = sse_zeroalloc(in_size);
  if (self->fBuffer == NULL) {
    LOG_ERROR("sse_zeroalloc() ... failed.");
    return SSE_E_NOMEM;
  }
  self->fBufferPos = self->fBuffer;
  self->fBufferSize = in_size;
  return SSE_E_OK;
}

void
TSseUtilStreamBuffer_Finalize(TSseUtilStreamBuffer* self)
{
  if (self == NULL) {
    LOG_ERROR("Invalid argument. self=[%p]", self);
    return;
  }

  if (self->fBuffer) {
    sse_free(self->fBuffer);
    self->fBuffer = NULL;
  }
  self->fBufferPos = NULL;
  self->fBufferSize = 0;
}

sse_int
TSseUtilStreamBuffer_OnReadable(TSseUtilStreamBuffer* self, sse_int in_desc)
{
  sse_int len;
  sse_char* end;

  if (self == NULL) {
    LOG_ERROR("Invalid argument. self=[%p]", self);
    return SSE_E_INVAL;
  }

  end = self->fBuffer + self->fBufferSize;
  while (self->fBufferPos < end) {
    len = read(in_desc, self->fBufferPos, end - self->fBufferPos);
    if (len == 0) {
      return SSE_E_NOENT;
    } else if (len < 0) {
      if (errno == EAGAIN) {
	break;
      }
      LOG_ERROR("read() ... failed with [%s]", strerror(errno));
      return SSE_E_GENERIC;
    }
    self->fBufferPos += len;
  }
  return SSE_E_OK;
}

sse_int
TSseUtilStreamBuffer_Read(TSseUtilStreamBuffer* self, sse_char* out_buffer, sse_uint in_buffer_size, sse_uint* out_data_len)
{
  sse_uint data_len;
  sse_uint copy_len;

  if (self == NULL || out_buffer == NULL || out_data_len == NULL) {
    LOG_ERROR("Invalid argument. self=[%p], out_buffer=[%p], out_data_len",
	      self, out_buffer, out_data_len);
    return SSE_E_INVAL;
  }

  data_len = self->fBufferPos - self->fBuffer;
  copy_len = data_len > in_buffer_size ? in_buffer_size : data_len;

  sse_memcpy(out_buffer, self->fBuffer, copy_len);
  sse_memmove(self->fBuffer, self->fBuffer + copy_len, (self->fBufferPos - self->fBuffer) - copy_len);
  self->fBufferPos -= copy_len;
  *out_data_len = copy_len;

  return SSE_E_OK;
}

sse_int
TSseUtilStreamBuffer_ReadLine(TSseUtilStreamBuffer* self, sse_char** out_buffer, sse_bool in_strip_crlf)
{
  sse_char* p;
  sse_char* line;
  sse_uint copy_len;

  if (self == NULL || out_buffer == NULL) {
    LOG_ERROR("Invalid argument. self=[%p], out_buffer=[%p]", self, out_buffer);
    return SSE_E_INVAL;
  }

  for (p = self->fBuffer; p < self->fBufferPos; p++) {
    if (*p == 0x0a) {
      p++;
      break;
    }
  }

  copy_len = p - self->fBuffer;
  if (copy_len == 0) {
    return SSE_E_NOENT;
  }

  line = sse_zeroalloc(copy_len + 1);
  if (line == NULL) {
    LOG_ERROR("sse_zeroalloc() ... failed.");
    return SSE_E_NOMEM;
  }

  sse_memcpy(line, self->fBuffer, copy_len);
  sse_memmove(self->fBuffer, self->fBuffer + copy_len, (self->fBufferPos - self->fBuffer) - copy_len);
  self->fBufferPos -= copy_len;

  /* Strip CRLF */
  if (in_strip_crlf) {
    p = line;
    while (*p++ != '\0') {
      if (*p == 0x0d || *p == 0x0a) {
	*p ='\0';
      }
    }
  }

  *out_buffer = line;
  return SSE_E_OK;
}

sse_int
TSseUtilsStramBuffer_ReadListWithSSEString(TSseUtilStreamBuffer* self, SSEString** out_string, sse_bool in_strip_crlf)
{
  sse_int err = SSE_E_GENERIC;
  sse_char* buffer = NULL;
  
  err = TSseUtilStreamBuffer_ReadLine(self, &buffer, in_strip_crlf);
  if (err != SSE_E_OK) {
    return err;
  }

  *out_string = sse_string_new(buffer);
  if (*out_string == NULL) {
    LOG_ERROR("sse_string_new() ... failed.");
    err= SSE_E_NOMEM;
    goto error_exit;
  }

  sse_free(buffer);
  return SSE_E_OK;

 error_exit:
  if (buffer) {
    sse_free(buffer);
  }
  return err;
}

sse_bool
TSseUtilsStramBuffer_HasData(TSseUtilStreamBuffer* self)
{
  if (self == NULL) {
    LOG_ERROR("Invalid argument, self=[%p]", self);
    return SSE_E_INVAL;
  }

  return (self->fBuffer != self->fBufferPos);
}
