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

#ifndef __SSE_UTIL_SHELL_COMMAND_H__
#define __SSE_UTIL_SHELL_COMMAND_H__

SSE_BEGIN_C_DECLS

#include <sys/types.h>

#define SSE_UTIL_SHELL_COMMAND_DEFAULT_BUFFER_SIZE (2048)

/**
 * @brief State of external shell command execution
 */
enum SseUtilShellCommandState_ {
  SSE_UTIL_SHELL_COMMAND_STATE_INITIALIZED,  /** Initialized */
  SSE_UTIL_SHELL_COMMAND_STATE_EXECUTING,    /** Shell command is executiong now */
  SSE_UTIL_SHELL_COMMAND_STATE_COMPLETED,    /** Shell command has been completed with successfully */
  SSE_UTIL_SHELL_COMMAND_STATE_ERROR,        /** Shell command has been aborted with failure */
  SSE_UTIL_SHELL_COMMANDS
};
typedef enum SseUtilShellCommandState_ SseUtilShellCommandState;

/**
 * @brief Shell command class
 *
 * Class to execute external shell command asynchronously
 *
 */
struct TSseUtilShellCommand_ {
  sse_char* fShellCommand;          /** Shell command */
  SSESList* fArgs;                  /** Arguments of shell command */
  SseUtilShellCommandState fState;  /** State of execution */

  void (*fOnCompletedCallback)(struct TSseUtilShellCommand_*, sse_pointer, sse_int); /** Callback called when on complete */
  sse_pointer* fOnCompletedCallbackUserData;                                         /** User data passed when on complete */
  void (*fOnReadCallback)(struct TSseUtilShellCommand_*, sse_pointer); /** Callback called when on read */
  sse_pointer* fOnReadCallbackUserData;                                /** User data passed when on read */
  void (*fOnErrorCallback)(struct TSseUtilShellCommand_*, sse_pointer, sse_int, const sse_char*); /** Callback called when on error */
  sse_pointer* fOnErrorCallbackUserData;                                                          /** User data passed when on error */

  MoatIOWatcher* fIoWatcher; /** I/O watcher for file descripter to read a result of shell command */

  TSseUtilStreamBuffer* fBuffer; /** Buffer to store a result of shell command */

  pid_t fChildPid; /** PID of child process */
};
typedef struct TSseUtilShellCommand_ TSseUtilShellCommand;

/**
 * @brief Prototype of callback function on complete
 *
 * Prototype of callback function on complete
 *
 * @param [in]  self          instance
 * @param [in]  in_user_data  User data
 * @param [in]  in_result     result of shell command
 *
 * @return None
 *
 */
typedef void (*TSseUtilShellCommand_OnCompletedCallback)(TSseUtilShellCommand* self, sse_pointer in_user_data, sse_int in_result);

/**
 * @brief Prototype of callback function on read
 *
 * Prototype of callback function on read a result of shell command.
 * Please read a result using TSseUtilShellCommand_Read() or TSseUtilShellCommand_ReadLine()
 * in this callback.
 *
 * @param [in]  self          instance
 * @param [in]  in_user_data  User data
 *
 * @return None
 *
 */
typedef void (*TSseUtilShellCommand_OnReadCallback)(TSseUtilShellCommand* self, sse_pointer in_user_data);

/**
 * @brief Prototype of callback function on error
 *
 * Prototype of callback function on error
 *
 * @param [in]  self          instance
 * @param [in]  in_user_data  User data
 * @param [in]  in_error_code Error code
 * @param [in]  in_message    Error message
 *
 * @return None
 *
 */
typedef void (*TSseUtilShellCommand_OnErrorCallback)(TSseUtilShellCommand* self, sse_pointer in_user_data, sse_int in_error_code, const sse_char* in_message);

/**
 * @brief Create instance of TSseUtilShellCommand
 *
 * Create instance of TSseUtilShellCommand
 *
 * @retval != NULL Success
 * @retval == NULL Failure
 *
 */
TSseUtilShellCommand*
SseUtilShellCommand_New(void);

/**
 * @brief Delete instance of TSseUtilShellCommand
 *
 * Delete instance of TSseUtilShellCommand
 *
 * @param [in]  self          instance
 *
 * @return None
 *
 */
void
TSseUtilShellCommand_Delete(TSseUtilShellCommand* self);

/**
 * @brief Initialize instance of TSseUtilShellCommand
 *
 * Delete instance of TSseUtilShellCommand
 *
 * @param [in]  self          instance
 *
 * @retval SSE_E_OK  Success
 * @retval others    Failure
 *
 */
sse_int
TSseUtilShellCommand_Initialize(TSseUtilShellCommand* self);


/**
 * @brief Finalize instance of TSseUtilShellCommand
 *
 * Finalize instance of TSseUtilShellCommand
 *
 * @param [in]  self          instance
 *
 * @retval SSE_E_OK  Success
 * @retval others    Failure
 *
 */
void
TSseUtilShellCommand_Finalize(TSseUtilShellCommand* self);

/**
 * @brief Set a shell command
 *
 * Set a shell command
 *
 * @param [in]  self              instance
 * @param [in]  in_shell_command  shell command (e.g. cat, ls and so on)
 *
 * @retval SSE_E_OK  Success
 * @retval others    Failure
 *
 */
sse_int
TSseUtilShellCommand_SetShellCommand(TSseUtilShellCommand* self,
				     const sse_char* in_shell_command);

