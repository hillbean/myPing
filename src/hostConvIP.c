#include "ping.h"

struct addrinfo *
host_serv(const char *host, const char *serv, int family, int socktype) {
	int n;
	struct addrinfo hints, *result;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags = AI_CANONNAME; /* always return canonical name */
	hints.ai_family = family; /* 0, AF_INET, AF_INET6, etc. */
	hints.ai_socktype = socktype; /* 0, SOCK_STREAM, SOCK_DGRAM, etc. */

	if ((n = getaddrinfo(host, serv, &hints, &result)) != 0)
		printf("host_serv error for %s, %s: %s",
				(host == NULL) ? "(no hostname)" : host,
				(serv == NULL) ? "(no service name)" : serv, gai_strerror(n));

	return (result);
}
