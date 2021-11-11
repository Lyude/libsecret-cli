What is this?
=============

libsecret-cli is meant to be a command line tool to access passwords, secrets, etc. using the [libsecret](https://wiki.gnome.org/Projects/Libsecret) library. libsecret. Originally a gnome specific service, libgnome-keyring, libsecret is currently used by many various desktop environments on Linux to store all of a user's saved passwords and other secure information in a single, secure and easy to access keyring. Unfortunately however, there has been no good implementations of this on the command line. To many developers, such a feature sounds (understandably) useless, but actually has the potential to be quite useful. For example, libsecret-cli would allow you to get your very long and unmemorizable Twitter password over ssh, without having to do X forwarding (which over a slow connection, is not ideal).

How do I use it?
================

Eventually if this tool becomes complex enough, I may write some proper documentation. For the time being, you can view all the current commands libsecret-cli supports by running
``
libsecret-cli --help
``.

It should be noted that libsecret-cli is in the early stages of development, and as a result can't really do anything very useful yet.

How do I install it?
====================

As of writing this, all you really need is `libsecret`. However, for packagers here's a full list of dependencies you need:
* `libsecret`
* `glib-2.0`
* Access to the `dbus-session` command

This project looks dead!
========================

~~If at any point this project looks dead, I promise you: it's probably not. Right now libsecret-cli is strictly a side-project, since my main focuses are on Wayland right now. libsecret-cli is a program I need for my own personal purposes, so it's very unlikely I'll ever drop this project completely, it will just take a long time to finish. :)~~

actually-it's kinda dead at this point, sorry!
