#include "ping.h"

/*
 * RFC 1071 Internet 校验和：按 16 位累加，把高 16 位进位折回，再取反。
 * ICMP 头部在计算前须把 icmp_cksum 置 0。
 */
u_int16_t
in_cksum(u_short *addr, int len)
{
	int nleft = len;
	u_short *w = addr;
	int sum = 0;
	u_short answer = 0;

	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}

	/* 奇数长度时，最后一字节按高 8 位补进累加器 */
	if (nleft == 1) {
		*(u_char *)(&answer) = *(u_char *)w;
		sum += answer;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;
	return answer;
}
