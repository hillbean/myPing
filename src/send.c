#include "ping.h"


void mysend() {
	createPingIcmpPackage();
	sendto(sockfd, sendbuf, 8 + datalen, 0, sasend, salen);
}

void sig_alrm(int signo) {
	/*signal(signo, mysend);
	alarm(1);*/
	mysend();
	return;
}
