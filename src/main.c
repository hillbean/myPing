#include "ping.h"

/* 全局状态：各 .c 通过 ping.h 的 extern 共享 */
pid_t pid;
int sockfd;
char sendbuf[BUFSIZE];
int datalen;
struct sockaddr *sasend;
struct sockaddr *sarecv;
int salen;
int nsent;

int main(int argc, char **argv)
{
	struct addrinfo *ai;
	char ipstr[INET_ADDRSTRLEN];
	char *host;

	/* 管道/后台运行时也按行刷新，避免 RTT 输出被完全缓冲 */
	setvbuf(stdout, NULL, _IOLBF, 0);

	if (argc < 2) {
		fprintf(stderr, "usage: %s <destination>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* ICMP 的 id 字段只有 16 位，用来区分本进程发出的回包 */
	pid = getpid() & 0xffff;
	datalen = ICMP_DATALEN;
	host = argv[argc - 1];

	ai = host_serv(host, NULL, AF_INET, 0);
	if (ai == NULL)
		exit(EXIT_FAILURE);

	inet_ntop(AF_INET,
			&((struct sockaddr_in *)ai->ai_addr)->sin_addr,
			ipstr, sizeof(ipstr));
	printf("ping %s (%s): %d data bytes.\n",
			ai->ai_canonname ? ai->ai_canonname : ipstr,
			ipstr, datalen);

	sasend = ai->ai_addr;
	salen = ai->ai_addrlen;
	sarecv = malloc(salen);
	if (sarecv == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	loopSend();
	return 0;
}
