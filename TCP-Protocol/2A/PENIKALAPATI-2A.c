//server

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<errno.h>
#include<unistd.h>
#define PORT 8080

int main(int argc,char *argv[])
{
	int server_fd;
	//socket creation
	if((server_fd=socket(AF_INET,SOCK_STREAM,0)) <=0)
	{
		perror("socket creation failed");
		exit(-1);
	}
	//binding
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr= INADDR_ANY;

	if(bind(server_fd,(struct sockaddr *)&address,sizeof(address))<0)
	{
		perror("failure in bind");
		exit(-1);
	}
	//listen
	if(listen(server_fd,100) <0)
	{
		perror("failure in listen");
		exit(-1);
	}
	//accept
	int cli;
	int addr_len = sizeof(address);
	if((cli= accept(server_fd,(struct sockaddr *)&address,(socklen_t *)&addr_len))<0)
	{
		perror("failure in accept");
		exit(-1);
	}
	int readval;
	char buffr[1024]={0};
	readval = read(cli,buffr,1024);
	printf("%s\n",buffr);
	char *msg = "Helloo this is server";
	send(cli,msg,strlen(msg),0);
	
	char *belong = "This number belongs to NITC S6 batch";
	char *ok = "OK";
	char *my = "Hello:b160664cs";
	while(1)
	{
		char roll[20];
		readval= read(cli,roll,20);
		printf("%s",roll);
		if(strcmp(my,roll)==0)
			send(cli,belong,strlen(belong),0);
		else
			send(cli,ok,strlen(ok),0);
	}

	close(server_fd);

return 0;
}

//client

#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#define PORT 8080

int main(int argc,char *argv[])
{
	int sock;
	if((sock= socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("socket creation error");
		exit(-1);
	}

	struct sockaddr_in server_addr;
	memset(&server_addr,'0',sizeof(server_addr));
	server_addr.sin_family= AF_INET;
	server_addr.sin_port = htons(PORT);

	//ipv4 add from text to bin
	if(inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr) <=0)
	{
		perror("Invalid address");
		exit(-1);
	}
	//connect
	if(connect(sock,(struct sockaddr *)&server_addr,sizeof(server_addr)) <0)
	{
		perror("connection error");
		exit(-1);
	}

	char *hello = "hello this is client";
	send(sock,hello,strlen(hello),0);
	printf("HELLO msz sent\n");
	int valread;
	char buffer[1024]={0};
	valread = read(sock,buffer,1024);
	printf("%s\n",buffer);
	char *belong = "This number belongs to NITC S6 batch";
        char *ok = "OK";

	while(1)
	{
		printf("ENTER INPUT: ");
		char str[100];
		scanf("%s",str);
		send(sock,str,strlen(str),0);
		char my[100]={0};
		valread = read(sock,my,100);
		printf("%s\n",my);
		if(strcmp(my,belong)==0)
			break;
	}

	close(sock);
return 0;
}



