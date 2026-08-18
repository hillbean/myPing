#include "ping.h"

/* 返回 recv - send 的毫秒差，正确处理秒与微秒进位 */
static double tv_sub_ms(const struct timeval *tvrecv,
		const struct timeval *tvsend)
{
	double ms_recv = tvrecv->tv_sec * 1000.0 + tvrecv->tv_usec / 1000.0;
	double ms_send = tvsend->tv_sec * 1000.0 + tvsend->tv_usec / 1000.0;
	return ms_recv - ms_send;
}

void proc_v4(char *ptr, ssize_t len, struct msghdr *msg,
		struct timeval *tvrecv)
{
	int hlen1, icmplen;
	struct ip *ip;
	struct icmp *icmp;
	struct timeval *tvsend;
	struct sockaddr_in *ipin;
	char ipstr[INET_ADDRSTRLEN];
	double rtt;

	if (len < (ssize_t)sizeof(struct ip))
		return;

	ip = (struct ip *)ptr;
	hlen1 = ip->ip_hl << 2;		/* IHL 以 4 字节为单位 */
	if (hlen1 < 20)
		return;
	if (ip->ip_p != IPPROTO_ICMP)
		return;

	icmp = (struct icmp *)(ptr + hlen1);
	icmplen = (int)len - hlen1;
	if (icmplen < ICMP_HLEN)
		return;

	/* 只处理对本进程 Echo Request 的应答；数据区至少要有 timeval */
	if (icmp->icmp_type != ICMP_ECHOREPLY)
		return;
	if (icmp->icmp_id != pid)
		return;
	if (icmplen < ICMP_HLEN + (int)sizeof(struct timeval))
		return;

	tvsend = (struct timeval *)icmp->icmp_data;
	rtt = tv_sub_ms(tvrecv, tvsend);

	ipin = (struct sockaddr_in *)msg->msg_name;
	inet_ntop(AF_INET, &ipin->sin_addr, ipstr, sizeof(ipstr));

	printf("%d bytes from %s: seq=%u, ttl=%d, rtt=%.3f ms\n",
			icmplen, ipstr, icmp->icmp_seq, ip->ip_ttl, rtt);
}
