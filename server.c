#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#include "proto.h"

#define BUFSIZE 1024
#define IPSTRSIZE 40

static void server_job(int sd)
{
	char buf[BUFSIZE];
	int len;		
	len = sprintf(buf,"%s","Welcome to our world!\n");
	if(send(sd,buf,len,0) < 0)
	{
		perror("send()");
		exit(1);
	}
}
int main()
{
	int sd,newsd;
	struct sockaddr_in laddr,raddr;
	socklen_t raddrlen;
	char ipstr[IPSTRSIZE];

	sd = socket(AF_INET,SOCK_STREAM,0);
	if(sd < 0)
	{
		perror("socket()\n");
		exit(1);
	}

	int val = 1;
	if(setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val)) < 0)
	{
		perror("setsockopt()");
		exit(1);
	}

	laddr.sin_family = AF_INET;
	laddr.sin_port = htons(atoi(SERVERPORT));
	inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr.s_addr);


	if(bind(sd,(struct sockaddr *)&laddr,sizeof(laddr)) < 0)
	{
		perror("bind() failed\n");
		exit(1);
	}


	if(listen(sd,10) < 0)
	{
		perror("listen() failed\n");
		exit(1);
	}
	
 
	raddrlen = sizeof(raddr);
	while(1)
	{
	newsd = accept(sd, (struct sockaddr *)&raddr, &raddrlen);
	if(newsd < 0)
	{
		perror("accept failed\n");
		exit(1);
	}
	inet_ntop(AF_INET,&raddr.sin_addr.s_addr,ipstr,IPSTRSIZE);
	printf("Client:%s:%d\n",ipstr,ntohs(raddr.sin_port));	
	server_job(newsd);
	close(newsd);	
	}

	close(sd);

	exit(0);
}
