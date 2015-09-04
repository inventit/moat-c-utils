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

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <servicesync/moat.h>
#include <sseutils.h>

#define TAG "Util:ShellCommand"
#define LOG_ERROR(format, ...) MOAT_LOG_ERROR(TAG, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...)  MOAT_LOG_WARN(TAG, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)  MOAT_LOG_INFO(TAG, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) MOAT_LOG_DEBUG(TAG, format, ##__VA_ARGS__)
#define LOG_TRACE(format, ...) MOAT_LOG_TRACE(TAG, format, ##__VA_ARGS__)

static void
TSseUtilShellCommand_OnProcessReadable(MoatIOWatcher *in_watcher, sse_pointer in_user_data, sse_int in_desc, sse_int in_event_flags)
{
  sse_int err;
  TSseUtilShellCommand* self;

  if (in_user_data == NULL) {
    LOG_ERROR("Invalid argumet. in_user_data=[%p]", in_user_data);
    return;
  }

  self = (TSseUtilShellCommand*)in_user_data;

  err = TSseUtilStreamBuffer_OnReadable(self->fBuffer, in_desc);
  if (err != SSE_E_OK) {
    moat_io_watcher_stop(in_watcher);
    moat_io_watcher_free(in_watcher);
    self->fIoWatcher = NULL;
    close(in_desc);
    wait(NULL);

    if (err == SSE_E_NOENT) { /* Normal end */
      if (TSseUtilsStramBuffer_HasData(self->fBuffer)) {
	if (self->fOnReadCallback) {
	  self->fOnReadCallback(self, self->fOnReadCallbackUserData);
	}
      }
      self->fState = SSE_UTIL_SHELL_COMMAND_STATE_COMPLETED;
      if (self->fOnCompletedCallback) {
	self->fOnCompletedCallback(self, self->fOnCompletedCallbackUserData, SSE_E_OK);
      }
    } else {
      LOG_ERROR("read() ... failed with [%s]", strerror(errno));
      self->fState = SSE_UTIL_SHELL_COMMAND_STATE_ERROR;
      if (self->fOnErrorCallback) {
	self->fOnErrorCallback(self, self->fOnErrorCallbackUserData, err, strerror(errno));
      }
    }
    return;
  }

  if (self->fOnReadCallback) {
    self->fOnReadCallback(self, self->fOnReadCallbackUserData);
  }
}

TSseUtilShellCommand*
SseUtilShellCommand_New(void)
{
  TSseUtilShellCommand* self = sse_zeroalloc(sizeof(TSseUtilShellCommand));
  if (self == NULL) {
    LOG_ERROR("sse_zeroalloc() ... failed.");
    return NULL;
  }

  sse_int err = TSseUtilShellCommand_Initialize(self);
  if (err != SSE_E_OK) {
    sse_free(self);
    return NULL;
  }

  return self;
}

void
TSseUtilShellCommand_Delete(TSseUtilShellCommand* self)
{
  if (self == NULL) {
    LOG_WARN("Invalid argument. self = [%p]", self);
    return;
  }
  TSseUtilShellCommand_Finalize(self);
  sse_free(self);
}


sse_int
TSseUtilShellCommand_Initialize(TSseUtilShellCommand* self)
{
  if (self == NULL) {
    LOG_ERROR("Invalid argument. self = [%p]", self);
    return SSE_E_INVAL;
  }

  self->fShellCommand = NULL;
  self->fArgs = NULL;
  self->fState = SSE_UTIL_SHELL_COMMAND_STATE_INITIALIZED;
  self->fOnCompletedCallback = NULL;
  self->fOnCompletedCallbackUserData = NULL;
  self->fOnReadCallback = NULL;
  self->fOnReadCallbackUserData = NULL;
  self->fOnErrorCallback = NULL;
  self->fOnErrorCallbackUserData = NULL;
  self->fIoWatcher = NULL;
  self->fBuffer = SseUtilStreamBuffer_New(SSE_UTIL_SHELL_COMMAND_DEFAULT_BUFFER_SIZE);
  if (self->fBuffer == NULL) {
    return SSE_E_NOMEM;
  }

  return SSE_E_OK;
}

