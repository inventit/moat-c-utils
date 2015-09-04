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

#define TAG "Util:CommandListener"
#define LOG_ERROR(format, ...) MOAT_LOG_ERROR(TAG, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...)  MOAT_LOG_WARN(TAG, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)  MOAT_LOG_INFO(TAG, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) MOAT_LOG_DEBUG(TAG, format, ##__VA_ARGS__)
#define LOG_TRACE(format, ...) MOAT_LOG_TRACE(TAG, format, ##__VA_ARGS__)

TSseUtilCommandListener::TSseUtilCommandListener() : fUserData(NULL)
{
}

TSseUtilCommandListener::TSseUtilCommandListener(sse_pointer in_user_data) : fUserData(in_user_data)
{
}

TSseUtilCommandListener::~TSseUtilCommandListener()
{
}

sse_int
TSseUtilCommandListener::OnReadable()
{
  LOG_WARN("%s shoud be never called..", __PRETTY_FUNCTION__);
  return SSE_E_OK;
}

sse_int
TSseUtilCommandListener::OnCompleted(sse_int in_result)
{
  LOG_WARN("%s shoud be never called..", __PRETTY_FUNCTION__);
  return SSE_E_OK;
}

sse_int
TSseUtilCommandListener::OnError(sse_int in_error_code, const sse_char* in_message)
{
  LOG_WARN("%s shoud be never called..", __PRETTY_FUNCTION__);
  return SSE_E_OK;
}

