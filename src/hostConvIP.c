#include "ping.h"

/*
 * 把主机名或点分十进制 IP 解析成 addrinfo。
 * family / socktype 传给 getaddrinfo 做过滤；失败时打印原因并返回 NULL。
 */
struct addrinfo *
host_serv(const char *host, const char *serv, int family, int socktype)
{
	int n;
	struct addrinfo hints, *result = NULL;

	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_CANONNAME;	/* 同时返回规范主机名 */
	hints.ai_family = family;
	hints.ai_socktype = socktype;

	n = getaddrinfo(host, serv, &hints, &result);
	if (n != 0) {
		fprintf(stderr, "host_serv error for %s: %s\n",
				host ? host : "(no hostname)",
				gai_strerror(n));
		return NULL;
	}

	return result;
}
