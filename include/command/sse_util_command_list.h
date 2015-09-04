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

#ifndef __SSE_UTIL_COMMAND_LIST_H__
#define __SSE_UTIL_COMMAND_LIST_H__

class TSseUtilCommandListener;

class TSseUtilCommandListCommandListener : public TSseUtilCommandListener {
 public:
  TSseUtilCommandListCommandListener();
  explicit TSseUtilCommandListCommandListener(sse_pointer in_user_data);
  virtual ~TSseUtilCommandListCommandListener();
  sse_int OnReadable();
  sse_int OnCompleted(sse_int in_result);
  sse_int OnError(sse_int in_error_code, const sse_char* in_message);
};


class TSseUtilCommandList: public TSseUtilCommand {
 public:
  TSseUtilCommandList();
  virtual ~TSseUtilCommandList();
  virtual sse_int Execute();
  virtual sse_int AddCommand(TSseUtilCommand& in_command);
  virtual sse_int RemoveCommand(TSseUtilCommand& in_command);
  virtual sse_int RemoveCompletedCommand();

 private:
  SSESList* fCommands;
  TSseUtilCommandListCommandListener fListener;
};

#endif //__SSE_UTIL_COMMAND_LIST_H__
