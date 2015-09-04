#include <stdio.h>
#include <servicesync/moat.h>
#include <sseutils.h>

#define TAG "SampleApp"

// workaround for "failed to dlsym() [./wiced-demo.so: undefined symbol: moat_app_main]"
extern "C" {
  sse_int moat_app_main(sse_int in_argc, sse_char *argv[]);
}

class MyListener : public TSseUtilCommandListener {
public:
  MyListener() {}
  MyListener(sse_pointer in_user_data) : TSseUtilCommandListener(in_user_data) {
  }

  ~MyListener() {}
  sse_int OnCompleted(sse_int in_result) {
    MOAT_LOG_INFO(TAG, "Completed with [%d].", in_result);
    return SSE_E_OK;
  }

  sse_int OnReadable() {
    sse_char* buff;
    sse_int err;
    TSseUtilCommandShell* cmd = (TSseUtilCommandShell*)fUserData;

    do {
      err = cmd->ReadLine(&buff, sse_true);
      if (err == SSE_E_OK) {
	MOAT_LOG_INFO(TAG, "[%s]", buff);
        sse_free(buff);
      }
    } while (err == SSE_E_OK);

    return SSE_E_OK;
  }

  sse_int OnError(sse_int in_error_code, const sse_char* in_message) {
    MOAT_LOG_ERROR(TAG, "Error with [%d].", in_error_code);
    return SSE_E_OK;
  }
};

sse_int
moat_app_main(sse_int in_argc, sse_char *argv[])
{
  Moat moat = NULL;
  sse_int err = SSE_E_OK;

  TSseUtilCommandShell cmd1;
  MyListener listener1(&cmd1);
  TSseUtilCommandShell cmd2;
  MyListener listener2(&cmd2);
  TSseUtilCommandShell cmd3;
  MyListener listener3(&cmd3);
  TSseUtilCommandShell cmd4;
  MyListener listener4(&cmd4);
  TSseUtilCommandList list_child;
  MyListener listener_list_child(&list_child);
  TSseUtilCommandList list_parent;
  MyListener listener_list_parent(&list_parent);

  err = moat_init(argv[0], &moat);
  if (err != SSE_E_OK) {
    goto error_exit;
  }

  /* setup event handlers, timers, etc */
  /* register models */

  cmd1.AddListener(listener1);
  cmd1.SetCommand("ls");
  cmd1.AddArgument("-l");

  cmd2.AddListener(listener2);
  cmd2.SetCommand("pwd");

  cmd2.AddListener(listener3);
  cmd2.SetCommand("dummy");
  cmd1.AddArgument("-h");

  list_child.AddListener(listener_list_child);
  list_child.AddCommand(cmd1);
  list_child.AddCommand(cmd2);
  list_child.AddCommand(cmd3);

  cmd4.AddListener(listener4);
  cmd4.SetCommand("tail");
  cmd4.AddArgument("-f");
  cmd4.AddArgument("/var/log/syslog");

  list_parent.AddListener(listener_list_parent);
  list_parent.AddCommand(list_child);
  list_parent.AddCommand(cmd4);
  list_parent.Execute();

  err = moat_run(moat);
  if (err != SSE_E_OK) {
    goto error_exit;
  }

  /* unregister models */
  /* teardown */

  moat_destroy(moat);
  return SSE_E_OK;

 error_exit:
  if (moat != NULL) {
    moat_destroy(moat);
  }
  return err;
}
