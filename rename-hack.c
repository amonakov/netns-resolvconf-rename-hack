#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64

#include <dlfcn.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

static int
copyfile(const char *old, const char *new)
{
  int oldfd, newfd, ret = -1;

  if ((oldfd = open(old, O_RDONLY)) == -1)
    goto out0;
  if ((newfd = open(new, O_WRONLY | O_TRUNC | O_CREAT, 0600)) == -1)
    goto out1;

  ret = sendfile(newfd, oldfd, NULL, 1<<16);

  ret |= close(newfd);
out1:
  ret |= close(oldfd);
out0:
  return ret;
}

static const char resolvconf[] = "/etc/resolv.conf";

int
rename(const char *old, const char *new)
{
  if (!strcmp(new, resolvconf)
      || !strcmp(old, resolvconf))
    {
      if (copyfile(old, new) == -1)
	return -1;
      else if (!strcmp(old, resolvconf))
	return truncate(old, 0);
      else
	return unlink(old);
    }
  int (*real_rename)(const char *, const char *);
  real_rename = dlsym(RTLD_NEXT, "rename");
  return real_rename(old, new);
}
