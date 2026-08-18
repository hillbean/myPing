#include "ping.h"

void mysend(void)
{
	ssize_t n;

	createPingIcmpPackage();
	n = sendto(sockfd, sendbuf, ICMP_HLEN + datalen, 0, sasend, salen);
	if (n < 0)
		perror("sendto");
}

/* 每秒由 SIGALRM 触发一次发送，与收包循环解耦 */
void sig_alrm(int signo)
{
	(void)signo;
	mysend();
	alarm(1);
}
