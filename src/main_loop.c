#include "ping.h"

void loopSend() {
	int size;
	char recvbuf[BUFSIZE];
	char controlbuf[BUFSIZE];
	struct msghdr msg;
	struct iovec iov;
	ssize_t n;
	struct timeval tval;

	sockfd = socket(sasend->sa_family, SOCK_RAW, IPPROTO_ICMP);
	setuid(getuid());

	size = 60 * 1024;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
	sig_alrm(SIGALRM); /* send first packet */

	iov.iov_base = recvbuf;
	iov.iov_len = sizeof(recvbuf);
	msg.msg_name = sarecv;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 4;
	msg.msg_control = controlbuf;
	while (1) {
		sig_alrm(SIGALRM);
		msg.msg_namelen = salen;
		msg.msg_controllen = sizeof(controlbuf);
		n = recvmsg(sockfd, &msg, MSG_DONTWAIT);

		gettimeofday(&tval, NULL);
		proc_v4(recvbuf, n, &msg, &tval);
		sleep(1);
	}
}
