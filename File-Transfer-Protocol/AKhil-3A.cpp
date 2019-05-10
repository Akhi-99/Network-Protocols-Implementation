//client program
#include<bits/stdc++.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

using namespace std;

void error(const char *str)
{
	perror(str);
	exit(1);
}

int create_socket(int port,char *addr);

int main(int argc,char *argv[])
{

	int sockfd,portno,sockfd2,sockfd3,n;
	char buffer[256];
	struct sockaddr_in serv_addr;
	struct hostent *server;

	if(argc<3)
	{
		printf("We must need three arguments\n");
		exit(1);
	}

	portno=atoi(argv[2]);
	sockfd=socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		error("erroe opening socket");
		exit(1);
	}

	server = gethostbyname(argv[1]);
	if(server==NULL)
	{
		printf("Error,no such hhost");
		exit(1);
	}

	bzero((char *) &serv_addr ,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	bcopy((char *) server->h_addr , (char *) &serv_addr.sin_addr.s_addr ,server->h_length);
	serv_addr.sin_port =htons(portno);

	if(connect(sockfd, (struct sockaddr *) &serv_addr ,sizeof(serv_addr)) <0)
		error("Connection failed");


//connected with 20021
	while(1)
	{
		//scanf("%s",buffer);
		char command[100];
		char filename[100];
		//getline(cin ,input);
		cin.getline(command,sizeof(command));
		//send(sockfd,buffer,256,0);
		send(sockfd,command,strlen(command),0);
		if (strncmp(command,"RETR ",strlen("RETR "))==0)
		{     
			memcpy(filename,command+5,strlen(command));
		    cout<<"file is"<<filename;
		    send(sockfd,filename,strlen(filename),0);
		}

		if(strncmp(command,"GOODBYE SERVER",sizeof("GOODBYE SERVER"))==0)
		{
			recv(sockfd,buffer,256,0);
			cout <<buffer<<endl;
			close(sockfd);
		}

		recv(sockfd,buffer,256,0);
		if(strcmp(buffer,"CONN 20020")==0)//buffer[0]=='C' && buffer[1]=='O' && buffer[2]=='N' && buffer[3]=='N')
		{
			/*string port2="";
			port2+=buffer[5];port2+=buffer[6];port2+=buffer[7];
			port2+=buffer[8];port2+=buffer[9];

			cout <<port2<<endl;
			*/
			if(fork()==0)
			{
				//data connection
				int data_port=20020;//=atoi(port2);
				//char filename[30];
				sockfd2=create_socket(data_port,argv[1]);
				//file transfer
				//bzero(filename,sizeof(filename));
				//recv(sockfd2,filename,30,0);
				FILE *fp;

				cout <<filename<<endl;
				int words,cnt=0;
				fp=fopen("temp2.txt","w");

				recv(sockfd2 ,&words,sizeof(int),0);

				while(cnt!=words)
				{
					recv(sockfd2, buffer ,256,0);
					fprintf(fp, "%s" ,buffer);
					cnt++;
				}

	

				printf("client received file %s \n",filename);

				fclose(fp);
				close(sockfd2);

				//return 0;
				exit(5);

			}

		}
		else if(strcmp(buffer,"CONN 20023")==0)
		{
			int data_port=20023;
			sockfd3=create_socket(data_port,argv[1]);

			recv(sockfd3,buffer,256,0);
			cout <<buffer<<endl;
			close(sockfd3);


		}
		break;

		
	}
	close(sockfd);
}


int create_socket(int port,char *addr)
{
 int sockfd;
 struct sockaddr_in serv_addr;
 struct hostent *server;

server=gethostbyname(addr);
if(server==NULL)
	{
		printf("Error,no such hhost");
		exit(1);
	}


 //Create a socket for the client
 //If sockfd<0 there was an error in the creation of the socket
 if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
  cerr<<"Problem in creating the socket"<<endl;
  exit(2);
 }

 //Creation of the socket
 
	bzero((char *) &serv_addr ,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	bcopy((char *) server->h_addr , (char *) &serv_addr.sin_addr.s_addr ,server->h_length);
	serv_addr.sin_port =htons(port);

	if(connect(sockfd, (struct sockaddr *) &serv_addr ,sizeof(serv_addr)) <0)
		error("Connection2 failed");


return(sockfd);
}


////////////////////////////////////////server
//server program

/*#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
*/
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <iostream>

using namespace std;




void error(const char *str)
{
	perror(str);
	exit(1);
}

int accept_conn(int sock);
int create_socket(int port);

