#include "ping.h"

/*
 * 在 sendbuf 中组装一帧 ICMP Echo Request：
 *   type=8/code=0，id=pid，seq 递增，
 *   数据区先填 0xa5，再把发送时刻写入开头，供对端原样回显以算 RTT。
 */
void createPingIcmpPackage(void)
{
	struct icmp *pingicmp = (struct icmp *)sendbuf;

	pingicmp->icmp_type = ICMP_ECHO;
	pingicmp->icmp_code = 0;
	pingicmp->icmp_id = pid;
	pingicmp->icmp_seq = nsent++;
	memset(pingicmp->icmp_data, 0xa5, datalen);
	gettimeofday((struct timeval *)pingicmp->icmp_data, NULL);

	pingicmp->icmp_cksum = 0;
	pingicmp->icmp_cksum = in_cksum((u_short *)pingicmp,
			ICMP_HLEN + datalen);
}
