#include "ping.h"



int main(int argc, char **argv) {
	struct addrinfo *ai;
	char ipstr[16];

	char * host = argv[argc - 1];
	pid = getpid() & 0xffff;
	datalen = 56;
	//printf("hostname(IP) : %s\npid: %x\n", host, pid);

	ai = host_serv(host, NULL, AF_INET, SOCK_STREAM);

	struct sockaddr_in *sin = (struct sockaddr_in *) ai->ai_addr;
	inet_ntop(AF_INET, &sin->sin_addr, ipstr, sizeof(ipstr));
	printf("ping %s (%s): %d data bytes.\n",
			ai->ai_canonname ? ai->ai_canonname : ipstr, ipstr, datalen);

	sasend = ai->ai_addr;
	salen = ai->ai_addrlen;
	sarecv = malloc(ai->ai_addrlen);
	loopSend();
}
