#include "nvram.h"

#define NVRAM_INI_FILE "/iotemu_nvram.ini"

static inline int sem_get();
static inline int sem_lock(int semid);
static inline int sem_free(int semid);

static inline FILE *open_nvram_ini_file(void);
static inline void close_nvram_ini_file(FILE *fp);

FILE *open_nvram_ini_file(void)
{
    FILE *fp;

    fp = fopen(NVRAM_INI_FILE, "a+");

    if (fp == NULL)
        return NULL;

    fseek(fp, 0, SEEK_SET);

    return fp;
}

void close_nvram_ini_file(FILE *fp)
{
    if (fp == NULL)
        return;
    
    fclose(fp);
}


int nvram_set(char *key, char *value)
{
    int semid;
    int keyidx;
    int complete = 0;
    size_t len = 0;
    char c;
    char *content = NULL;
    char *cur, *tmp;
    FILE *fp;

#ifdef VERBOSE
    printf("(nvram_set) Caller: [0x%08x] \n", __builtin_return_address(0));  // get caller's address
#endif

    if ((semid = sem_get()) == -1)
        return -1;

    if (sem_lock(semid) != 0)
        return -1;

    fp = open_nvram_ini_file();

    if (fp == NULL) {
#ifdef VERBOSE
        printf("nvram_set error!\n");
#endif
        sem_free(semid);
        return -1;
    }

    // Get content
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    content = malloc(len + 1);
    if (content == NULL) {
#ifdef VERBOSE
        printf("nvram_set malloc error!\n");
#endif
        close_nvram_ini_file(fp);
        sem_free(semid);
        return -1;
    }

    fread(content, len, 1, fp);
    content[len] = 0;
    close_nvram_ini_file(fp);

    // Create new file
    fp = fopen(NVRAM_INI_FILE, "w");

    // Scan through the content
    cur = content;

    while (*cur) {
        // is key?
        char isKey = 1;

        tmp = cur;
        keyidx = 0;

        // match key
        while (key[keyidx]) {
            if (*(tmp++) != key[keyidx++]) {
                isKey = 0;
                break;
            }
        }

        // match '='
        if (isKey && *(tmp++) != '=')
            isKey = 0;

        // yeah it is.
        if (isKey) {
            // write key=value to file
            fwrite(key, 1, strlen(key), fp);
            fwrite("=", 1, 1, fp);
            fwrite(value, 1, strlen(value), fp);
            fwrite("\n", 1, 1, fp);

            // let cur skip this line
            while ((c = *cur++) != '\n' && c != '0');

            // write all the rest
            fwrite(cur, 1, strlen(cur), fp);

            // complete
            complete = 1;

            break;
        } else {
            // no, just write cur to file
            while ((c = *cur++) != '\n' && c != 0) {
                fwrite(&c, 1, 1, fp);
            }
            fwrite("\n", 1, 1, fp);
        }
    }

    // key not found
    if (!complete) {
        // create new key-value pair
        fwrite(key, 1, strlen(key), fp);
        fwrite("=", 1, 1, fp);
        fwrite(value, 1, strlen(value), fp);
        fwrite("\n", 1, 1, fp);
    }

    // Close new file
    fclose(fp);

    free(content);

    if (sem_free(semid) != 0)
        return -1;

#ifdef VERBOSE
    printf("nvram_set return\n");
#endif

    return 0;
}

char *nvram_get(char *key)
{
    int semid;
    int keyidx;
    size_t len = 0;
    char c;
    char *content = NULL;
    char *cur, *tmp;
    char *value = NULL;
    FILE *fp;

#ifdef VERBOSE
    printf("(nvram_get) Caller: [0x%08x] \n", __builtin_return_address(0));  // get caller's address
#endif

    if ((semid = sem_get()) == -1)
        return -1;

    if (sem_lock(semid) != 0)
        return -1;

    fp = open_nvram_ini_file();

    if (fp == NULL) {
#ifdef VERBOSE
        printf("nvram_set error!\n");
#endif
        sem_free(semid);
        return -1;
    }

    // Get content
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    content = malloc(len + 1);
    if (content == NULL) {
#ifdef VERBOSE
        printf("nvram_set malloc error!\n");
#endif
        close_nvram_ini_file(fp);
        sem_free(semid);
        return -1;
    }

    fread(content, len, 1, fp);
    content[len] = 0;
    close_nvram_ini_file(fp);

    // Scan through the content
    cur = content;

    while (*cur) {
        // is key?
        char isKey = 1;

        tmp = cur;
        keyidx = 0;

        // match key
        while (key[keyidx]) {
            if (*(tmp++) != key[keyidx++]) {
                isKey = 0;
                break;
            }
        }

        // match '='
        if (isKey && *(tmp++) != '=')
            isKey = 0;

        // yeah it is.
        if (isKey) {
            // compute len of value
            char *tmpv = tmp;
            len = 0;
            while ((c = *(tmp++)) != '\n' && c != 0)
                ++len;
            
            // create memory for value
            value = malloc(len + 1);
            
            // copy value
            strncpy(value, tmpv, len);
            value[len] = 0;
            
            // complete
            break;
        } else {
            // no, just skip this line
            while ((c = *cur++) != '\n' && c != 0);
        }
    }

    free(content);

    if (sem_free(semid) != 0)
        return -1;

#ifdef VERBOSE
    printf("nvram_get return\n");
#endif

    // is key found?
    return value ? value : -1;
}

int nvram_unset(char *uParm1)
{
    // TODO: nvram unset
    return 0;
}


inline int sem_get()
{
#ifdef SEM_ENABLE
    int key, semid;
    struct sembuf initbuf = {0, 1, 0};

    key = ftok(NVRAM_INI_FILE, 'G');
    semid = semget(key, 0, 0);
    if (semid == -1) { 
        semid = semget(key, 1, IPC_CREAT | IPC_EXCL | S_IRWXU);

        if (semid == -1) {
#ifdef VERBOSE
            printf("[sem_get] semaphore creation failed\n");
#endif
            return -1;
        }

        if (semop(semid, &initbuf, 1) == -1) {
#ifdef VERBOSE
            printf("[sem_get] semop failed\n");
#endif
            return -1;
        }
    }
    return semid;
#else
    return 0;
#endif
}

inline int sem_lock(int semid)
{
#ifdef SEM_ENABLE
    struct sembuf lockbuf = {0, -1, 0};

    if (semop(semid, &lockbuf, 1) == -1) {
#ifdef VERBOSE
        printf("[sem_lock] failed\n");
#endif
        return -1;
    }

    return 0;
#else
    return 0;
#endif
}

inline int sem_free(int semid)
{
#ifdef SEM_ENABLE
    struct sembuf freebuf = {0, 1, 0};

    if (semop(semid, &freebuf, 1) == -1) {
#ifdef VERBOSE        
        printf("[sem_free] failed\n");
#endif
        return -1;
    }

    return 0;
#else
    return 0;
#endif
}