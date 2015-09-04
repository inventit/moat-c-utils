{
  'variables': {
    'sseutils_src': [
      '<(sseutils_root)/src/stream_buffer/sse_util_stream_buffer.c',
      '<(sseutils_root)/src/command/sse_util_shell_command.c',
      '<(sseutils_root)/src/command/sse_util_command_listener.cc',
      '<(sseutils_root)/src/command/sse_util_command.cc',
      '<(sseutils_root)/src/command/sse_util_command_shell.cc',
      '<(sseutils_root)/src/command/sse_util_command_list.cc',
      '<(sseutils_root)/src/network/sse_util_netinfo.c',
      '<(sseutils_root)/src/file/sse_util_file.c',
      '<(sseutils_root)/src/debug/sse_util_hexdump.c',
      '<(sseutils_root)/src/debug/sse_util_moatobject_dump.c',
    ],
    'sseutils_include' : [
      '<(sseutils_root)/include'
    ],
  },
}

