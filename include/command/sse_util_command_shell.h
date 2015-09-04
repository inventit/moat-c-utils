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

#ifndef __SSE_UTIL_COMMAND_SHELL_H__
#define __SSE_UTIL_COMMAND_SHELL_H__

class TSseUtilCommand;

class TSseUtilCommandShell : public TSseUtilCommand {
 public:
  TSseUtilCommandShell();
  virtual ~TSseUtilCommandShell();
  virtual sse_int SetCommand(const sse_char* in_command);
  virtual sse_int AddArgument(const sse_char* in_argument);
  virtual sse_int Execute();

  sse_int Read(sse_char* out_buffer, sse_uint in_buffer_size, sse_uint* out_data_len);
  sse_int ReadLine(sse_char** out_buffer, sse_bool in_strip_crlf);

  static void OnCompletedCallback(TSseUtilShellCommand* in_self, sse_pointer in_user_data, sse_int in_result);
  static void OnRead(TSseUtilShellCommand* in_self, sse_pointer in_user_data);
  static void OnError(TSseUtilShellCommand* in_self, sse_pointer in_user_data, sse_int in_error_code, const sse_char* in_message);

 private:
  TSseUtilShellCommand fShellCommand;
};

#endif // __SSE_UTIL_COMMAND_SHELL_H__
