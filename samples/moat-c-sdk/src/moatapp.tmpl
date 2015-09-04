#include <servicesync/moat.h>

sse_int
moat_app_main(sse_int in_argc, sse_char *argv[])
{
	Moat moat = NULL;
	sse_int err = SSE_E_OK;

	err = moat_init(argv[0], &moat);
	if (err != SSE_E_OK) {
		goto error_exit;
	}

	/* setup event handlers, timers, etc */
	/* register models */

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
