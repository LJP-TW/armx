#ifndef NVRAM_H
#define NVRAM_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>

int nvram_set(char *uParm1, char *pcParm2);
char *nvram_get(char *uParm1);
int nvram_unset(char *uParm1);

#endif