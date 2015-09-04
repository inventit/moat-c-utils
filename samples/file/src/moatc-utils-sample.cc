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
#include <assert.h>
#include <stdlib.h>
#include <setjmp.h>
#include <servicesync/moat.h>
#include <sseutils.h>

jmp_buf g_env;
sse_int g_success = 0;
sse_int g_failure = 0;
sse_int g_unknown = 0;

const sse_int TEST_RESULT_SUCCESS = 0;
const sse_int TEST_RESULT_FAILURE = 1;
const sse_int TEST_RESULT_UNKNOWN = 2;

#define LOG_PRINT(format, ...) printf("## TEST ## %s L%d: " format "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define DO_TEST(func) {				\
  sse_int ret;					\
  sse_int result;				\
  LOG_PRINT("Test case: " #func);		\
  if ((ret = setjmp(g_env)) == 0) {		\
    result = func;				\
    if (result == TEST_RESULT_SUCCESS) {	\
      LOG_PRINT(#func "... Success\n");		\
      g_success++;				\
    } else {					\
      LOG_PRINT(#func "... Unknown\n");		\
      g_unknown++;				\
    }						\
  } else if (ret == 1) {			\
    LOG_PRINT(#func "... Failure\n");		\
    g_failure++;				\
  }						\
}

#define ASSERT(cond) {				\
  if (!(cond)) {				\
    LOG_PRINT("Assersion failure: " #cond);	\
    longjmp(g_env, 1);				\
  }						\
}

#define TAG "SampleApp"

// workaround for "failed to dlsym() [./wiced-demo.so: undefined symbol: moat_app_main]"
extern "C" {
  sse_int moat_app_main(sse_int in_argc, sse_char *argv[]);
}

static sse_int
test_file_GetDirecoryPath_1(void)
{
  MoatValue *path;
  MoatValue *dir;
  const sse_char *file = "/foo/bar/baz.txt";
  const sse_char *expected = "/foo/bar";
  sse_char *actual;
  sse_uint actual_len;

  path = moat_value_new_string((sse_char*)file, 0, sse_true);
  ASSERT((SseUtilFile_GetDirectoryPath(path, &dir) == SSE_E_OK));
  ASSERT((dir != NULL));
  MOAT_VALUE_DUMP_INFO("Test", dir);
  ASSERT((moat_value_get_string(dir, &actual, &actual_len) == SSE_E_OK));
  ASSERT((sse_strncmp(expected, actual, actual_len) == 0));

  moat_value_free(path);
  moat_value_free(dir);
  return TEST_RESULT_SUCCESS;
}

static sse_int
test_file_GetDirecoryPath_2(void)
{
  MoatValue *path;
  MoatValue *dir;
  const sse_char *file = "/hogehoge.txt";
  const sse_char *expected = "/";
  sse_char *actual;
  sse_uint actual_len;

  path = moat_value_new_string((sse_char*)file, 0, sse_true);
  ASSERT((SseUtilFile_GetDirectoryPath(path, &dir) == SSE_E_OK));
  ASSERT((dir != NULL));
  MOAT_VALUE_DUMP_INFO("Test", dir);
  ASSERT((moat_value_get_string(dir, &actual, &actual_len) == SSE_E_OK));
  ASSERT((sse_strncmp(expected, actual, actual_len) == 0));

  moat_value_free(path);
  moat_value_free(dir);
  return TEST_RESULT_SUCCESS;
}

static sse_int
test_file_GetDirecoryPath_3(void)
{
  MoatValue *path;
  MoatValue *dir;
  const sse_char *file = "../foo/bar/baz.txt";
  const sse_char *expected = "../foo/bar";
  sse_char *actual;
  sse_uint actual_len;

  path = moat_value_new_string((sse_char*)file, 0, sse_true);
  ASSERT((SseUtilFile_GetDirectoryPath(path, &dir) == SSE_E_OK));
  ASSERT((dir != NULL));
  MOAT_VALUE_DUMP_INFO("Test", dir);
  ASSERT((moat_value_get_string(dir, &actual, &actual_len) == SSE_E_OK));
  ASSERT((sse_strncmp(expected, actual, actual_len) == 0));

  moat_value_free(path);
  moat_value_free(dir);
  return TEST_RESULT_SUCCESS;
}

static sse_int
test_file_GetDirecoryPath_4(void)
{
  MoatValue *path;
  MoatValue *dir;
  const sse_char *file = "hoge.txt";

  path = moat_value_new_string((sse_char*)file, 0, sse_true);
  ASSERT((SseUtilFile_GetDirectoryPath(path, &dir) == SSE_E_NOENT));

  moat_value_free(path);
  return TEST_RESULT_SUCCESS;
}

static sse_int
test_file_IsFile_file(void)
{
  sse_char *pwd;
  sse_char path[1024];
  sse_char cmd[1024];
  MoatValue *value;

  pwd = getenv("PWD");
  sprintf(path, "%s/%s", pwd, "utest_dummy_file");
  LOG_PRINT("path=[%s]", path);

  sprintf(cmd, "rm -f %s", path); system(cmd);
  sprintf(cmd, "touch %s", path); system(cmd);
  ASSERT((value = moat_value_new_string(path, 0, sse_true)));
  ASSERT(SseUtilFile_IsFile(value));
  moat_value_free(value);
  sprintf(cmd, "rm -f %s", path); system(cmd);
  return TEST_RESULT_SUCCESS;
}

static sse_int
test_file_IsFile_dir(void)
{
  sse_char *pwd;
  sse_char path[1024];
  sse_char cmd[1024];
  MoatValue *value;

  pwd = getenv("PWD");
  sprintf(path, "%s/%s", pwd, "utest_dummy_dir");
  LOG_PRINT("path=[%s]", path);

  sprintf(cmd, "rm -rf %s", path); system(cmd);
  sprintf(cmd, "mkdir %s", path); system(cmd);
  ASSERT((value = moat_value_new_string(path, 0, sse_true)));
  ASSERT(!SseUtilFile_IsFile(value));
  moat_value_free(value);
  sprintf(cmd, "rm -rf %s", path); system(cmd);
  return TEST_RESULT_SUCCESS;
}

static sse_int
test_file_IsFile_slink_file(void)
{
  sse_char *pwd;
  sse_char path[1024];
  sse_char link[1024];
  sse_char cmd[1024];
  MoatValue *value;

  pwd = getenv("PWD");
  sprintf(path, "%s/%s", pwd, "utest_dummy_file");
  LOG_PRINT("path=[%s]", path);
  sse_strcpy(link, path);
  sse_strcat(link, ".link");
  LOG_PRINT("link=[%s]", link);

  sprintf(cmd, "rm -f %s", path); system(cmd);
  sprintf(cmd, "rm -f %s", link); system(cmd);
  sprintf(cmd, "touch %s", path); system(cmd);
  sprintf(cmd, "ln -s %s %s", path, link); system(cmd);

  ASSERT((value = moat_value_new_string(link, 0, sse_true)));
  ASSERT(SseUtilFile_IsFile(value));
  moat_value_free(value);

  sprintf(cmd, "rm -f %s", path); system(cmd);
  sprintf(cmd, "rm -f %s", link); system(cmd);

  return TEST_RESULT_SUCCESS;
}

static sse_int
test_file_IsFile_slink_dir(void)
{
  sse_char *pwd;
  sse_char path[1024];
  sse_char link[1024];
  sse_char cmd[1024];
  MoatValue *value;

  pwd = getenv("PWD");
  sprintf(path, "%s/%s", pwd, "utest_dummy_dir");
  LOG_PRINT("path=[%s]", path);
  sse_strcpy(link, path);
  sse_strcat(link, ".link");
  LOG_PRINT("link=[%s]", link);

  sprintf(cmd, "rm -rf %s", path); system(cmd);
  sprintf(cmd, "rm -rf %s", link); system(cmd);
  sprintf(cmd, "mkdir %s", path); system(cmd);
  sprintf(cmd, "ln -s %s %s", path, link); system(cmd);

  ASSERT((value = moat_value_new_string(link, 0, sse_true)));
  ASSERT(!SseUtilFile_IsFile(value));
  moat_value_free(value);

  sprintf(cmd, "rm -rf %s", path); system(cmd);
  sprintf(cmd, "rm -rf %s", link); system(cmd);

  return TEST_RESULT_SUCCESS;
}

static sse_int
test_file_IsFile_noent(void)
{
  sse_char *pwd;
  sse_char path[1024];
  sse_char cmd[1024];
  MoatValue *value;

  pwd = getenv("PWD");
  sprintf(path, "%s/%s", pwd, "utest_dummy_file");
  LOG_PRINT("path=[%s]", path);
  sprintf(cmd, "rm -rf %s", path); system(cmd);
  ASSERT((value = moat_value_new_string(path, 0, sse_true)));
  ASSERT(!SseUtilFile_IsFile(value));
  moat_value_free(value);

  return TEST_RESULT_SUCCESS;
}

static sse_int
test_file_IsDirectory_file(void)
{
  sse_char *pwd;
  sse_char path[1024];
  sse_char cmd[1024];
  MoatValue *value;

  pwd = getenv("PWD");
  sprintf(path, "%s/%s", pwd, "utest_dummy_file");
  LOG_PRINT("path=[%s]", path);

  sprintf(cmd, "rm -f %s", path); system(cmd);
  sprintf(cmd, "touch %s", path); system(cmd);
  ASSERT((value = moat_value_new_string(path, 0, sse_true)));
  ASSERT(!SseUtilFile_IsDirectory(value));
  moat_value_free(value);
  sprintf(cmd, "rm -f %s", path); system(cmd);
  return TEST_RESULT_SUCCESS;
}

static sse_int
test_file_IsDirectory_dir(void)
{
  sse_char *pwd;
  sse_char path[1024];
  sse_char cmd[1024];
  MoatValue *value;

  pwd = getenv("PWD");
  sprintf(path, "%s/%s", pwd, "utest_dummy_dir");
  LOG_PRINT("path=[%s]", path);

  sprintf(cmd, "rm -rf %s", path); system(cmd);
  sprintf(cmd, "mkdir %s", path); system(cmd);
  ASSERT((value = moat_value_new_string(path, 0, sse_true)));
  ASSERT(SseUtilFile_IsDirectory(value));
  moat_value_free(value);
  sprintf(cmd, "rm -rf %s", path); system(cmd);
  return TEST_RESULT_SUCCESS;
}

static sse_int
test_file_IsDirectory_slink_file(void)
{
  sse_char *pwd;
  sse_char path[1024];
  sse_char link[1024];
  sse_char cmd[1024];
  MoatValue *value;

  pwd = getenv("PWD");
  sprintf(path, "%s/%s", pwd, "utest_dummy_file");
  LOG_PRINT("path=[%s]", path);
  sse_strcpy(link, path);
  sse_strcat(link, ".link");
  LOG_PRINT("link=[%s]", link);

  sprintf(cmd, "rm -f %s", path); system(cmd);
  sprintf(cmd, "rm -f %s", link); system(cmd);
  sprintf(cmd, "touch %s", path); system(cmd);
  sprintf(cmd, "ln -s %s %s", path, link); system(cmd);

  ASSERT((value = moat_value_new_string(link, 0, sse_true)));
  ASSERT(!SseUtilFile_IsDirectory(value));
  moat_value_free(value);

  sprintf(cmd, "rm -f %s", path); system(cmd);
  sprintf(cmd, "rm -f %s", link); system(cmd);

  return TEST_RESULT_SUCCESS;
}

static sse_int
test_file_IsDirectory_slink_dir(void)
{
  sse_char *pwd;
  sse_char path[1024];
  sse_char link[1024];
  sse_char cmd[1024];
  MoatValue *value;

  pwd = getenv("PWD");
  sprintf(path, "%s/%s", pwd, "utest_dummy_dir");
  LOG_PRINT("path=[%s]", path);
  sse_strcpy(link, path);
  sse_strcat(link, ".link");
  LOG_PRINT("link=[%s]", link);

  sprintf(cmd, "rm -rf %s", path); system(cmd);
  sprintf(cmd, "rm -rf %s", link); system(cmd);
  sprintf(cmd, "mkdir %s", path); system(cmd);
  sprintf(cmd, "ln -s %s %s", path, link); system(cmd);

  ASSERT((value = moat_value_new_string(link, 0, sse_true)));
  ASSERT(SseUtilFile_IsDirectory(value));
  moat_value_free(value);

  sprintf(cmd, "rm -rf %s", path); system(cmd);
  sprintf(cmd, "rm -rf %s", link); system(cmd);

  return TEST_RESULT_SUCCESS;
}

static sse_int
test_file_MakeDirectory_normal(void)
{
  sse_char *pwd;
  sse_char base[1024];
  sse_char path[1024];
  sse_char cmd[1024];
  MoatValue *value;

  pwd = getenv("PWD");
  sprintf(base, "%s/%s", pwd, "utest_work_dir");
  sprintf(path, "%s/%s", pwd, "utest_work_dir/foo/bar/baz");
  LOG_PRINT("path=[%s]", path);
  
  sprintf(cmd, "rm -rf %s", base); system(cmd);

  ASSERT((value = moat_value_new_string(path, 0, sse_true)));
  ASSERT((SseUtilFile_MakeDirectory(value) == SSE_E_OK));
  ASSERT((SseUtilFile_IsDirectory(value) == sse_true));
  moat_value_free(value);

  system("tree -p utest_work_dir");
  sprintf(cmd, "rm -rf %s", base); system(cmd);
  return TEST_RESULT_SUCCESS;
}

static sse_int
test_file_MoveFile_succcess(void)
{
  sse_char *pwd;
  sse_char work_dir[1024];
  sse_char work_dir_src[1024];
  sse_char work_dir_dst[1024];
  sse_char src_file[1024];
  sse_char dst_file[1024];
  sse_char cmd[1024];
  MoatValue *value_src_file;
  MoatValue *value_dst_file;

  pwd = getenv("PWD");
  sprintf(work_dir, "%s/%s", pwd, "utest_work_dir");
  sprintf(work_dir_src, "%s/%s", work_dir, "src");
  sprintf(work_dir_dst, "%s/%s", work_dir, "dst");
  sprintf(src_file, "%s/%s", work_dir_src, "file.txt.part");
  sprintf(dst_file, "%s/%s", work_dir_dst, "file.txt");

  sprintf(cmd, "rm -rf %s", work_dir); system(cmd);
  sprintf(cmd, "mkdir -p %s", work_dir_src); system(cmd);
  sprintf(cmd, "mkdir -p %s", work_dir_dst); system(cmd);
  sprintf(cmd, "touch %s", src_file); system(cmd);

  value_src_file = moat_value_new_string(src_file, 0, sse_true);
  value_dst_file = moat_value_new_string(dst_file, 0, sse_true);

  sprintf(cmd, "tree -p %s", work_dir); system(cmd);
  ASSERT((SseUtilFile_MoveFile(value_src_file, value_dst_file) == SSE_E_OK));
  ASSERT((!SseUtilFile_IsExist(value_src_file)));
  ASSERT((SseUtilFile_IsFile(value_dst_file)));

  moat_value_free(value_src_file);
  moat_value_free(value_dst_file);

  sprintf(cmd, "tree -p %s", work_dir); system(cmd);
  sprintf(cmd, "rm -rf %s", work_dir); system(cmd);
  return TEST_RESULT_SUCCESS;
}

static sse_int
test_file_MoveFile_failure_exist_dir(void)
{
  sse_char *pwd;
  sse_char work_dir[1024];
  sse_char work_dir_src[1024];
  sse_char work_dir_dst[1024];
  sse_char src_file[1024];
  sse_char dst_file[1024];
  sse_char cmd[1024];
  MoatValue *value_src_file;
  MoatValue *value_dst_file;

  pwd = getenv("PWD");
  sprintf(work_dir, "%s/%s", pwd, "utest_work_dir");
  sprintf(work_dir_src, "%s/%s", work_dir, "src");
  sprintf(work_dir_dst, "%s/%s", work_dir, "dst");
  sprintf(src_file, "%s/%s", work_dir_src, "file.txt.part");
  sprintf(dst_file, "%s/%s", work_dir_dst, "file.txt");

  sprintf(cmd, "rm -rf %s", work_dir); system(cmd);
  sprintf(cmd, "mkdir -p %s", work_dir_src); system(cmd);
  sprintf(cmd, "touch %s", work_dir_dst); system(cmd);
  sprintf(cmd, "touch %s", src_file); system(cmd);

  value_src_file = moat_value_new_string(src_file, 0, sse_true);
  value_dst_file = moat_value_new_string(dst_file, 0, sse_true);

  sprintf(cmd, "tree -p %s", work_dir); system(cmd);
  ASSERT((SseUtilFile_MoveFile(value_src_file, value_dst_file) != SSE_E_OK));
  ASSERT((SseUtilFile_IsFile(value_src_file))); // The source file should not be removed.

  moat_value_free(value_src_file);
  moat_value_free(value_dst_file);

  sprintf(cmd, "tree -p %s", work_dir); system(cmd);
  sprintf(cmd, "rm -rf %s", work_dir); system(cmd);
  return TEST_RESULT_SUCCESS;
}

static sse_int
test_file_DeleteFile_success(void)
{
  sse_char *pwd;
  sse_char work_dir[1024];
  sse_char file[1024];
  sse_char cmd[1024];
  MoatValue *value_file;

  pwd = getenv("PWD");
  sprintf(work_dir, "%s/%s", pwd, "utest_work_dir");
  sprintf(file, "%s/%s", work_dir, "test.txt");

  sprintf(cmd, "rm -rf %s", work_dir); system(cmd);
  sprintf(cmd, "mkdir -p %s", work_dir); system(cmd);
  sprintf(cmd, "touch %s", file); system(cmd);

  value_file = moat_value_new_string(file, 0, sse_true);

  sprintf(cmd, "tree -p %s", work_dir); system(cmd);
  ASSERT((SseUtilFile_DeleteFile(value_file) == SSE_E_OK));
  ASSERT((!SseUtilFile_IsExist(value_file)));
  ASSERT((!SseUtilFile_IsFile(value_file)));

  moat_value_free(value_file);

  sprintf(cmd, "tree -p %s", work_dir); system(cmd);
  sprintf(cmd, "rm -rf %s", work_dir); system(cmd);
  return TEST_RESULT_SUCCESS;
}

static sse_int
test_file_DeleteFile_failure_dir(void)
{
  sse_char *pwd;
  sse_char work_dir[1024];
  sse_char file[1024];
  sse_char cmd[1024];
  MoatValue *value_file;

  pwd = getenv("PWD");
  sprintf(work_dir, "%s/%s", pwd, "utest_work_dir");
  sprintf(file, "%s/%s", work_dir, "test.txt");

  sprintf(cmd, "rm -rf %s", work_dir); system(cmd);
  sprintf(cmd, "mkdir -p %s", work_dir); system(cmd);
  sprintf(cmd, "mkdir -p %s", file); system(cmd);

  value_file = moat_value_new_string(file, 0, sse_true);

  sprintf(cmd, "tree -p %s", work_dir); system(cmd);
  ASSERT((SseUtilFile_DeleteFile(value_file) != SSE_E_OK));
  ASSERT((SseUtilFile_IsDirectory(value_file)));

  moat_value_free(value_file);

  sprintf(cmd, "tree -p %s", work_dir); system(cmd);
  sprintf(cmd, "rm -rf %s", work_dir); system(cmd);
  return TEST_RESULT_SUCCESS;
}

static int
test_file_GetFileName_1(void)
{
  const sse_char *path_cstr = "/foo/bar/baz/file.txt";
  const sse_char *expected = "file.txt";
  MoatValue *path;
  MoatValue *file;
  sse_char *str;
  sse_uint len;

  ASSERT((path = moat_value_new_string((sse_char*)path_cstr, 0, sse_true)));
  ASSERT((SseUtilFile_GetFileName(path, &file) == SSE_E_OK));
  LOG_PRINT("File path ...");
  MOAT_VALUE_DUMP_INFO("Test", path);
  LOG_PRINT("File name ...");
  MOAT_VALUE_DUMP_INFO("Test", file);
  ASSERT((moat_value_get_string(file, &str, &len) == SSE_E_OK));
  ASSERT(((sse_uint)sse_strlen(expected) == len));
  ASSERT((sse_strncmp(expected, str, len) == 0));

  moat_value_free(path);
  moat_value_free(file);
  return TEST_RESULT_SUCCESS;
}

static int
test_file_GetFileName_2(void)
{
  const sse_char *path_cstr = "file.txt";
  const sse_char *expected = "file.txt";
  MoatValue *path;
  MoatValue *file;
  sse_char *str;
  sse_uint len;

  ASSERT((path = moat_value_new_string((sse_char*)path_cstr, 0, sse_true)));
  ASSERT((SseUtilFile_GetFileName(path, &file) == SSE_E_OK));
  LOG_PRINT("File path ...");
  MOAT_VALUE_DUMP_INFO("Test", path);
  LOG_PRINT("File name ...");
  MOAT_VALUE_DUMP_INFO("Test", file);
  ASSERT((moat_value_get_string(file, &str, &len) == SSE_E_OK));
  ASSERT(((sse_uint)sse_strlen(expected) == len));
  ASSERT((sse_strncmp(expected, str, len) == 0));

  moat_value_free(path);
  moat_value_free(file);
  return TEST_RESULT_SUCCESS;
}

static int
test_file_GetFileName_3(void)
{
  const sse_char *path_cstr = "/foo/bar/baz/";
  MoatValue *path;
  MoatValue *file;

  ASSERT((path = moat_value_new_string((sse_char*)path_cstr, 0, sse_true)));
  ASSERT((SseUtilFile_GetFileName(path, &file) == SSE_E_NOENT));

  moat_value_free(path);
  return TEST_RESULT_SUCCESS;
}


static void
test_report(void)
{
  LOG_PRINT("Result : success = %d / unknown = %d / failure = %d", g_success, g_unknown,  g_failure);
}

sse_int
moat_app_main(sse_int in_argc, sse_char *argv[])
{
  Moat moat = NULL;
  sse_int err = SSE_E_OK;


  err = moat_init(argv[0], &moat);
  if (err != SSE_E_OK) {
    goto error_exit;
  }

  DO_TEST(test_file_GetDirecoryPath_1());
  DO_TEST(test_file_GetDirecoryPath_2());
  DO_TEST(test_file_GetDirecoryPath_3());
  DO_TEST(test_file_GetDirecoryPath_4());

  DO_TEST(test_file_IsFile_file());
  DO_TEST(test_file_IsFile_dir());
  DO_TEST(test_file_IsFile_slink_file());
  DO_TEST(test_file_IsFile_slink_dir());
  DO_TEST(test_file_IsFile_noent());

  DO_TEST(test_file_IsDirectory_file());
  DO_TEST(test_file_IsDirectory_dir());
  DO_TEST(test_file_IsDirectory_slink_file());
  DO_TEST(test_file_IsDirectory_slink_dir());

  DO_TEST(test_file_MakeDirectory_normal());

  DO_TEST(test_file_MoveFile_succcess());
  DO_TEST(test_file_MoveFile_failure_exist_dir());

  DO_TEST(test_file_DeleteFile_success());
  DO_TEST(test_file_DeleteFile_failure_dir());

  DO_TEST(test_file_GetFileName_1());
  DO_TEST(test_file_GetFileName_2());
  DO_TEST(test_file_GetFileName_3());

  err = moat_run(moat);
  if (err != SSE_E_OK) {
    goto error_exit;
  }

  test_report();

  moat_destroy(moat);
  return SSE_E_OK;

 error_exit:
  if (moat != NULL) {
    moat_destroy(moat);
  }
  return err;
}
