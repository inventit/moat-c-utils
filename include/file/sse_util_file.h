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

#ifndef __SSE_UTIL_FILE_H__
#define __SSE_UTIL_FILE_H__

SSE_BEGIN_C_DECLS


/**
 * @brief Move a file to a new location
 *
 * Move a file to a new location. If the directory holding the destination file is created if it does not exist.
 * If the destination file exists, this function will operwirte it. 
 *
 * @param [in] in_src an existing file to copy.
 * @param [in] in_dst the new file.
 *
 * @retval SSE_E_OK Success
 * @retval others Failure
 */
sse_int
SseUtilFile_MoveFile(MoatValue *in_src, MoatValue *in_dst);

/**
 * @brief Delete a file
 *
 * Delete a speccified file.
 *
 * @param [in] in_file an existing file to delete.
 *
 * @retval SSE_E_OK Success
 * @retval others Failure
 */
sse_int
SseUtilFile_DeleteFile(MoatValue *in_file);

/**
 * @brief Create a directory
 *
 * Create a directory, including any necessary but nonexistent parent directories.
 *
 * @param [in] in_dir an directory to create.
 *
 * @retval SSE_E_OK Success
 * @retval others Failure
 */
sse_int
SseUtilFile_MakeDirectory(MoatValue *in_dir);

/**
 * @brief Extract a directory path from the specified file path.
 *
 * Extract a directory path from the specified file path.
 * out_dirname must be released by moat_value_free() by caller side.
 *
 * @param [in]  in_pathe an file path
 * @param [out] out_dirname the parentdirectory name 
 *
 * @retval SSE_E_OK Success
 * @retval others Failure
 */
sse_int
SseUtilFile_GetDirectoryPath(MoatValue *in_path, MoatValue **out_dirname);

/**
 * @brief Extract a file name from the specified file path.
 *
 * Extract a file name from the specified file path.
 * out_filename must be released by moat_value_free() by caller side.
 *
 * @param [in]  in_pathe an file path
 * @param [out] out_filename the parentdirectory name 
 *
 * @retval SSE_E_OK Success
 * @retval others Failure
 */
sse_int
SseUtilFile_GetFileName(MoatValue *in_path, MoatValue **out_filename);

/**
 * @brief Test if the specified directory exists
 *
 * Test if the specified directory exists.
 *
 * @param [in] in_path an directory path
 *
 * @retval sse_true  exist
 * @retval sse_false NOT exist
 */
sse_bool
SseUtilFile_IsDirectory(MoatValue *in_path);

/**
 * @brief Test if the specified file exists
 *
 * Test if the specified file exists
 *
 * @param [in] in_path an file path
 *
 * @retval sse_true  exist
 * @retval sse_false NOT exist
 */
sse_bool
SseUtilFile_IsFile(MoatValue *in_path);

/**
 * @brief Test if the specified entry exists
 *
 * Test if the specified entry exists
 *
 * @param [in] in_path an file path
 *
 * @retval sse_true  exist
 * @retval sse_false NOT exist
 */
sse_bool
SseUtilFile_IsExist(MoatValue *in_path);

SSE_END_C_DECLS

#endif /*__SSE_UTIL_FILE_H__*/
