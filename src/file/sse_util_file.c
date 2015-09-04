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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <servicesync/moat.h>
#include <sseutils.h>

#define TAG "Util:File"
#define LOG_ERROR(format, ...) MOAT_LOG_ERROR(TAG, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...)  MOAT_LOG_WARN(TAG, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)  MOAT_LOG_INFO(TAG, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) MOAT_LOG_DEBUG(TAG, format, ##__VA_ARGS__)
#define LOG_TRACE(format, ...) MOAT_LOG_TRACE(TAG, format, ##__VA_ARGS__)

sse_int
SseUtilFile_MoveFile(MoatValue *in_src, MoatValue *in_dst)
{
  sse_int err = SSE_E_GENERIC;
  MoatValue *dst_dir = NULL;
  sse_char src_file[1024];
  sse_char dst_file[1024];
  sse_char *str;
  sse_uint len;


  if (in_src == NULL) {
    LOG_ERROR("Invalid argument, in_src=[%p]", in_src);
    return SSE_E_INVAL;
  }
  if (in_dst == NULL) {
    LOG_ERROR("Invalid argument, in_dst=[%p]", in_dst);
    return SSE_E_INVAL;
  }

  if (SseUtilFile_IsFile(in_src) == sse_false) {
    LOG_ERROR("No specified source file.");
    return SSE_E_INVAL;
  }

  /* Test the destination directory, then if it does not exist, make it.
   */
  err = SseUtilFile_GetDirectoryPath(in_dst, &dst_dir);
  if (err != SSE_E_OK) {
    LOG_ERROR("SseUtilFile_GetDirectoryPath() has been failed with [%s].", sse_get_error_string(err));
    MOAT_VALUE_DUMP_ERROR(TAG, in_dst);
    return err;
  }

  if (SseUtilFile_IsDirectory(dst_dir) == sse_false) {
    LOG_INFO("Create a new directory...");
    MOAT_VALUE_DUMP_INFO(TAG, dst_dir);
    err = SseUtilFile_MakeDirectory(dst_dir);
    if (err != SSE_E_OK) {
      LOG_ERROR("SseUtilFile_MakeDirectory() has been failed with [%s].", sse_get_error_string(err));
      MOAT_VALUE_DUMP_ERROR(TAG, dst_dir);
      moat_value_free(dst_dir);
      return err;
    }
  }
  moat_value_free(dst_dir);

  /* Rename the file
   */
  err = moat_value_get_string(in_src, &str, &len);
  if (err != SSE_E_OK) {
    LOG_ERROR("moat_value_get_string() has been failed with [%s].", sse_get_error_string(err));
    return err;
  }
  sse_strncpy(src_file, str, len);
  src_file[len] = '\0';

  err = moat_value_get_string(in_dst, &str, &len);
  if (err != SSE_E_OK) {
    LOG_ERROR("moat_value_get_string() has been failed with [%s].", sse_get_error_string(err));
    return err;
  }
  sse_strncpy(dst_file, str, len);
  dst_file[len] = '\0';

  LOG_INFO("Rename the file [%s] -> [%s].", src_file, dst_file);
  err = rename(src_file, dst_file);
  if (err != 0) {
    LOG_ERROR("rename([%s], [%s]) has been failed with [%s].", src_file, dst_file, strerror(errno));
    if (errno == EACCES) {
      return SSE_E_ACCES;
    } else if (errno == EINVAL) {
      return SSE_E_INVAL;
    } else if (errno == ENOENT) {
      return SSE_E_NOENT;
    }
    return SSE_E_GENERIC;
  }
  
  return SSE_E_OK;

}

sse_int
SseUtilFile_DeleteFile(MoatValue *in_file)
{
  sse_int err;
  sse_char *str;
  sse_uint len;
  sse_char path[1024];

  if (in_file == NULL) {
    LOG_ERROR("Invalid argument, in_file=[%p].", in_file);
    return SSE_E_INVAL;
  }

  if (!SseUtilFile_IsFile(in_file)) {
    LOG_ERROR("The specified file dose not exist or does not regular file.");
    MOAT_VALUE_DUMP_ERROR(TAG, in_file);
    return SSE_E_NOENT;
  }

  err = moat_value_get_string(in_file, &str, &len);
  if (err != SSE_E_OK) {
    LOG_ERROR("moat_value_get_string() has been failed with [%s].", sse_get_error_string(err));
    return err;
  }
  if (len >= sizeof(path)) {
    LOG_ERROR("Too long file path (%d).", len);
    return SSE_E_INVAL;
  }
  sse_strncpy(path, str, len);
  path[len] = '\0';

  LOG_INFO("Delete file=[%s].", path);
  err = unlink(path);
  if (err != 0) {
    LOG_ERROR("unlink(path=[%s]) has been failed with [%s].", path, strerror(errno));
    if (errno == EACCES) {
      return SSE_E_ACCES;
    } else if (errno == ENOENT) {
      return SSE_E_NOENT;
    }
    return SSE_E_GENERIC;
  }

  return SSE_E_OK;
}

