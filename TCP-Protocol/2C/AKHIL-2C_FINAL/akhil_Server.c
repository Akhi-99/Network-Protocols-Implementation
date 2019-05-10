//SERVER CODE

#include<stdio.h>
#include<stdlib.h>

#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<ctype.h>
#include<netinet/in.h>

#define buffer_size 1024

#define PORT_A 8080
#define PORT_B 8081

char buffer[buffer_size];
int main()
{
      char server_message[256]  = "HII from server!";

      //create server socket
      int sock_A,sock_B;
      if((sock_A = socket(AF_INET,SOCK_STREAM,0))==-1)
		{
			printf("ERROR IN SOCKET CREATION");
			exit(-1);
		}
	  if((sock_B = socket(AF_INET,SOCK_STREAM,0))==-1)
		{
			printf("ERROR IN SOCKET CREATION");
			exit(-1);
		}
      //define server address
      struct sockaddr_in addr_A,addr_B;
      addr_A.sin_family = AF_INET;
      addr_A.sin_port = htons(PORT_A);
      addr_A.sin_addr.s_addr = INADDR_ANY;
	  memset(&(addr_A.sin_zero),'\0',8*sizeof(char));	
		

      addr_B.sin_family = AF_INET;
      addr_B.sin_port = htons(PORT_B);
      addr_B.sin_addr.s_addr = INADDR_ANY;
	  memset(&(addr_B.sin_zero),'\0',8*sizeof(char));	

      //bind the socket to our specified IP and port
      if((bind(sock_A,(struct sockaddr*) &addr_A, sizeof(addr_A))==-1))
		{
			printf("ERROR IN BIND");
			exit(-1);
		}
			
	  if((bind(sock_B,(struct sockaddr*) &addr_B, sizeof(addr_B))==-1))
		{
			printf("ERROR IN BIND");
			exit(-1);
		}
	
	  //listen
      listen(sock_A,5);
	  listen(sock_B,5);
	

	//accept clients
		struct sockaddr_in cli_addr_A,cli_addr_B;
		int len_A =sizeof(cli_addr_A),len_B =sizeof(cli_addr_B);
      int cli_A,cli_B;

		cli_A = accept(sock_A,(struct sockaddr *)&cli_addr_A,&len_A);
		cli_B = accept(sock_B,(struct sockaddr *)&cli_addr_B,&len_B);

	//send and receive
		int closed_A=0,closed_B=0;
		char ch;
		while( (!closed_A) || (!closed_B))
		{	
			int val_A=0,val_B=0;
			if(!closed_A)
			{
				val_A = read(cli_A,&ch,sizeof(char));
			}
			
			if(!closed_B)
			{
				val_B = read(cli_B,&ch,sizeof(char));
			}
				
				if(val_A!=0 || ch=='\n')
				{
					FILE *fa = fopen("RollS.txt","r+");
					char cc= getc(fa);
					if(cc==EOF)	//file is empty
					{
						//close connection
						char *stop = "stop";
						send(cli_A,stop,strlen(stop),0);
						close(sock_A);
						closed_A=1;	
					}
					else
					{	
						rewind(fa);
						fscanf(fa,"%s",buffer);
						send(cli_A,buffer,sizeof(buffer),0);
						//delete the first roll num;						
						//char c=getc(fa);
						char c;
						FILE *fr = fopen("Replica.txt","w");
						while(c!=EOF)
						{
							fscanf(fa,"%s",buffer);
							fprintf(fr,"%s\n",buffer);
							c=getc(fa);
						}
						fclose(fr);
						fclose(fa);
						remove("RollS.txt");
						rename("Replica.txt","RollS.txt");
					}
				}
			
			
			
				if(val_B!=0 || ch=='\n')
				{
					FILE *fb = fopen("RollS.txt","r+");
					char cc= getc(fb);
					if(cc==EOF)		//file is empty
					{
						//close connection
						char *stop = "stop";
						send(cli_B,stop,strlen(stop),0);
						close(sock_B);
						closed_A=1;	
					}
					else
					{	
						rewind(fb);
						fscanf(fb,"%s",buffer);
						send(cli_B,buffer,sizeof(buffer),0);
						//delete the first roll num;
						
						//char c=getc(fb);
						char c;
						FILE *fr = fopen("Replica.txt","w");
						while(c!=EOF)
						{
							fscanf(fb,"%s",buffer);
							fprintf(fr,"%s\n",buffer);
							c=getc(fb);
						}
						fclose(fr);
						fclose(fb);
						remove("RollS.txt");
						rename("Replica.txt","RollS.txt");
					}
				}
					
			
		}

		//close(sock_A);
		//close(sock_B);

	return 0;
	
/*
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
*/

}


