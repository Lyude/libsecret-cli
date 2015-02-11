/* ©2015 Stephen Chandler Paul
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 */

#include "options.h"
#include "list-collections.h"
#include "../config.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>
#include <string.h>
#include <libsecret/secret.h>
#include <signal.h>
#include <errno.h>

static pid_t dbus_pid = 0;

bool libsecret_cli_verbose = false;

static GOptionEntry main_options[] = {
	{ "verbose", 'v', 0, G_OPTION_ARG_NONE, &libsecret_cli_verbose, "Be verbose", NULL },
	{ NULL }
};

static void
print_cmd_help(GOptionContext *option_context) {
	fprintf(stderr, "%s",
		g_option_context_get_help(option_context, true, NULL));
}

static bool
try_starting_dbus(GError **error) {
	char *dbus_stdout, *dbus_stderr;
	int exit_status;

	g_debug("No dbus session was found, attempting to start one");

	if (!g_spawn_command_line_sync(
		DBUS_DAEMON " --session --print-address 1 --print-pid 2 --fork",
		&dbus_stdout, &dbus_stderr, &exit_status, error)) {
		return false;
	}

	if (exit_status != 0) {
		*error = g_error_new(G_SPAWN_ERROR_FAILED, 1,
				     "dbus-daemon exited with %d", exit_status);
		return false;
	}

	dbus_pid = (pid_t) strtol(dbus_stderr, NULL, 10);
	g_debug("dbus session started, PID=%d",
		dbus_pid);

	setenv("DBUS_SESSION_BUS_ADDRESS", dbus_stdout, 1);

	g_free(dbus_stdout);
	g_free(dbus_stderr);

	return true;
}

static void
signal_handler(int signal) {
	g_debug("Disconnecting");
	secret_service_disconnect();

	if (dbus_pid) {
		g_debug("Ending dbus session");

		kill(dbus_pid, SIGTERM);
	}
}

static struct sigaction signal_act = {
	.sa_handler = signal_handler
};

int
main(int argc, char *argv[]) {
	GError *error = NULL;
	GOptionContext *option_context;
	SecretService *service;

	option_context = g_option_context_new(
	    "command [args]\n"
	    "  Access dark secrets from the command line");
	g_option_context_set_help_enabled(option_context, true);
	g_option_context_add_main_entries(option_context, main_options, NULL);
	g_option_context_set_summary(option_context,
		"Commands:\n"
		"  list-collections:\n"
		"    List the available collections"
	);

	if (!g_option_context_parse(option_context, &argc, &argv, &error)) {
		printf("Invalid options: %s\n", error->message);
		exit(1);
	}

	if (argc < 2) {
		print_cmd_help(option_context);
		exit(1);
	}

	g_debug("Getting SecretService proxy");

try_getting_secret_service_proxy:

	service = secret_service_get_sync (SECRET_SERVICE_NONE, NULL, &error);
	if (!service) {
		/* If we can't get the servce and there's no dbus address, it's
		 * got to be that dbus isn't running, try to run dbus if that's
		 * the case
		 */
		if (g_error_matches(error, G_SPAWN_EXIT_ERROR, 1) &&
		    getenv("DISPLAY") == NULL &&
		    getenv("DBUS_SESSION_BUS_ADDRESS") == NULL) {
			if (!try_starting_dbus(&error)) {
				fprintf(stderr, "Couldn't start dbus: %s\n",
					error->message);
				exit(1);
			}

			g_debug("dbus session created, trying to connect to "
				"secret service again");

			g_error_free(error);
			error = NULL;

			goto try_getting_secret_service_proxy;
		}
		else {
			fprintf(stderr,
				"Couldn't connect to secret service: %s\n",
				error->message);
			exit(1);
		}
	}

	g_warn_if_fail(sigaction(SIGSEGV, &signal_act, NULL) == 0);
	g_warn_if_fail(sigaction(SIGTERM, &signal_act, NULL) == 0);
	g_warn_if_fail(sigaction(SIGHUP, &signal_act, NULL) == 0);

	if (strcmp(argv[1], "list-collections") == 0)
		libsecret_cli_command_list_collections(service);
	else {
		print_cmd_help(option_context);
		exit(1);
	}

	g_debug("Disconnecting");
	secret_service_disconnect();

	if (dbus_pid) {
		g_debug("Ending dbus session");

		kill(dbus_pid, SIGTERM);
	}

	return 0;
}