sse_int
SseUtilFile_MakeDirectory(MoatValue *in_dir)
{
  sse_int err;
  MoatValue *parent;
  sse_char path[1024];
  sse_char *str;
  sse_uint len;

  if (in_dir == NULL) {
    LOG_ERROR("Invalid argument, in_dir=[%p].", in_dir);
    return SSE_E_INVAL;
  }

  if (SseUtilFile_IsDirectory(in_dir) == sse_true) {
    LOG_DEBUG("The specified directory already exists, so noting to do.");
    return SSE_E_OK;
  }

  if (SseUtilFile_IsExist(in_dir) == sse_true) {
    LOG_ERROR("Same name of file already exists.");
    return SSE_E_ACCES;
  }

  /* Create a parent directory if it does not exist.
   */
  err = SseUtilFile_GetDirectoryPath(in_dir, &parent);
  if (err != SSE_E_OK) {
    LOG_ERROR("SseUtilFile_GetDirectoryPath() has been failed with [%s].", sse_get_error_string(err));
    return err;
  }

  if (SseUtilFile_IsDirectory(parent) == sse_false) {
    MOAT_VALUE_DUMP_INFO(TAG, parent);
    err = SseUtilFile_MakeDirectory(parent);
    if (err != SSE_E_OK) {
      LOG_ERROR("SseUtilFile_MakeDirectory() has been failed with [%s].", sse_get_error_string(err));
      moat_value_free(parent);
      return err;
    }
  }
  moat_value_free(parent);

  /* Create a directory.
   */
  err = moat_value_get_string(in_dir, &str, &len);
  if (err != SSE_E_OK) {
    LOG_ERROR("moat_value_get_string() has been failed with [%s].", sse_get_error_string(err));
    MOAT_VALUE_DUMP_ERROR(TAG, in_dir);
    return SSE_E_INVAL;
  }

  if (len >= sizeof(path)) {
    LOG_ERROR("The specified path is too long (%s).", len);
    return SSE_E_INVAL;
  }

  strncpy(path, str, len);
  path[len] = '\0';
  LOG_DEBUG("path=[%s]", path);

  err = mkdir(path, 0750);
  if (err != 0) {
    LOG_ERROR("mkdir(path=[%s]) has been failed with [%s]", path, strerror(errno));
    if (errno == EACCES) {
      return SSE_E_ACCES;
    } else if (errno == ENOENT) {
      return SSE_E_NOENT;
    }
    return SSE_E_GENERIC;
  }

  return SSE_E_OK;
}

sse_int
SseUtilFile_GetDirectoryPath(MoatValue *in_path,
                             MoatValue **out_dirname)
{
  sse_int err;
  sse_char *top;
  sse_char *end;
  sse_uint len;
  sse_char *p;

  if (in_path == NULL) {
    LOG_ERROR("Invalid argument, in_file = [%p].", in_path);
    return SSE_E_INVAL;
  }

  if (out_dirname == NULL) {
    LOG_ERROR("Invalid argument, in_file = [%p].", out_dirname);
    return SSE_E_INVAL;
  }

  err = moat_value_get_string(in_path, &top, &len);
  if (err != SSE_E_OK) {
    LOG_ERROR("moat_value_get_string() has been failed with [%s].", sse_get_error_string(err));
    return SSE_E_INVAL;
  }
  end = top + len;
  if (top == end) {
    LOG_ERROR("No directory was found in the specified path.");
    return SSE_E_NOENT;
  }
  
  for (p = (end - 1); p >= top; p--) {
    if (*p == '/') {
      break;
    }
  }

  if (p == top) {
    *out_dirname = moat_value_new_string("/", 0, sse_true);
  } else if (p < top) {
    LOG_ERROR("No directory was found in the specified path.");
    return SSE_E_NOENT;
  } else {
    *out_dirname = moat_value_new_string(top, p - top, sse_true);
  }
  if (*out_dirname == NULL) {
    LOG_ERROR("moat_value_new_string() has been failed.");
    return SSE_E_NOMEM;
  }

  return SSE_E_OK;
}

