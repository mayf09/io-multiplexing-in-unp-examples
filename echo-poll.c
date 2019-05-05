#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <errno.h>

#include "echo.h"

#define MAXLINE 1024
#define OPEN_MAX 16
#define SERV_PORT 9999
#define LISTENQ 5
#define INFTIM -1

#define ECHO_DEBUG

int
main(int argc, char **argv)
{
	int					i, maxi, listenfd, connfd, sockfd;
	int					nready;
	ssize_t				n;
	char				buf[MAXLINE];
	socklen_t			clilen;
	struct pollfd		client[OPEN_MAX];
	struct sockaddr_in	cliaddr, servaddr;

#ifdef ECHO_DEBUG
	print_poll_macro();
#endif

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

#ifdef ECHO_DEBUG
	print_fd("listenfd", __LINE__, listenfd);
#endif

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	listen(listenfd, LISTENQ);

	client[0].fd = listenfd;
	client[0].events = POLLIN;
	for (i = 1; i < OPEN_MAX; i++)
		client[i].fd = -1;		/* -1 indicates available entry */
	maxi = 0;					/* max index into client[] array */

#ifdef ECHO_DEBUG
	print_poll_fds("client", __LINE__, client);
#endif

	for ( ; ; ) {
		nready = poll(client, maxi+1, INFTIM);

#ifdef ECHO_DEBUG
		print_poll_fds("client", __LINE__, client);
#endif

		if (client[0].revents & POLLIN) {	/* new client connection */
			clilen = sizeof(cliaddr);
			connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);

#ifdef ECHO_DEBUG
			print_fd("connfd", __LINE__, connfd);
#endif
#ifdef NOTDEF
			printf("new client: %s\n", Sock_ntop((SA *) &cliaddr, clilen));
#endif

			for (i = 1; i < OPEN_MAX; i++)
				if (client[i].fd < 0) {
					client[i].fd = connfd;	/* save descriptor */
					break;
				}
			if (i == OPEN_MAX)
				perror("too many clients");

			client[i].events = POLLIN;

#ifdef ECHO_DEBUG
			print_poll_fds("client", __LINE__, client);
#endif

			if (i > maxi)
				maxi = i;				/* max index in client[] array */

			if (--nready <= 0)
				continue;				/* no more readable descriptors */
		}

		for (i = 1; i <= maxi; i++) {	/* check all clients for data */
			if ( (sockfd = client[i].fd) < 0)
				continue;
			if (client[i].revents & (POLLIN | POLLERR)) {
				if ( (n = read(sockfd, buf, MAXLINE)) < 0) {
					if (errno == ECONNRESET) {
							/*4connection reset by client */
#ifdef	NOTDEF
						printf("client[%d] aborted connection\n", i);
#endif
						close(sockfd);
						client[i].fd = -1;
					} else
						perror("read error");
				} else if (n == 0) {
						/*4connection closed by client */
#ifdef	NOTDEF
					printf("client[%d] closed connection\n", i);
#endif
					close(sockfd);
					client[i].fd = -1;

#ifdef ECHO_DEBUG
					print_poll_fds("after close", __LINE__, client);
#endif
				} else
					write(sockfd, buf, n);

				if (--nready <= 0)
					break;				/* no more readable descriptors */
			}
		}
	}
}
