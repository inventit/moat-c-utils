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

#include <stdexcept>
#include <servicesync/moat.h>
#include <sseutils.h>

#define TAG "Util:SystemCommand"
#define LOG_ERROR(format, ...) MOAT_LOG_ERROR(TAG, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...)  MOAT_LOG_WARN(TAG, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)  MOAT_LOG_INFO(TAG, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) MOAT_LOG_DEBUG(TAG, format, ##__VA_ARGS__)
#define LOG_TRACE(format, ...) MOAT_LOG_TRACE(TAG, format, ##__VA_ARGS__)

TSseUtilCommandShell::TSseUtilCommandShell()
{
  sse_int err;

  err = TSseUtilShellCommand_Initialize(&fShellCommand);
  if (err != SSE_E_OK) {
    LOG_ERROR("TSseUtilShellCommand_Initialize() ... failed with [%d].", err);
    throw std::runtime_error("TSseUtilShellCommand_Initialize()");
  }

  TSseUtilShellCommand_SetOnComplatedCallback(&fShellCommand,
					      TSseUtilCommandShell::OnCompletedCallback,
					      this);
  TSseUtilShellCommand_SetOnReadCallback(&fShellCommand,
					 TSseUtilCommandShell::OnRead,
					 this);
  TSseUtilShellCommand_SetOnErrorCallback(&fShellCommand,
					  TSseUtilCommandShell::OnError,
					  this);
}

TSseUtilCommandShell::~TSseUtilCommandShell()
{
  TSseUtilShellCommand_Finalize(&fShellCommand);
}

sse_int
TSseUtilCommandShell::SetCommand(const sse_char* in_command)
{
  return TSseUtilShellCommand_SetShellCommand(&fShellCommand, in_command);
}

sse_int
TSseUtilCommandShell::AddArgument(const sse_char* in_argument)
{
  return TSseUtilShellCommand_AddArgument(&fShellCommand, in_argument);
}

sse_int
TSseUtilCommandShell::Execute()
{
  SetState(StateExecuting);
  return TSseUtilShellCommand_Execute(&fShellCommand);
}

void
TSseUtilCommandShell::OnCompletedCallback(TSseUtilShellCommand* in_self, sse_pointer in_user_data, sse_int in_result)
{
  LOG_DEBUG("in_self=[%p], in_user_data=[%p], in_result=[%d]", in_self, in_user_data, in_result);

  TSseUtilCommandShell* self = static_cast<TSseUtilCommandShell*>(in_user_data);
  self->SetState(StateCompleted);
  self->CallOnCompletedCallback(in_result);
}

void
TSseUtilCommandShell::OnRead(TSseUtilShellCommand* in_self, sse_pointer in_user_data)
{
  LOG_DEBUG("in_self=[%p], in_user_data=[%p]", in_self, in_user_data);

  TSseUtilCommandShell* self = (TSseUtilCommandShell*)in_user_data;
  self->CallOnReadableCallback();
}

void
TSseUtilCommandShell::OnError(TSseUtilShellCommand* in_self, sse_pointer in_user_data, sse_int in_error_code, const sse_char* in_message)
{
  LOG_ERROR("Message=[%s], Code=[%d].", in_message, in_error_code);

  TSseUtilCommandShell* self = (TSseUtilCommandShell*)in_user_data;
  self->SetState(StateError);
  self->CallOnErrorCallback(in_error_code, in_message);
}

sse_int
TSseUtilCommandShell::Read(sse_char* out_buffer, sse_uint in_buffer_size, sse_uint* out_data_len)
{
  return TSseUtilShellCommand_Read(&fShellCommand, out_buffer, in_buffer_size, out_data_len);
}

sse_int
TSseUtilCommandShell::ReadLine(sse_char** out_buffer, sse_bool in_strip_crlf)
{
  return TSseUtilShellCommand_ReadLine(&fShellCommand, out_buffer, in_strip_crlf);
}