sse_int
SseUtilFile_GetFileName(MoatValue *in_path, MoatValue **out_filename)
{
  sse_int err;
  sse_char *top;
  sse_char *end;
  sse_uint len;
  sse_char *p;

  if (in_path == NULL) {
    LOG_ERROR("Invalid argument, in_path=[%p].", in_path);
    return SSE_E_INVAL;
  }
  if (out_filename == NULL) {
    LOG_ERROR("Invalid argument, out_filename=[%p].", out_filename);
    return SSE_E_INVAL;
  }

  err = moat_value_get_string(in_path, &top, &len);
  if (err != SSE_E_OK) {
    LOG_ERROR("moat_value_get_string() has been failed with [%s].", sse_get_error_string(err));
    return SSE_E_INVAL;
  }

  end = top + len;

  if ((top == end) || (*(end - 1) == '/')) {
    LOG_INFO("No file name has been found in ...");
    MOAT_VALUE_DUMP_INFO(TAG, in_path);
    return SSE_E_NOENT;
  }

  for (p = (end - 1); p >= top; p--) {
    if (*p == '/') {
      p++;
      *out_filename = moat_value_new_string(p, end - p, sse_true);
      if (*out_filename == NULL) {
        LOG_ERROR("moat_value_new_string() has been failed.");
        return SSE_E_NOMEM;
      }
      return SSE_E_OK;
    }
  }
  *out_filename = moat_value_new_string(top, end - top, sse_true);
  if (*out_filename == NULL) {
    LOG_ERROR("moat_value_new_string() has been failed.");
    return SSE_E_NOMEM;
  }
  return SSE_E_OK;  
}

static sse_int
SseUtilFile_GetFileType(MoatValue *in_path, mode_t *out_mode)
{
  sse_int err;
  sse_char *str;
  sse_uint len;
  sse_char path[1024];
  struct stat sb;

  if (in_path == NULL) {
    LOG_ERROR("Invalid argument, in_path=[%p].", in_path);
    return SSE_E_INVAL;
  }

  err = moat_value_get_string(in_path, &str, &len);
  if (err != SSE_E_OK) {
    LOG_ERROR("moat_value_get_string() has been failed with [%s].", sse_get_error_string(err));
    MOAT_VALUE_DUMP_ERROR(TAG, in_path);
    return SSE_E_INVAL;
  }

  if (len >= sizeof(path)) {
    LOG_ERROR("The specified path is too long (%s).", len);
    return SSE_E_INVAL;
  }

  strncpy(path, str, len);
  path[len] = '\0';
  LOG_DEBUG("path=[%s]", path);

  err = stat(path, &sb);
  if (err != 0) {
    if (errno == EACCES) {
      LOG_ERROR("stat(path=[%s]) has been failed with [%s].", path, strerror(errno));
      return SSE_E_ACCES;
    } else if (errno == ENOENT) {
      LOG_DEBUG("stat(path=[%s]) has been failed with [%s].", path, strerror(errno));
      return SSE_E_NOENT;
    }
    LOG_ERROR("stat(path=[%s]) has been failed with [%s].", path, strerror(errno));
    return SSE_E_GENERIC;
  }
  LOG_DEBUG("sb.st_mode=[0x%x]", (sb.st_mode & S_IFMT));
  *out_mode = (sb.st_mode & S_IFMT);
  return SSE_E_OK;
}

sse_bool
SseUtilFile_IsDirectory(MoatValue *in_path)
{
  sse_int err;
  mode_t mode;

  err = SseUtilFile_GetFileType(in_path, &mode);
  if (err != SSE_E_OK) {
    if (err != SSE_E_NOENT) {
      LOG_ERROR("SseUtilFile_GetFileType() has been failed with [%s].", sse_get_error_string(err));
    }
    return sse_false;
  }
  return S_ISDIR(mode);
}

sse_bool
SseUtilFile_IsFile(MoatValue *in_path)
{
  sse_int err;
  mode_t mode;

  err = SseUtilFile_GetFileType(in_path, &mode);
  if (err != SSE_E_OK) {
    if (err != SSE_E_NOENT) {
      LOG_ERROR("SseUtilFile_GetFileType() has been failed with [%s].", sse_get_error_string(err));
    }
    return sse_false;
  }
  return S_ISREG(mode);
}

sse_bool
SseUtilFile_IsExist(MoatValue *in_path)
{
  sse_int err;
  mode_t mode;

  err = SseUtilFile_GetFileType(in_path, &mode);
  if (err != SSE_E_OK) {
    if (err != SSE_E_NOENT) {
      LOG_ERROR("SseUtilFile_GetFileType() has been failed with [%s].", sse_get_error_string(err));
    }
    return sse_false;
  }
  return sse_true;
}