/**
 * @brief Add argument shell command
 *
 * Set a shell command. Able to add only one argument. If you would like to
 * specify multiple argument, please call this function more than once.
 * 
 * @code{.c}
 * TSseUtilShellCommand_SetShellCommand(self, "tail");
 * TSseUtilShellCommand_AddArgument(self, "-f");
 * TSseUtilShellCommand_AddArgument(self, "/var/log/syslog");
 * @endcode
 *
 * @param [in]  self              instance
 * @param [in]  in_shell_command  shell command (e.g. cat, ls and so on)
 *
 * @retval SSE_E_OK  Success
 * @retval others    Failure
 *
 */
sse_int
TSseUtilShellCommand_AddArgument(TSseUtilShellCommand* self,
				 const sse_char* in_argument);

/**
 * @brief Execute a external shell command
 *
 * Execute a external shell command
 *
 * @param [in]  self              instance
 *
 * @retval SSE_E_OK  Success
 * @retval others    Failure
 *
 */
sse_int
TSseUtilShellCommand_Execute(TSseUtilShellCommand* self);

/**
 * @brief Set on comlete callback
 *
 * Set a callback function called when on complete.
 *
 * @param [in]  self              instance
 * @param [in]  in_callback       Callback function
 * @param [in]  in_user_data      User data
 *
 * @retval SSE_E_OK  Success
 * @retval others    Failure
 *
 */
sse_int
TSseUtilShellCommand_SetOnComplatedCallback(TSseUtilShellCommand* self,
					    TSseUtilShellCommand_OnCompletedCallback in_callback,
					    sse_pointer in_user_data);

/**
 * @brief Remove on comlete callback
 *
 * Remove a callback function called when on complete.
 *
 * @param [in]  self              instance
 * @param [in]  in_user_data      User data
 *
 * @retval SSE_E_OK  Success
 * @retval others    Failure
 *
 */
sse_int
TSseUtilShellCommand_RemoveOnComplatedCallback(TSseUtilShellCommand* self,
					       sse_pointer out_user_data);

/**
 * @brief Set on read callback
 *
 * Set a callback function called when a result of shell command has been read.
 *
 * @param [in]  self              instance
 * @param [in]  in_callback       Callback function
 * @param [in]  in_user_data      User data
 *
 * @retval SSE_E_OK  Success
 * @retval others    Failure
 *
 */
sse_int
TSseUtilShellCommand_SetOnReadCallback(TSseUtilShellCommand* self,
				       TSseUtilShellCommand_OnReadCallback in_callback,
				       sse_pointer in_user_data);

/**
 * @brief Remove on read callback
 *
 * Remove a callback function called when a result of shell command has been read.
 *
 * @param [in]  self              instance
 * @param [in]  in_user_data      User data
 *
 * @retval SSE_E_OK  Success
 * @retval others    Failure
 *
 */
sse_int
TSseUtilShellCommand_RemoveOnReadCallback(TSseUtilShellCommand* self,
					  sse_pointer in_user_data);

/**
 * @brief Set on error callback
 *
 * Set a callback function called when a result of shell command has been read.
 *
 * @param [in]  self              instance
 * @param [in]  in_callback       Callback function
 * @param [in]  in_user_data      User data
 *
 * @retval SSE_E_OK  Success
 * @retval others    Failure
 *
 */
sse_int
TSseUtilShellCommand_SetOnErrorCallback(TSseUtilShellCommand* self,
					TSseUtilShellCommand_OnErrorCallback in_callback,
					sse_pointer in_user_data);

/**
 * @brief Remove on error callback
 *
 * Remove a callback function called when a result of shell command has been read.
 *
 * @param [in]  self              instance
 * @param [in]  in_user_data      User data
 *
 * @retval SSE_E_OK  Success
 * @retval others    Failure
 *
 */
sse_int
TSseUtilShellCommand_RemoveOnErrorCallback(TSseUtilShellCommand* self,
					   sse_pointer out_user_data);

/**
 * @brief Get current state of execution
 *
 * Get current state of execution.
 *
 * @param [in]  self              instance
 *
 * @return Cuurent state
 *
 */
SseUtilShellCommandState
TSseUtilShellCommand_GetState(TSseUtilShellCommand* self);

/**
 * @brief Read a result of shell command.
 *
 * Read a result of shell command. This function should be called in on read callback.
 * Caller side have to allocate memory area and pass the size of buffuer.
 *
 * @param [in]  self              instance
 * @param [out] out_buffer        Buffer to store a result of shell command
 * @param [in]  in_buffer_size    Size of out_buffer
 * @param [in]  out_data_len      Size of actual read
 *
 * @retval SSE_E_OK  Success
 * @retval others    Failure
 *
 */
sse_int
TSseUtilShellCommand_Read(TSseUtilShellCommand* self,
			  sse_char* out_buffer,
			  sse_uint in_buffer_size,
			  sse_uint* out_data_len);

/**
 * @brief Read a result of shell command by line.
 *
 * Read a result of shell command by line. This function should be called in on read callback.
 * This function allocates a memory area and return it via out_buffer. So caller size have to
 * release it with sse_free().
 * If sse_true is set in in_strip_crlf, CR and LF will be extracted from the result of shell command.
 *
 * @param [in]  self              instance
 * @param [out] out_buffer        Result of shell command
 * @param [in]  in_strip_crlf     Flag for strip CR and LF.
 *
 * @retval SSE_E_OK  Success
 * @retval others    Failure
 *
 */
sse_int
TSseUtilShellCommand_ReadLine(TSseUtilShellCommand* self,
			      sse_char** out_buffer,
			      sse_bool in_strip_crlf);

SSE_END_C_DECLS

#endif // __SSE_UTIL_SHELL_COMMAND_H__
