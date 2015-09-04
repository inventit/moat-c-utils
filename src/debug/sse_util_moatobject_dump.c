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

#include <servicesync/moat.h>
#include <sseutils.h>

#define TAG "Util:MoatObjectDump"
#define LOG_ERROR(format, ...) MOAT_LOG_ERROR(TAG, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...)  MOAT_LOG_WARN(TAG, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)  MOAT_LOG_INFO(TAG, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) MOAT_LOG_DEBUG(TAG, format, ##__VA_ARGS__)
#define LOG_TRACE(format, ...) MOAT_LOG_TRACE(TAG, format, ##__VA_ARGS__)

static void
__SseUtilMoatObjectDump(MoatObject* in_object, sse_int in_indent)
{
  sse_char* key;
  MoatValue* value;
  moat_value_type type;
  MoatObjectIterator* it = moat_object_create_iterator(in_object);
  sse_char* indent = sse_zeroalloc((in_indent * 4) + 1);

  sse_memset(indent, ' ', in_indent * 4);
  while (moat_object_iterator_has_next(it)) {
    key = moat_object_iterator_get_next_key(it);
    value = moat_object_get_value(in_object, key);
    type = moat_value_get_type(value);

    switch (type) {
    case MOAT_VALUE_TYPE_OBJECT:
      {
	MoatObject* object;
	moat_value_get_object(value, &object);
	
	LOG_DEBUG("%sKey=[%s], Type=OBJECT", indent, key);
	__SseUtilMoatObjectDump(object, in_indent + 1);
      }
      break;

    case MOAT_VALUE_TYPE_INT16:
      {
	sse_int16 val;
	moat_value_get_int16(value, &val);
	LOG_DEBUG("%sKey=[%s], Type=INT16, Value=[%d]", indent, key, val);
      }
      break;

    case MOAT_VALUE_TYPE_INT32:
      {
	sse_int32 val;
	moat_value_get_int32(value, &val);
	LOG_DEBUG("%sKey=[%s], Type=INT32, Value=[%d]", indent, key, val);
      }
      break;

    case MOAT_VALUE_TYPE_INT64:
      {
	sse_int64 val;
	moat_value_get_int64(value, &val);
	LOG_DEBUG("%sKey=[%s], Type=INT64, Value=[%lld]", indent, key, val);
      }
      break;

    case MOAT_VALUE_TYPE_STRING:
      {
	sse_char* val;
	sse_uint len;
	moat_value_get_string(value, &val, &len);
	LOG_DEBUG("%sKey=[%s], Type=STRING, Value=[%s], Len=[%d]", indent, key, val, len);
      }
      break;

    default:
      LOG_DEBUG("%sKey=[%s]", indent, key);
      LOG_WARN("%sSorry, unsupported vaule type=[%d]", indent, type);
    }
  }
  moat_object_iterator_free(it);
  sse_free(indent);
}

void
SseUtilMoatObjectDump(MoatObject* in_object)
{
  LOG_DEBUG("<--------------- DUMP MOAT OBJECT --------------->");
  __SseUtilMoatObjectDump(in_object, 0);
  LOG_DEBUG("<------------------------------------------------>");
}
