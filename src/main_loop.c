#include "ping.h"

void loopSend(void)
{
	int size;
	char recvbuf[BUFSIZE];
	char controlbuf[BUFSIZE];
	struct msghdr msg;
	struct iovec iov;
	ssize_t n;
	struct timeval tval;

	/* ICMP Echo 必须用 RAW socket，创建后立刻丢掉 root 权限 */
	sockfd = socket(sasend->sa_family, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd < 0) {
		perror("socket");
		fprintf(stderr, "creating a raw ICMP socket requires root (sudo)\n");
		exit(EXIT_FAILURE);
	}
	setuid(getuid());

	size = 60 * 1024;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)) < 0)
		perror("setsockopt SO_RCVBUF");

	memset(&msg, 0, sizeof(msg));
	iov.iov_base = recvbuf;
	iov.iov_len = sizeof(recvbuf);
	msg.msg_name = sarecv;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;		/* 只有一块 iovec，原先误写为 4 */
	msg.msg_control = controlbuf;

	signal(SIGALRM, sig_alrm);
	sig_alrm(SIGALRM);		/* 立刻发第一帧，并启动 1 秒周期 */

	for (;;) {
		msg.msg_namelen = salen;
		msg.msg_controllen = sizeof(controlbuf);
		n = recvmsg(sockfd, &msg, 0);
		if (n < 0) {
			/* alarm 会中断阻塞的 recvmsg，继续等下一包即可 */
			if (errno == EINTR)
				continue;
			perror("recvmsg");
			continue;
		}

		gettimeofday(&tval, NULL);
		proc_v4(recvbuf, n, &msg, &tval);
	}
}
