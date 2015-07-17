#include	"ping.h"

float tv_sub(struct timeval *out, struct timeval *in) {
	double outa = out->tv_sec*1000.0+out->tv_usec/1000.0;
	double ina = in->tv_sec*1000.0+in->tv_usec/1000.0;
	return (out->tv_usec-in->tv_usec)/1000.0;
}

void proc_v4(char *ptr, ssize_t len, struct msghdr *msg, struct timeval *tvrecv) {
	int hlen1, icmplen;
	double rtt;
	struct ip *ip;
	struct icmp *icmp;
	struct timeval *tvsend;

/*	int i;
	for (i = 0; i < len; i++)
		printf("%x ", *(msg + i));
	printf("\n");

	for (i = 0; i < msg->msg_iovlen; i++)
		printf("%x ", *(ptr + i));
	printf("\n");*/

	ip = (struct ip *) ptr; /* start of IP header */
	hlen1 = ip->ip_hl << 2; /* length of IP header */

	if (ip->ip_p != IPPROTO_ICMP)
		return; /* not ICMP */

	icmp = (struct icmp *) (ptr + hlen1); /* start of ICMP header */
	if ((icmplen = len - hlen1) < 8)
		return; /* malformed packet */

	if (icmp->icmp_type == ICMP_ECHOREPLY) {
		if (icmp->icmp_id != pid)
			return; /* not a response to our ECHO_REQUEST */
		if (icmplen < 16)
			return; /* not enough data to use */

		tvsend = (struct timeval *) icmp->icmp_data;
		rtt = tvrecv->tv_sec * 1000.0 + tvrecv->tv_usec / 1000.0;
		float timeDef = tv_sub(tvrecv, tvsend);

		char ipstr[16];
		struct sockaddr_in * ipin = (struct sockaddr_in *)sarecv;
		inet_ntop(AF_INET,&(ipin->sin_addr), ipstr, salen);

		printf("%d bytes from %s: seq=%u, ttl=%d, rtt=%.3f ms\n", icmplen,
				ipstr, icmp->icmp_seq, ip->ip_ttl, timeDef);
	}
}
