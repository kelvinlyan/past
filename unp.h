#ifndef _UNP_H
#define _UNP_H

#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>

#define MAXLINE 128

int Socket(int family, int type, int protocol);
int Connect(int sockfd, const struct sockaddr* servaddr, socklen_t addrlen);
int Bind(int sockfd, const struct sockaddr* myaddr, socklen_t addrlen);
int Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr* cliaddr, socklen_t* addrlen);
int Close(int sockfd);
int Select(int maxfd, fd_set* readset, fd_set* writeset, fd_set* exceptset, struct timeval* timeout);
int max(int a, int b);
int readn(int fd, void* vptr, size_t n);
ssize_t writen(int fd, const void* vptr, size_t n);
ssize_t readline(int fd, void* vptr, size_t n);
void str_cli(FILE* fp, int sockfd);
void str_ser(int sockfd);
void str_cli_select(FILE* fp, int sockfd);
void GetSockOpts(int sockfd);


int Socket(int family, int type, int protocol)
{
	int n;
	if((n = socket(family, type, protocol)) < 0){
		perror("socket");
		exit(0);
	}
	return n;
}

int Connect(int sockfd, const struct sockaddr* servaddr, socklen_t addrlen)
{
	int n;
	if((n = connect(sockfd, servaddr, addrlen)) < 0)
	{
		perror("connect");
		exit(0);
	}
	return n;        
}

int Bind(int sockfd, const struct sockaddr* myaddr, socklen_t addrlen)
{
	int n;
	if((n = bind(sockfd, myaddr, addrlen)) < 0)
	{
		perror("bind");
		exit(0);
	}
	return n;
}

int Listen(int sockfd, int backlog)
{ 
	char* ptr = NULL;
	if((ptr = getenv("LISTENQ")) != NULL){
		printf("Default Listen Queen is %s!\n", ptr);
		backlog = atoi(ptr);
	}
	int n;
	if((n = listen(sockfd, backlog)) < 0)
	{
		perror("listen");
		exit(0);
	}
	return n;
}

int Accept(int sockfd, struct sockaddr* cliaddr, socklen_t* addrlen)
{
	int n;
	if((n = accept(sockfd, cliaddr, addrlen)) < 0)
	{
		perror("accept");
		exit(0);
	}
	return n;
}

int Close(int sockfd)
{
	int n;
	if((n = close(sockfd)) < 0)
	{
		perror("close");
		exit(0);
	}
	return n;
}

int Select(int maxfd, fd_set* readset, fd_set* writeset, fd_set* exceptset, struct timeval* timeout)
{
	int n;
	if((n = select(maxfd, readset, writeset, exceptset, timeout)) < 0)
	{
		perror("select");
		exit(0);
	}
	return n;
}

int max(int a, int b)
{
	return a > b ? a : b;
}

ssize_t readn(int fd, void* vptr, size_t n)
{
	printf("reading..\n");
	size_t nleft;
	ssize_t nread;
	char* ptr = (char*)vptr;
	nleft = n;
	while(nleft > 0)
	{
		if((nread = read(fd, ptr, nleft)) < 0)
		{
			if(errno == EINTR)
				nread = 0;
			else{
				perror("read");
				exit(0);
			}
		}
		else if(nread == 0)
			break;
		printf("read count:%d\n", nread);
		nleft -= nread;
		ptr += nread;
	}
	return (n - nleft);
}  

ssize_t writen(int fd, const void* vptr, size_t n)
{
	printf("writing..\n");
	size_t nleft;
	ssize_t nwritten;
	const char* ptr = (const char*)vptr;
	nleft = n;
	while(nleft > 0)
	{
		if((nwritten = write(fd, ptr, nleft)) <= 0)
		{
			if(nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else{
				perror("write");
				exit(0);
			}
		}
		nleft -= nwritten;
		ptr += nwritten;
	}
	printf("write count:%d\n", n);
	return n;
} 

ssize_t readline(int fd, void* vptr, size_t n)
{ 
	printf("readline ing..\n");
	size_t nleft;
	ssize_t nread;
	char* ptr = (char*)vptr;
	nleft = n;
	int notread = 0;
	while(nleft > 0)
	{
		if((nread = read(fd, ptr, nleft)) < 0)
		{
			if(errno == EINTR)
				nread = 0;
			else{
				perror("readline");
				exit(0);
			}
		}
		else if(nread == 0)
			break;
		bool flag = false;
		for(int i = 0; i < nread; ++i)
		{
			if(ptr[i] == 0 || ptr[i] == '\n')
			{
				notread = nread - i - 1;
				flag = true;
				break;
			}
		}
		nleft -= nread;
		ptr += nread;
		if(flag){
			break;
		}
	}
	printf("actual count:%d, line count:%d\n", n - nleft, n - nleft - notread);
	return (n - nleft);
}

void str_cli(FILE* fp, int sockfd)
{
	char sendline[MAXLINE], recvline[MAXLINE];
	while(1)
	{
		fgets(sendline, MAXLINE, fp);
		writen(sockfd, sendline, 1);
		int n = writen(sockfd, sendline + 1, strlen(sendline));
		readline(sockfd, recvline, MAXLINE);
		fputs(recvline, stdout);
	}
} 

void str_ser(int sockfd)
{
	char recvline[MAXLINE];
	while(1){
		readline(sockfd, recvline, MAXLINE);
		writen(sockfd, recvline, strlen(recvline) + 1);
	}
}

void str_cli_select(FILE* fp, int sockfd)
{
	char sendline[MAXLINE], recvline[MAXLINE];
	fd_set sets;
	FD_ZERO(&sets);
	while(1)
	{
		FD_SET(sockfd, &sets);
		FD_SET(fileno(fp), &sets);
		int maxfd = max(sockfd, fileno(fp)) + 1;
		int n = Select(maxfd, &sets, NULL, NULL, NULL);
		if(FD_ISSET(sockfd, &sets))
		{
			if(readline(sockfd, recvline, MAXLINE) == 0){
				printf("close\n");
				break;
			}
			else{
				printf("recv\n");
				fputs(recvline, stdout);
			}
		}
		if(FD_ISSET(fileno(fp), &sets))
		{
			printf("fgets\n");
			fgets(sendline, MAXLINE, fp);
			writen(sockfd, sendline, strlen(sendline) + 1);
		}

	}
}

void GetSockOpts(int sockfd)
{
	int result = 0;
	int len = sizeof(result);
	if(getsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &result, (socklen_t*)&len) == -1)
	{
		perror("GetSockOpts::SO_BROADCAST");
	}
	else{
		printf("GetSockOpts::SO_BROADCAST: %d\n", result);
	}
}





#endif


