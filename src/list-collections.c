/* ©2015 Stephen Chandler Paul <thatslyude@gmail.com>
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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>
#include <string.h>
#include <libsecret/secret.h>

#include "list-collections.h"
#include "options.h"

void
libsecret_cli_command_list_collections(SecretService *service) {
	GError *error = NULL;
	GList *collections;

	if (libsecret_cli_verbose)
		printf("Getting list of collections...\n");

	collections = libsecret_cli_list_collections(service, &error);

	if (!collections) {
		fprintf(stderr, "Couldn't get list of collections: %s\n",
			error->message);
		exit(1);
	}

	for (GList *l = collections; l != NULL; l = l->next) {
		char *label = secret_collection_get_label(l->data);

		if (strcmp(label, "") == 0)
			continue;

		printf("%s\n",
		       label);
	}

	g_list_free(collections);
}

GList *
libsecret_cli_list_collections(SecretService *service,
			       GError **error) {
	GList *collections;

	g_debug("Loading collections");

	if (!secret_service_load_collections_sync(service, NULL, error))
		return NULL;

	collections = secret_service_get_collections(service);

	return collections;
}
