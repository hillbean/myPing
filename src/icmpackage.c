#include "ping.h"

void createPingIcmpPackage() {
	struct icmp* pingicmp;
	pingicmp = (struct icmp*) sendbuf;

	pingicmp->icmp_type = ICMP_ECHO;
	pingicmp->icmp_code = 0;
	pingicmp->icmp_id = pid;
	pingicmp->icmp_seq = nsent++;
	memset(pingicmp->icmp_data, 0xa5, datalen);
	gettimeofday((struct timeval *) pingicmp->icmp_data, NULL);

	pingicmp->icmp_cksum = 0;
	pingicmp->icmp_cksum = in_cksum((u_short *) pingicmp, 8 + datalen);
}
