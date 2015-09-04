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

#define TAG "Util:CommandList"
#define LOG_ERROR(format, ...) MOAT_LOG_ERROR(TAG, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...)  MOAT_LOG_WARN(TAG, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)  MOAT_LOG_INFO(TAG, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) MOAT_LOG_DEBUG(TAG, format, ##__VA_ARGS__)
#define LOG_TRACE(format, ...) MOAT_LOG_TRACE(TAG, format, ##__VA_ARGS__)

TSseUtilCommandListCommandListener::TSseUtilCommandListCommandListener()
{
}

TSseUtilCommandListCommandListener::TSseUtilCommandListCommandListener(sse_pointer in_user_data) : TSseUtilCommandListener(in_user_data)
{
}

TSseUtilCommandListCommandListener::~TSseUtilCommandListCommandListener()
{
}
 
sse_int
TSseUtilCommandListCommandListener::OnReadable()
{
  return SSE_E_OK;
}

sse_int
TSseUtilCommandListCommandListener::OnCompleted(sse_int in_result)
{
  LOG_INFO("Command has been completed with [%d]. Let's execute the next command.", in_result);
  TSseUtilCommandList* self = static_cast<TSseUtilCommandList*>(fUserData);
  self->RemoveCompletedCommand();
  return self->Execute();
}

sse_int
TSseUtilCommandListCommandListener::OnError(sse_int in_error_code, const sse_char* in_message)
{
  LOG_ERROR("Command has been failed with [%s. err=(%d)].", in_message, in_error_code);
  return SSE_E_OK;
}


TSseUtilCommandList::TSseUtilCommandList() : fCommands(NULL), fListener(this)
{
}

TSseUtilCommandList::~TSseUtilCommandList()
{
  /* Elements in each node should be released by caller application.
   */
  SSESList* list = fCommands;
  while (list) {
    sse_slist_free(list);
    list = sse_slist_next(list);
  }
}

sse_int
TSseUtilCommandList::Execute()
{
  sse_int err = SSE_E_OK;
  TSseUtilCommand::State current_state = GetState();

  switch (current_state) {
  case TSseUtilCommand::StateInitialized:
  case TSseUtilCommand::StateExecuting:
    // Refer a first command in the command list.
    {
      TSseUtilCommand* command = static_cast<TSseUtilCommand*>(sse_slist_data(fCommands));
      if (command == NULL) {
	LOG_INFO("No more entry in the command list.");
	err = SSE_E_NOENT;
	SetState(TSseUtilCommand::StateCompleted);
	CallOnCompletedCallback(err);
	break;
      }

      // Execute the command.
      LOG_DEBUG("Execute the command. [%p]", command);
      command->AddListener(fListener);
      err = command->Execute();
      if (err != SSE_E_OK) {
	LOG_WARN("command(%p)->Execute() ... failed", command);
	SetState(TSseUtilCommand::StateError);
	CallOnErrorCallback(err, "");
	break;
      }
    }

    SetState(TSseUtilCommand::StateExecuting);
    break;

  case TSseUtilCommand::StateCompleted:
    LOG_WARN("This command was completed.");
    err = SSE_E_ALREADY;
    break;

  default:
    LOG_ERROR("Curresnt state is [%d].", current_state);
    err = SSE_E_GENERIC;
  }

  return err;
}

sse_int
TSseUtilCommandList::AddCommand(TSseUtilCommand& in_command)
{
  if (in_command.GetState() != TSseUtilCommand::StateInitialized) {
    LOG_ERROR("State of command is not initialized. (current = %d)", in_command.GetState());
    return SSE_E_GENERIC;
  }

  SSESList* list = sse_slist_add(fCommands, &in_command);
  if (list == NULL) {
    LOG_ERROR("sse_slist_add() ... failed.");
    return SSE_E_NOMEM;
  }
  fCommands = list;
  return SSE_E_OK;
}

sse_int
TSseUtilCommandList::RemoveCommand(TSseUtilCommand& in_command)
{  if (in_command.GetState() == TSseUtilCommand::StateExecuting) {
    LOG_ERROR("Command is executing now.");
    return SSE_E_GENERIC;
  }

  fCommands = sse_slist_remove(fCommands, &in_command);
  return SSE_E_OK;
}

sse_int
TSseUtilCommandList::RemoveCompletedCommand()
{
  SSESList* list = fCommands;
  while (list) {
    TSseUtilCommand* command = static_cast<TSseUtilCommand*>(sse_slist_data(list));
    if (command->GetState() == TSseUtilCommand::StateCompleted ||
	command->GetState() == TSseUtilCommand::StateError) {
      fCommands = sse_slist_unlink(fCommands, list);
    }
    list = sse_slist_next(list);
  }
  return SSE_E_OK;
}