void
TSseUtilShellCommand_Finalize(TSseUtilShellCommand* self)
{
  if (self == NULL) {
    LOG_WARN("Invalid argument. self = [%p]", self);
    return;
  }

  if (self->fShellCommand) {
    sse_free(self->fShellCommand);
  }

  if (self->fArgs) {
    SSESList* list = self->fArgs;
    while (self->fArgs) {
      sse_pointer data = sse_slist_data(self->fArgs);
      if (data) {
	sse_free(data);
      }
      self->fArgs = sse_slist_unlink(self->fArgs, list);
      sse_slist_free(list);
      list = self->fArgs;
    }
  }

  if (self->fState == SSE_UTIL_SHELL_COMMAND_STATE_EXECUTING) {
    /* TODO: kill child process and close pipe */
    /*   kill(child_pid, SIGKILL);
     *   close(fd);
     * Need to add above parameters into class field.
     */
  }

  self->fOnCompletedCallback = NULL;
  self->fOnCompletedCallbackUserData = NULL;

  self->fOnReadCallback = NULL;
  self->fOnReadCallbackUserData = NULL;

  self->fOnErrorCallback = NULL;
  self->fOnErrorCallbackUserData = NULL;

  if (self->fIoWatcher) {
    moat_io_watcher_free(self->fIoWatcher);
  }

  self->fState = SSE_UTIL_SHELL_COMMAND_STATE_INITIALIZED;
  if (self->fBuffer) {
    TSseUtilStreamBuffer_Delete(self->fBuffer);
  }
}

sse_int
TSseUtilShellCommand_SetShellCommand(TSseUtilShellCommand* self, const sse_char* in_shell_command)
{
  if (self == NULL || in_shell_command == NULL) {
    LOG_ERROR("Invalid argumets. self=[%p], in_shell_command=[%p]", self, in_shell_command);
    return SSE_E_INVAL;
  }

  self->fShellCommand = sse_strdup(in_shell_command);
  if (self->fShellCommand == NULL) {
    LOG_ERROR("sse_strdup() ... failed.");
    return SSE_E_NOMEM;
  }

  return SSE_E_OK;
}

sse_int
TSseUtilShellCommand_AddArgument(TSseUtilShellCommand* self, const sse_char* in_argument)
{
  sse_char* argument = NULL;
  SSESList* list = NULL;

  if (self == NULL || in_argument == NULL) {
    LOG_ERROR("Invalid argumets. self=[%p], in_argument=[%p]", self, in_argument);
    return SSE_E_INVAL;
  }

  argument = sse_strdup(in_argument);
  if (argument == NULL) {
    LOG_ERROR("sse_strdup() ... failed");
    return SSE_E_NOMEM;
  }

  list = sse_slist_add(self->fArgs, argument);
  if (list == NULL) {
    LOG_ERROR("sse_slist_add() ... failed.");
    sse_free(argument);
    return SSE_E_NOMEM;
  }
  self->fArgs = list;

  return SSE_E_OK;
}

