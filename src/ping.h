#ifndef __PING_H

#include <stdio.h>

#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>

char * host;
pid_t pid;

int sockfd;

#define	BUFSIZE 1500
char sendbuf[BUFSIZE];
int datalen; //ping的ICMP包长度salent

struct sockaddr *sasend; //发送的sockaddr结构体
struct sockaddr *sarecv; //
int salen;

int nsent; //发送报的序列

#define __USAGE_MESSAGE__ "usage: ping [-v] <option> destinationIPaddress"

//创建ping的icmp包
extern void createPingIcmpPackage();
extern void sig_alrm(int signo);
extern u_int16_t in_cksum(u_short * addr, int len);

typedef struct addrinfo * _addrinfo_;
extern _addrinfo_ host_serv(const char *, const char *, int, int);

extern void loopSend();
#endif
