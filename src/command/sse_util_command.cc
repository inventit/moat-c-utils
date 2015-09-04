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

#include <new>
#include <servicesync/moat.h>
#include <sseutils.h>

#define TAG "Util:Command"
#define LOG_ERROR(format, ...) MOAT_LOG_ERROR(TAG, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...)  MOAT_LOG_WARN(TAG, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)  MOAT_LOG_INFO(TAG, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) MOAT_LOG_DEBUG(TAG, format, ##__VA_ARGS__)
#define LOG_TRACE(format, ...) MOAT_LOG_TRACE(TAG, format, ##__VA_ARGS__)

TSseUtilCommand::TSseUtilCommand() : fState(StateInitialized), fListeners(NULL)
{
}

TSseUtilCommand::~TSseUtilCommand()
{
  if (fListeners) {
    // NOTE: No need to relase elemens. These should be released by caller side.
    sse_slist_free(fListeners);
  }
}

sse_int
TSseUtilCommand::Execute()
{
  LOG_WARN("This method should be never called.");
  return SSE_E_GENERIC;
}

sse_int
TSseUtilCommand::AddListener(TSseUtilCommandListener& in_listener)
{
  LOG_DEBUG("Add listener=[%p]", &in_listener);
  fListeners = sse_slist_add(fListeners, &in_listener);
  return SSE_E_OK;
}

sse_int
TSseUtilCommand::RemoveListener(TSseUtilCommandListener& in_listener)
{
  fListeners = sse_slist_remove(fListeners, &in_listener);
  return SSE_E_OK;
}

sse_int
TSseUtilCommand::CallOnReadableCallback() const
{
  SSESList* p = fListeners;
  while (p) {
    TSseUtilCommandListener* listener = static_cast<TSseUtilCommandListener*>(sse_slist_data(p));
    if (listener) {
      LOG_DEBUG("listener=[%p]->OnReadable()", listener);
      listener->OnReadable();
    }
    p = sse_slist_next(p);
  }
  return SSE_E_OK;
}

sse_int
TSseUtilCommand::CallOnCompletedCallback(sse_int in_result) const
{
  SSESList* p = fListeners;
  while (p) {
    TSseUtilCommandListener* listener = static_cast<TSseUtilCommandListener*>(sse_slist_data(p));
    if (listener) {
      LOG_DEBUG("listener=[%p]->OnCompleted()", listener);
      listener->OnCompleted(in_result);
    }
    p = sse_slist_next(p);
  }
  return SSE_E_OK;
}

sse_int
TSseUtilCommand::CallOnErrorCallback(sse_int in_error_code, const sse_char* in_message) const
{
  SSESList* p = fListeners;
  while (p) {
    TSseUtilCommandListener* listener = static_cast<TSseUtilCommandListener*>(sse_slist_data(p));
    if (listener) {
      LOG_DEBUG("listener=[%p]->OnError()", listener);
      listener->OnError(in_error_code, in_message);
    }
    p = sse_slist_next(p);
  }
  return SSE_E_OK;
}

