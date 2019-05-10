//CLIENT B CODE
           
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<unistd.h>
#include<errno.h>

#include<sys/types.h>
#include<sys/socket.h>

#include<netinet/in.h>
#include<ctype.h>
#define buffer_size 1024
#define PORT_A 8081

char buffer[buffer_size];


int main(){
	//create a socket
	int cli_socket;
	cli_socket=socket(AF_INET,SOCK_STREAM,0);

	//specify an address for the socket
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT_A);
	server_address.sin_addr.s_addr = INADDR_ANY;

	int connection_status = connect(cli_socket,(struct sockaddr *) &server_address, sizeof(server_address));
	// check for error in connection
	if(connection_status == -1)
	{
		printf("There was an error making connection.\n");	
	}

	char ch=0;
	while(1)
	{
		scanf("%c",&ch);
		if(ch=='\n')
		{
			send(cli_socket,&ch,sizeof(char),0);
			int val= read(cli_socket,buffer,buffer_size);
			//if(strcmp(buffer,"stop")==0) break;
			if(buffer[0]=='s' && buffer[1]=='t' && buffer[2]=='o' && buffer[3]=='p') break;
			if(val==0) break;
			FILE *fp = fopen("akhilC2.txt","a");
			fprintf(fp,"%s\n",buffer);
			printf("%s\n",buffer);
			fclose(fp);
		}
	}
	
//	printf("got out from loop");
	close(cli_socket);
	return 0;
			
		
/*
	//sending file
	FILE *f;
	f = fopen("RollC.txt","r");
	char ch;
	int words=0;
	while((ch = getc(f))!=EOF)
	{
		fscanf(f,"%s",buffer);
		if(isspace(ch) || ch=='\t')
			words++;
	}
	send(network_socket,&words,sizeof(int),0);
        rewind(f);

	char c;
	while(c!=EOF)
	{
		fscanf(f,"%s",buffer);
		send(network_socket,buffer,sizeof(buffer),0);
		c=getc(f);
	}

	

	//receive data
	char a[6];
	recv(network_socket,a,sizeof(a),0);
        FILE *fp;
        fp = fopen("NumberC.txt","w");
        fprintf(fp,"%s",a);
        fclose(fp);	

	close(network_socket);

	return 0;
*/
}
