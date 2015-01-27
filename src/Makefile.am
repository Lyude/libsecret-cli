AM_CFLAGS = -Wall

bin_PROGRAMS = libsecret-cli
libsecret_cli_SOURCES = main.c \
			list-collections.c
libsecret_cli_CFLAGS = $(GLIB_CFLAGS) $(LIBSECRET_CFLAGS)
libsecret_cli_LDFLAGS = $(GLIB_LIBS) $(LIBSECRET_LIBS)
