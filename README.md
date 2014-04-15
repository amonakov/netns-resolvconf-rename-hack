netns-resolvconf-rename-hack
============================

This LD_PRELOAD interposer is intended to be used for (proprietary) VPN
clients such as Juniper Network Connect running inside a network namespace
with per-namespace '/etc/resolv.conf' bind-mounted from
'/etc/netns/*NAME*/resolv.conf'.  Calling 'rename()' on the bind mount fails
with EBUSY, which may cause the VPN client to fail prematurely.  To workaround
that, this library emulates 'rename()' by copying the contents and truncating
or removing the original file.

When running a 32-bit VPN client on a 64-bit Linux distribution you need to
build a 32-bit version of the library:

    CC=gcc\ -m32 make
