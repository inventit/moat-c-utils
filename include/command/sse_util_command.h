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

#ifndef __SSE_UTIL_COMMAND_H__
#define __SSE_UTIL_COMMAND_H__

class TSseUtilCommandListener;

class TSseUtilCommand {
 public:
  TSseUtilCommand();
  virtual ~TSseUtilCommand();
  virtual sse_int Execute();
  virtual sse_int AddListener(TSseUtilCommandListener& in_listener);
  virtual sse_int RemoveListener(TSseUtilCommandListener& in_listener);

 protected:
  virtual sse_int CallOnReadableCallback() const;
  virtual sse_int CallOnCompletedCallback(sse_int in_result) const;
  virtual sse_int CallOnErrorCallback(sse_int in_error_code, const sse_char* in_message) const;

 public:
  enum State {
    StateInitialized,
    StateExecuting,
    StateCompleted,
    StateError
  };
  State GetState() const { return fState; }
 protected:
  void SetState(State in_state) { fState = in_state; }

 private:
  State fState;
  SSESList* fListeners;
};

#endif //__SSE_UTIL_COMMAND_H__
