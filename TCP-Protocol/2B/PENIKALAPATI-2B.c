//SERVER CODE

#include<stdio.h>
#include<stdlib.h>

#include<string.h>

#include<sys/types.h>
#include<sys/socket.h>

#include<netinet/in.h>
#define buffersize 1024

char buffer[buffersize];
int main()
{
      char server_message[256]  = "HII from server!";

      //create server socket
      int server_socket;
      server_socket = socket(AF_INET,SOCK_STREAM,0);
      //define server address
      struct sockaddr_in server_address;
      server_address.sin_family = AF_INET;
      server_address.sin_port = htons(8001);
      server_address.sin_addr.s_addr = INADDR_ANY;

      //bind the socket to our specified IP and port
      bind(server_socket,(struct sockaddr*) &server_address, sizeof(server_address));

      listen(server_socket,5);

      int client_socket;
   client_socket = accept(server_socket,NULL,NULL);

    FILE *f;
    f= fopen("RollS.txt","w");
    int ch=0,i;
    int words;

    recv(client_socket,&words,sizeof(int),0);
    while(ch!=words)
    {
	    recv(client_socket,buffer,sizeof(buffer),0);
	    fprintf(f,"%s\n",buffer);
	    ch++;
    }
   
    fclose(f);
   
    f=fopen("RollS.txt","r");
   
    char b[9];
   fscanf(f,"%s",b);
    char a[6];
    for(i=0;i<6;i++)
    {
	    a[i]=b[1+i];
    }
    send(client_socket,a,sizeof(a),0);

      //close the connection
      close(server_socket);

        return 0;

}

/*-------------------------------------------------------------------------------------------------------------------------------------------*/

//CLIENT CODE
           
#include<stdio.h>
#include<stdlib.h>

#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>

#include<netinet/in.h>
#include<ctype.h>
#define buffersize 1024

char buffer[buffersize];


int main(){
	//create a socket
	int network_socket;
	network_socket=socket(AF_INET,SOCK_STREAM,0);

	//specify an address for the socket
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8001);
	server_address.sin_addr.s_addr = INADDR_ANY;

	int connection_status = connect(network_socket,(struct sockaddr *) &server_address, sizeof(server_address));
	// check for error in connection
	if(connection_status == -1)
	{
		printf("There was an error making connection.\n");	
	}

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
}