sse_int
TSseUtilShellCommand_Execute(TSseUtilShellCommand* self)
{
  sse_int pipefd[2];
  pid_t cpid;
  sse_char* argv[0];

  if (pipe(pipefd) == -1) {
    LOG_ERROR("pipe() ... failed with [%s]", strerror(errno));
    return SSE_E_GENERIC;
  }

  cpid = fork();
  if (cpid == -1) {
    LOG_ERROR("fork() ... failed with [%s]", strerror(errno));
    return SSE_E_GENERIC;
  }

  if (cpid == 0) { /* child process */
    /* Close readable end of pipe */
    close(pipefd[0]);

    /* Make stdout into writeable end of pipe */
    dup2(pipefd[1], 1);

    /* Execute shell command */
    *argv = sse_zeroalloc(sizeof(sse_char*) * (sse_slist_length(self->fArgs) + 2));
    if (*argv == NULL) {
      LOG_ERROR("sse_zeroalloc() ... failed");
      close(pipefd[1]);
      _exit(EXIT_FAILURE);
    }

    /* Make arguments */
    SSESList* list = self->fArgs;
    sse_char** p = argv;
    *p++ = self->fShellCommand;

    while (list) {
      *p = sse_slist_data(list);
      if (*p == NULL) {
	LOG_ERROR("list elem has no data");
	close(pipefd[1]);
	sse_free(*argv);
	_exit(EXIT_FAILURE);
      }
      list = sse_slist_next(list);
      p++;
    }
    *p = NULL;

    /* Execute shell command */
    if (execvp(self->fShellCommand, argv) == -1) {
      LOG_ERROR("execvp() ... failed with [%s]", strerror(errno));
      close(pipefd[1]);
      sse_free(*argv);
      _exit(EXIT_FAILURE);
    }

    close(pipefd[1]);
    sse_free(*argv);
    _exit(EXIT_SUCCESS);
  }
  /* Close writeable end of pipe */
  close(pipefd[1]);

  /* Monitor readable end of pipe */
  if (fcntl(pipefd[0], F_SETFL, fcntl(pipefd[0], F_GETFL) | O_NONBLOCK) == -1) {
    LOG_ERROR("fcntl() ... failed with [%s].", strerror(errno));
    close(pipefd[0]);
    wait(NULL);
    return SSE_E_GENERIC;
  }
  self->fIoWatcher = moat_io_watcher_new(pipefd[0],
					    TSseUtilShellCommand_OnProcessReadable,
					    self,
					    MOAT_IO_FLAG_READ);
  if (self->fIoWatcher == NULL) {
    LOG_ERROR("moat_io_watcher_new() ... failed.");
    close(pipefd[0]);
    wait(NULL);
    return SSE_E_NOMEM;
  }

  sse_int err = moat_io_watcher_start(self->fIoWatcher);
  if (err != SSE_E_OK) {
    LOG_ERROR("moat_io_watcher_start() ... failed with [%d]", err);
    close(pipefd[0]);
    wait(NULL);
    return err;      
  }
  return SSE_E_OK;
}

sse_int
TSseUtilShellCommand_Read(TSseUtilShellCommand* self, sse_char* out_buffer, sse_uint in_buffer_size, sse_uint* out_data_len)
{
  return TSseUtilStreamBuffer_Read(self->fBuffer, out_buffer, in_buffer_size, out_data_len);
}

sse_int
TSseUtilShellCommand_ReadLine(TSseUtilShellCommand* self, sse_char** out_buffer, sse_bool in_strip_crlf)
{
  return TSseUtilStreamBuffer_ReadLine(self->fBuffer, out_buffer, in_strip_crlf);
}

sse_int
TSseUtilShellCommand_SetOnComplatedCallback(TSseUtilShellCommand* self, TSseUtilShellCommand_OnCompletedCallback in_callback, sse_pointer in_user_data)
{
  LOG_DEBUG("Regiser callback = [%p], user data = [%p]", in_callback, in_user_data);
  self->fOnCompletedCallback = in_callback;
  self->fOnCompletedCallbackUserData = in_user_data;
  return SSE_E_OK;
}

sse_int
TSseUtilShellCommand_RemoveOnComplatedCallback(TSseUtilShellCommand* self, sse_pointer in_user_data)
{
  LOG_DEBUG("Unregiser callback = [%p]", self->fOnCompletedCallback);
  self->fOnCompletedCallback = NULL;
  self->fOnCompletedCallbackUserData = NULL;
  return SSE_E_OK;
}

sse_int
TSseUtilShellCommand_SetOnReadCallback(TSseUtilShellCommand* self, TSseUtilShellCommand_OnReadCallback in_callback, sse_pointer in_user_data)
{
  LOG_DEBUG("Regiser callback = [%p]", in_callback);
  self->fOnReadCallback = in_callback;
  self->fOnReadCallbackUserData = in_user_data;
  return SSE_E_OK;
}

sse_int
TSseUtilShellCommand_RemoveOnReadCallback(TSseUtilShellCommand* self, sse_pointer out_user_data)
{
  LOG_DEBUG("Unregiser callback = [%p]", self->fOnCompletedCallback);
  self->fOnReadCallback = NULL;
  self->fOnReadCallbackUserData = NULL;
  return SSE_E_OK;
}

sse_int
TSseUtilShellCommand_SetOnErrorCallback(TSseUtilShellCommand* self, TSseUtilShellCommand_OnErrorCallback in_callback, sse_pointer in_user_data)
{
  LOG_DEBUG("Regiser callback = [%p]", in_callback);
  self->fOnErrorCallback = in_callback;
  self->fOnErrorCallbackUserData = in_user_data;
  return SSE_E_OK;
}

SseUtilShellCommandState
TSseUtilShellCommand_GetState(TSseUtilShellCommand* self)
{
  return self->fState;
}
