#ifndef PING_H
#define PING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#define BUFSIZE       1500	/* 收发缓冲区大小 */
#define ICMP_HLEN     8		/* ICMP 头部长度（不含数据） */
#define ICMP_DATALEN  56	/* 默认 ICMP 数据长度，与常见 ping 一致 */

/* ---------- 进程内共享状态（定义在 main.c） ---------- */
extern pid_t pid;			/* ICMP 标识，取 PID 低 16 位 */
extern int sockfd;			/* RAW ICMP socket */
extern char sendbuf[BUFSIZE];		/* 发送缓冲区 */
extern int datalen;			/* ICMP 数据区长度 */
extern struct sockaddr *sasend;		/* 目的地址 */
extern struct sockaddr *sarecv;		/* 对端回包地址 */
extern int salen;			/* sockaddr 长度 */
extern int nsent;			/* 已发送序号（icmp_seq） */

/* 按主机名/IP 解析 IPv4 地址，失败返回 NULL */
struct addrinfo *host_serv(const char *host, const char *serv,
		int family, int socktype);

/* 组装 ICMP Echo Request 到 sendbuf */
void createPingIcmpPackage(void);

/* 计算 ICMP 校验和 */
u_int16_t in_cksum(u_short *addr, int len);

/* 发送一帧 Echo Request */
void mysend(void);

/* SIGALRM：定时发下一帧 */
void sig_alrm(int signo);

/* 打开 RAW socket，循环收包 */
void loopSend(void);

/* 解析 IPv4 ICMP Echo Reply 并打印 RTT */
void proc_v4(char *ptr, ssize_t len, struct msghdr *msg,
		struct timeval *tvrecv);

#endif /* PING_H */
