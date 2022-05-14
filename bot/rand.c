#define _GNU_SOURCE

#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "includes.h"
#include "rand.h"
#include "table.h"
#include "util.h"

static uint32_t x, y, z, w;

void rand_init(void)
{
    x = time(NULL);
    y = getpid() ^ getppid();
    z = clock();
    w = z ^ y;
}

uint32_t rand_next(void) //period 2^96-1
{
    uint32_t t = x;
    t ^= t << 11;
    t ^= t >> 8;
    x = y; y = z; z = w;
    w ^= w >> 19;
    w ^= t;
    return w;
}

void rand_str(char *str, int len) // Generate random buffer (not alphanumeric!) of length len
{
    while (len > 0)
    {
        if (len >= 4)
        {
            *((uint32_t *)str) = rand_next();
            str += sizeof (uint32_t);
            len -= sizeof (uint32_t);
        }
        else if (len >= 2)
        {
            *((uint16_t *)str) = rand_next() & 0xFFFF;
            str += sizeof (uint16_t);
            len -= sizeof (uint16_t);
        }
        else
        {
            *str++ = rand_next() & 0xFF;
            len--;
        }
    }
}

void rand_alpha_str(uint8_t *str, int len) // Random alphanumeric string, more expensive than rand_str
{
	table_unlock_val(TABLE_MISC_RAND);
    char alpha_set[32];
    strcpy(alpha_set,table_retrieve_val(TABLE_MISC_RAND, NULL));
    while(len--)
        *str++ = alpha_set[rand_next() % util_strlen(alpha_set)];
	table_lock_val(TABLE_MISC_RAND);
}
