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

#ifndef __SSE_UTIL_COMMAND_LISTENER_H__
#define __SSE_UTIL_COMMAND_LISTENER_H__

class TSseUtilCommandListener
{
 public:
  TSseUtilCommandListener();
  explicit TSseUtilCommandListener(sse_pointer in_user_data);
  virtual ~TSseUtilCommandListener();
  virtual sse_int OnReadable();
  virtual sse_int OnCompleted(sse_int in_result);
  virtual sse_int OnError(sse_int in_error_code, const sse_char* in_message);
  const sse_pointer GetUserData() const { return fUserData; }

 protected:
  sse_pointer fUserData;
};

#endif // __SSE_UTIL_COMMAND_LISTENER_H__
