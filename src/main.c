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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>
#include <string.h>
#include <libsecret/secret.h>

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
	service = secret_service_get_sync (SECRET_SERVICE_NONE, NULL, &error);
	if (!service) {
		fprintf(stderr, "Couldn't connect to secret service: %s\n",
			error->message);
		exit(1);
	}

	if (strcmp(argv[1], "list-collections") == 0)
		libsecret_cli_list_collections(service);
	else {
		print_cmd_help(option_context);
		exit(1);
	}

	g_debug("Disconnecting");
	secret_service_disconnect();

	return 0;
}
