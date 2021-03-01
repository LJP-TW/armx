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

// This is the only key function that we have to fake
int get_flash_type()
{
#ifdef VERBOSE
    printf("(get_flash_type) Caller: [0x%08x] \n", __builtin_return_address(0));
    printf("get_flash_type() = 4\n");
#endif
    return 4;
}

// bcm_nvram_get --> nvram_get
char *bcm_nvram_get(char *k)
{
    char *v = "";

#ifdef VERBOSE
    printf("(bcm_nvram_get) Caller: [0x%08x] \n", __builtin_return_address(0));
#endif

    v = nvram_get(k);

    if (v == -1)
        v = "";

#ifdef VERBOSE
    printf("bcm_nvram_get('%s') = '%s'\n", k, v);
#endif

    return v;
}

// bcm_nvram_set --> nvram_set
int bcm_nvram_set(char *k, char *v)
{
    int i;

#ifdef VERBOSE
    printf("(bcm_nvram_set) Caller: [0x%08x] \n", __builtin_return_address(0));  // get caller's address
#endif

    i = nvram_set(k, v);

#ifdef VERBOSE
    printf("[nvram %d] bcm_nvram_set('%s', '%s')\n", i, k, v);
#endif

    return 0;
}

// bcm_nvram_unset --> nvram_unset
int bcm_nvram_unset(char *k)
{
    int i;

#ifdef VERBOSE
    printf("(bcm_nvram_unset) [0x%08x] ", __builtin_return_address(0));  // get caller's address
#endif

    i = nvram_unset(k);

#ifdef VERBOSE
    printf("[nvram %d] bcm_nvram_unset('%s')\n", i, k);
#endif

    return i;
}