int main(int argc,char *argv[])
{
	//int length =256;
	int sockfd,newsockfd,newsockfd2,newsockfd3,portno,n;//,clilen;
	char buffer[256];
	socklen_t clilen;
	if(argc<2)
	{
		printf("Port number not provided\n");
		exit(1);
	}

	struct sockaddr_in serv_addr,cli_addr;
	//socklen_t clilen;

	sockfd=socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		error("Error Opening Socket");
		exit(1);
	}

	bzero((char *) &serv_addr,sizeof(serv_addr));
	//portno= 5001;
	portno=atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	

	//binding
	if( bind(sockfd, (struct sockaddr *) &serv_addr ,sizeof(serv_addr)) <0)
	{
		error("Binding Failed");
		exit(1);
	}
	//listen
	listen(sockfd,5);
	clilen= sizeof(cli_addr);

	newsockfd=  accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	if(newsockfd < 0)
	{
		error("Error on accept");
		exit(1);
	}


	char filename[100];
	int portno1=20020,portno2=20021,portno3=20023;

	while(1)
	{
		if(recv(newsockfd,buffer,256,0)<0)
		{
			printf("error reading");
			exit(1);
		}
		
		if(strncmp(buffer,"RETR",strlen("RETR"))==0)
		{
			//RETR FILENAME
			//string filename=s2;
			//string portstring;
			char filename[100];
			recv(newsockfd,filename,100,0);
			//sprintf(portstring,"%d",portno1);
			//portstring="CONN "+portstring;
			///send(newsockfd,portstring,portstring.length(),0);
			//child process
			FILE *fp=fopen(filename,"r");
			if(fp==NULL)
			{
				newsockfd3= create_socket(portno3);
				send(newsockfd,"CONN 20023",strlen("CONN 20023"),0);
				int temp=accept_conn(newsockfd3);

				send(newsockfd3,"<SERVER TIME>: FILE NOT FOUND AT CURRENT WORKING DIRECTORY",strlen("<SERVER TIME>: FILE NOT FOUND AT CURRENT WORKING DIRECTORY"),0);
				
			}
			//fclose(fp);
			else{
				fclose(fp);
				newsockfd2= create_socket(portno1);
				send(newsockfd,"CONN 20020",strlen("CONN 20020"),0);
				int temp=accept_conn(newsockfd2);

			if(fork()==0)
			{
				//transfer file
				//create newsocket
				/*newsockfd2= create_socket(portno1);
				send(newsockfd,"CONN 20020",strlen("CONN 20020"),0);
				int temp=accept_conn(newsockfd2);
				*/
				//now transfer file

				FILE *f;
				int words=0;
				char c;

				//send(newsockfd2,filename,filename.length(),0);
				f=fopen(filename , "r");
				while (fgets(buffer, sizeof(buffer), f))
				{
					//printf("%s",);
					words++;
				}	
				fclose(f);
	
				send(newsockfd2, &words,sizeof(int),0);

				f=fopen(filename,"r");
	
				while (fgets(buffer, sizeof(buffer), f))
				{
					send(newsockfd2 , buffer ,256,0);
				}

				printf("transfer of file %s done\n",filename);

				fclose(f);	

				//close(newsockfd2);
				//return 0;
				exit(5);

			}}
				//parent 


				
		}
		else
		{
			send(newsockfd,"THANK YOU, ALL CONNECTIONS SUCCESSFULLY TERMINATED",strlen("THANK YOU, ALL CONNECTIONS SUCCESSFULLY TERMINATED"),0);
				   
			break;
		}

		//break;



	}

	close(newsockfd);
	close(sockfd);





}

int create_socket(int port)
{
int listenfd;
struct sockaddr_in dataservaddr;


//Create a socket for the soclet
//If sockfd<0 there was an error in the creation of the socket
if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
cerr<<"Problem in creating the data socket"<<endl;
exit(2);
}


//preparation of the socket address
dataservaddr.sin_family = AF_INET;
dataservaddr.sin_addr.s_addr = htonl(INADDR_ANY);
dataservaddr.sin_port = htons(port);

if ((bind (listenfd, (struct sockaddr *) &dataservaddr, sizeof(dataservaddr))) <0) {
cerr<<"Problem in binding the data socket"<<endl;
exit(2);
}

 //listen to the socket by creating a connection queue, then wait for clients
 listen (listenfd, 1);

return(listenfd);
}


int accept_conn(int sock)
{
int dataconnfd;
socklen_t dataclilen;
struct sockaddr_in datacliaddr;

dataclilen = sizeof(datacliaddr);
  //accept a connection
if ((dataconnfd = accept (sock, (struct sockaddr *) &datacliaddr, &dataclilen)) <0) {
cerr<<"Problem in accepting the data socket"<<endl;
exit(2);
}

return(dataconnfd);
}	
