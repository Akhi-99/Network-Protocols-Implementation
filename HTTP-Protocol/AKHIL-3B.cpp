//SERVER
//#include<bits/stdc++.h>
#include<cstdio>
#include<stdlib.h>
#include<cstring>
#include<unistd.h>
#include<time.h>
#include<sys/stat.h>

#include<signal.h>


#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

using namespace std;
#define PORT1 20021
#define PORT2 80
#define PORT3 80

#define QUEUE_LIMIT 4


void get_file(char filename[], char file[])
{
	int i,j=0,count=0,temp;
	for(i=0;filename[i]!='\0' ; i++)
	{
		if(filename[i]=='/')
			count++;
	}
	printf("counnt:%d\n",count);
	if(count==0)
	{
		for(i=0;filename[i]!='\0' ; i++)
		{
			file[i]=filename[i];
		}
		file[i]=filename[i];
	}
	else
	{
		temp=0;
		for(i=0;filename[i]!='\0' ; i++)
		{
			if(filename[i]=='/')
			{
				temp++;
				if(temp==count)
					break;
			}
		}
		i++;
		for(j=0;filename[i]!='\0';i++,j++)
		{
			file[j]=filename[i];
		}
		file[j]='\0';


	}
	printf("file:%s\n",file);
	return;

}


void get_filename(char filename[],char relpath[],char http_version[], char buffer[],int j)
{
	//int j=4,
	int k=0;
	int i =0;
	relpath[0]='.';
	relpath[1]='/';
	for(i = 0 ; (buffer[j]!=' ' && buffer[j]!='\n') ;i++,j++)
	{
		filename[i] = buffer[j];
		relpath[i+2]=buffer[j];
	}
	if(buffer[j]=' ')
	{
		j++;
		for(k=0;buffer[j]!='\n';k++,j++)
		{
			http_version[k]=buffer[j];
		}
	}

	filename[i]='\0';
	relpath[i+2]='\0';
	http_version[k]='\0';
}


int file_transfer(char * filename, int sock_port1,FILE* fileptr)
{

	char buffer[100];
	bzero(buffer,sizeof(buffer));
	while(fgets(buffer,sizeof(buffer),fileptr)!=NULL)
	{
		send(sock_port1,buffer,sizeof(buffer),0);
		bzero(buffer,sizeof(buffer));		

	}	

	strcpy(buffer,"-1\n");
	send(sock_port1,buffer,sizeof(buffer),0);

	return 0;
}




void getFileCreationTime(char path[],int socknew)
{
	printf("%s\n",path);
    struct stat attr;
    stat(path, &attr);
   	char buff[100];
	strftime(buff, 100, "%d-%m-%y  %H:%M:%S", localtime(&(attr.st_ctime)));	printf("The file %s was last modified at %s\n", path, buff);
	send(socknew,buff,sizeof(buff),0);
}


int Checkinput1(char buffer[])
{
	int a=0;
	int i=0,j=0;
	for(i=0;buffer[i]!='\n';i++)
	{
		if(buffer[i]==' ')
			a++;
	}
	return a;

}
int Checkinput2(char buffer[])//,int a)//,int *b)
{
	int a=0;
	int i=0,j=0;
	for(i=0;buffer[i]!='\n';i++)
	{
		if(buffer[i]==' ' && buffer[i+1]==' ')
			a++;
		
	}
	return a;

}

void comm_server_to_client(int socknew)
{
	
	char buffer[100];
	int i,j;
	char filename[100];
	char file[100];
	char http_version[100];
	char relpath[100];
	char temper[100];
			
	
	while(1)
	{
		bzero(buffer,sizeof(buffer));
		recv(socknew,buffer,sizeof(buffer),0);
		printf("command is %s",buffer);
		
		int a=Checkinput1(buffer);
		int b=Checkinput2(buffer);
		if(((b)>0 || (a)<2 )&& strcmp(buffer,"exit\n")!=0)
		{
			bzero(buffer,sizeof(buffer));	
			strcpy(buffer,"NOT ok\n");
			printf("%s",buffer);
			send(socknew,buffer,sizeof(buffer),0);


			printf("/BAD request\n");
	  		bzero(temper,sizeof(temper));
    	  	strcpy(temper,"HTTP/1.1 400"); 
      		send(socknew,temper,sizeof(temper),0);
      		bzero(temper,sizeof(temper));
		}
		else//proceed
		{
			bzero(temper,sizeof(temper));	
			strcpy(temper,"ok\n");
			send(socknew,temper,sizeof(temper),0);
		
		if(strncmp(buffer,"GET ",4)==0)
		{
			get_filename(filename,relpath,http_version,buffer,4);
		
			
			if(strcmp(buffer,"GET / HTTP/1.1\n")==0)
			{
				printf("transfer index.html\n");
				bzero(buffer,sizeof(buffer));
				FILE* fileptr = fopen("index.html","r");
				char tempfile[]="index.html";
				int fileval = file_transfer(tempfile,socknew,fileptr);
				bzero(buffer,sizeof(buffer));
					//close(sock_port1);
				fclose(fileptr);			
				
			}
			else
			{
				if(strcmp(http_version,"HTTP/1.1")==0)
				{

					get_file(filename,file);
					printf("file:%s\n",file);

					FILE* fileptr = fopen(relpath,"r");
					if(fileptr != NULL)
					{
						if(file[0]=='1' && file[1]=='1')
						{
							printf("Forbidden file\n");
							bzero(temper,sizeof(temper));
    			  			strcpy(temper,"HTTP/1.1 403\n"); 
	    	  				send(socknew,temper,sizeof(temper),0);
      						bzero(temper,sizeof(temper));

						}
						else
						{
							bzero(buffer,sizeof(buffer));
							strcpy(buffer,"HTTP/1.1 200 ok\n");
							printf("%s",buffer);
							send(socknew,buffer,sizeof(buffer),0);
							bzero(buffer,sizeof(buffer));

							int fileval = file_transfer(filename,socknew,fileptr);
							bzero(buffer,sizeof(buffer));
							fclose(fileptr);
						}			
					}
					else
					{
						printf("FILe not found\n");

						bzero(buffer,sizeof(buffer));
						strcpy(buffer,"HTTP/1.1 404\n");
						printf("//>%s",buffer);
						send(socknew,buffer,sizeof(buffer),0);

					}
					

				}
				else
				{

					printf("BAD request\n");
	  		 		bzero(temper,sizeof(temper));
    	  			strcpy(temper,"HTTP/1.1 400\n"); 
      				send(socknew,temper,sizeof(temper),0);
      				bzero(temper,sizeof(temper));

				}
			}
			
		}
		else if(strncmp(buffer,"HEAD ",5)==0)
		{
			get_filename(filename,relpath,http_version,buffer,5);

			if(strcmp(buffer,"HEAD / HTTP/1.1\n")==0)
			{
				bzero(buffer,sizeof(buffer));
				strcpy(buffer,"HTTP/1.1 200 ok\n");
				printf("%s",buffer);
				send(socknew,buffer,sizeof(buffer),0);
				char tempfile[]="index.html";
				getFileCreationTime(tempfile,socknew);
					
			}			

			else if(strcmp(http_version,"HTTP/1.1")==0)
			{
				
				get_file(filename,file);
				printf("H:head action for %s\n",relpath);	

				FILE* tempptr=fopen(relpath,"r");
				if(tempptr!=NULL)
				{

					if(file[0]=='1' && file[1]=='1')
					{
						printf("Forbidden file\n");
						bzero(temper,sizeof(temper));
    			  		strcpy(temper,"HTTP/1.1 403\n"); 
	    	  			send(socknew,temper,sizeof(temper),0);
      					bzero(temper,sizeof(temper));
					}
					else
					{
						bzero(buffer,sizeof(buffer));
						strcpy(buffer,"HTTP/1.1 200 ok\n");
						printf(">%s",buffer);
						send(socknew,buffer,sizeof(buffer),0);
				
						getFileCreationTime(relpath,socknew);
					}
					fclose(tempptr);


				}
				else
				{
					printf("FILe not found\n");

						//“HTTP/1.1 404” if the file is not found
					bzero(temper,sizeof(temper));
					strcpy(temper,"HTTP/1.1 404\n");
					printf("//>%s",temper);
					send(socknew,temper,sizeof(temper),0);

				}
			}
			else
			{

				printf("//H:BAD request\n");
  		 		bzero(temper,sizeof(temper));
   	  			strcpy(temper,"HTTP/1.1 400\n"); 
   				send(socknew,temper,sizeof(temper),0);
   				bzero(temper,sizeof(temper));


			}


		}
		else if(strncmp(buffer,"PUT ",4)==0)
		{
			printf("//P:PUT action\n");
			get_filename(filename,relpath,http_version,buffer,4);
			get_file(filename,file);
			printf("%s\n",http_version);
			printf("%s\n",file);
			
			if(strcmp(http_version,"HTTP/1.1")==0)
			{	

					bzero(buffer,sizeof(buffer));
					recv(socknew, buffer,sizeof(buffer),0);
					if(strcmp(buffer,"Ready Receive\n")==0)
					{
						
						printf("transfer started\n");
						bzero(buffer,sizeof(buffer));
						recv(socknew, buffer,sizeof(buffer),0);
						FILE* serptr=fopen(file,"w");
						while(strcmp(buffer,"-1\n")!=0)
						{
							//printf("%s",buffer);
							fprintf(serptr,"%s",buffer);
							bzero(buffer,sizeof(buffer));
							recv(socknew,buffer,sizeof(buffer),0);
						}
						bzero(buffer,sizeof(buffer));
						fclose(serptr);
						printf("File received\n");
					}
					else
					{
						printf("Transfer failed\n");
					}

			}
			else
			{
				printf("/P:BAD request\n");
  		 		bzero(temper,sizeof(temper));
      			strcpy(temper,"HTTP/1.1 400\n"); 
      			send(socknew,temper,sizeof(temper),0);
      			bzero(temper,sizeof(temper));

			}
		}




		else if(strncmp(buffer,"DELETE ",7)==0)
		{
			printf("DELETE action\n");
			get_filename(filename,relpath,http_version,buffer,7);
			printf("%s\n",http_version);
			if(strcmp(http_version,"HTTP/1.1")==0)
			{
				FILE* tempptr=fopen(relpath,"r");
				if (tempptr != NULL)
				{			
					fclose(tempptr);
					get_file(filename,file);
					if(file[0]=='1' && file[1]=='1')
					{
						printf("Forbidden file\n");
						bzero(temper,sizeof(temper));
    			  		strcpy(temper,"HTTP/1.1 403\n"); 
	    	  			send(socknew,temper,sizeof(temper),0);
      					bzero(temper,sizeof(temper));
					}
					else
					{
						if(remove(relpath)==0)
						{
      						printf("Deleted %s successfully\n",filename); 
      						bzero(temper,sizeof(temper));
      						strcpy(temper,"HTTP/1.1 200 ok\n"); 
 	    	 				send(socknew,temper,sizeof(temper),0);
    	  					bzero(temper,sizeof(temper));
    	  				}
    	  				else
    	  				{
    	  					printf("//error deleting\n");
    	  				}
    	  			}
  			 	}
  		 		else
	  		 	{
    	  			printf("//Delete Unsuccessfull\n");
      				bzero(temper,sizeof(temper));
      				strcpy(temper,"HTTP/1.1 404\n"); 
      				send(socknew,temper,sizeof(temper),0);
    	  			bzero(temper,sizeof(temper));
  			 	}
  		 	}
  		 	else
  		 	{
  		 		printf("//BAD request\n");
  		 		bzero(temper,sizeof(temper));
      			strcpy(temper,"HTTP/1.1 400\n"); 
      			send(socknew,temper,sizeof(temper),0);
      			bzero(temper,sizeof(temper));

  		 	}
  


		} 
		else if(strcmp(buffer,"exit\n")==0)
		{
			printf("exit action\n");
			break;
		}

		else
		{
			printf("Enter valid method\n");
			bzero(temper,sizeof(temper));
      		strcpy(temper,"HTTP/1.1 405\n"); 
      		send(socknew,temper,sizeof(temper),0);
      		bzero(temper,sizeof(temper));



			
		}
	}
		
		
		
	}
	


	close(socknew);
	return ;
}

int main(){

	int sockid,len,socknew;
	struct sockaddr_in serveraddr,clientaddr;



	//socket creation
	if(!(sockid = socket(AF_INET,SOCK_STREAM,0))){
		printf("Socket Creation Failed!\n");
		exit(0);		
	}
	printf("Socket Creation Successful...\n");
	bzero(&serveraddr,sizeof(serveraddr));
	
	int opt =1;
	// Forcefully attaching socket to the port 8080 
    	if (setsockopt(sockid, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
   	 { 
        printf("setsockopt"); 
        exit(EXIT_FAILURE); 
   	 } 
	//setting the server address
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(PORT1);

	

	//binding port to the socket
	if(bind(sockid,(struct sockaddr *) &serveraddr,sizeof(serveraddr))!=0){
		printf("Socket Binding Failed!\n");
		exit(0);
	}
	printf("Binded to Socket Successfully...\n");
	


	//port listening
	if(listen(sockid,QUEUE_LIMIT)){
		printf("Listening Failed!\n");
		exit(0);
	}
	len= sizeof(clientaddr);
	printf("Server Listening...\n");


	//port accept connection
for(;;){	
	if(!(socknew=accept(sockid,(struct sockaddr *) &clientaddr,(socklen_t*)&len))!=0){
		printf("Server Accepting Failed!\n");
		exit(0);
	}

	signal(SIGPIPE,SIG_IGN);
	printf("Server Accepted the client...\n");
	//int c = fork();
	int childpid=-1;
	//printf("sksk");
	if((childpid =fork()) == 0)
	{
	close(sockid);	
	comm_server_to_client(socknew);	
	break;
	}
}
close(sockid);
return 0;
}

------------------------------------------------------------------------------------------------------------------------------------------------
//CLIENT
#include<cstdio>
#include<stdlib.h>
#include<unistd.h>
#include<cstring>

#include<time.h>
#include<sys/stat.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#define PORT1 20021
#define PORT2 20021
#define PORT3 20021


int file_transfer(char * filename, int sock_port1,FILE* fileptr)
{

	char buffer[100];
	bzero(buffer,sizeof(buffer));
	while(fgets(buffer,sizeof(buffer),fileptr)!=NULL)
	{
		send(sock_port1,buffer,sizeof(buffer),0);
		bzero(buffer,sizeof(buffer));		

	}	

	strcpy(buffer,"-1\n");
	send(sock_port1,buffer,sizeof(buffer),0);

	return 0;
}


void get_filename(char filename[],char relpath[],char http_version[], char buffer[],int j)
{
	int k=0;
	int i =0;
	relpath[0]='.';
	relpath[1]='/';
	for(i = 0 ; (buffer[j]!=' ' && buffer[j]!='\n') ;i++,j++)
	{
		filename[i] = buffer[j];
		relpath[i+2]=buffer[j];
	}
	if(buffer[j]==' ')
	{
		j++;
		for(k=0;buffer[j]!='\n';k++,j++)
		{
			http_version[k]=buffer[j];
		}
	}

	filename[i]='\0';
	relpath[i+2]='\0';
	http_version[k]='\0';

	return ;
}





void comm_clienttoserver(int sockid)
{
	char buffer[100];//={'\0'};
	char filename[100];
	char http_version[100];
	char relpath[100];
	char temp3[100];
	
	while(1)
	{
		bzero(buffer,sizeof(buffer));
		char *c= fgets(buffer,sizeof(buffer),stdin);
		
		printf("%s",buffer);
		send(sockid,buffer,sizeof(buffer),0);
			
			printf("sent\n");
		bzero(temp3,sizeof(temp3));
		recv(sockid, temp3,sizeof(temp3),0);
		if(strcmp(temp3,"NOT ok\n")==0)
		{
			bzero(temp3,sizeof(temp3));
			printf("G:Bad request/n");
			recv(sockid,temp3,sizeof(temp3),0);
			printf("%s\n",temp3 );

		}
		else
		{


		if(strncmp(buffer,"GET ",4)==0)
		{
			if(strcmp(buffer,"GET / HTTP/1.1\n")==0)
			{
				//default index.html
				bzero(buffer,sizeof(buffer));
				recv(sockid, buffer,sizeof(buffer),0);
				
				while(strcmp(buffer,"-1\n")!=0)
				{
					printf("%s",buffer);
					bzero(buffer,sizeof(buffer));
					recv(sockid,buffer,sizeof(buffer),0);
				}
				bzero(buffer,sizeof(buffer));
					
			}
			else
			{

				get_filename(filename,relpath,http_version,buffer,4);
					
				if(strcmp(http_version,"HTTP/1.1")==0)
				{
					printf("%s\n",filename);
					printf("%s\n",http_version);

					printf("relpath is %s\n",relpath);

					recv(sockid,buffer,sizeof(buffer),0);

					if(strcmp(buffer,"HTTP/1.1 403\n")==0)
					{
						printf(">Forbidden file\n");
						printf("%s",buffer);
						bzero(buffer,sizeof(buffer));
					}
					else if(strcmp(buffer,"HTTP/1.1 200 ok\n")==0)//strcmp(buffer,"conn 20020\n")==0)
					{

						printf("%s",buffer);
						bzero(buffer,sizeof(buffer));
						recv(sockid, buffer,sizeof(buffer),0);
				
						while(strcmp(buffer,"-1\n")!=0)
						{
							printf("%s",buffer);
							bzero(buffer,sizeof(buffer));
							recv(sockid,buffer,sizeof(buffer),0);
						}
						bzero(buffer,sizeof(buffer));
						//fclose(fileptr);
					}
					else if(strcmp(buffer,"HTTP/1.1 404\n")==0)//strcmp(buffer,"conn 20023\n")==0)
					{
						printf(">File not found\n");
						printf("%s",buffer);
						bzero(buffer,sizeof(buffer));
			
					}
					else
					{
						//
						printf("do some action\n");
					}
				}
				else
				{
					bzero(temp3,sizeof(temp3));
					printf("G:Bad request\n");
					recv(sockid,temp3,sizeof(temp3),0);
					printf("%s\n",temp3 );

				}


			}
			
			
		}
		else if(strncmp(buffer,"HEAD ",5)==0)
		{
				printf("head action\n");
				get_filename(filename,relpath,http_version,buffer,5);
				//bzero(buffer,sizeof(buffer));
					bzero(temp3,sizeof(temp3));
						
				recv(sockid,temp3,sizeof(temp3),0);

				if(strcmp(temp3,"HTTP/1.1 403\n")==0)
				{
					printf("file Forbidden\n");
					printf("%s",temp3);

				}
				else if(strcmp(temp3,"HTTP/1.1 404\n")==0)
				{
					printf("file not found\n");
					printf("%s",temp3);

				}
				else if(strcmp(temp3,"HTTP/1.1 400\n")==0)
				{
					//bzero(temp3,sizeof(temp3));
					printf("Bad request/n");
					//recv(sockid,temp3,sizeof(temp3),0);
					printf("%s\n",temp3 );

				}
				else if(strcmp(temp3,"HTTP/1.1 200 ok\n")==0)
				{
					printf("%s",temp3);
					bzero(temp3,sizeof(temp3));
					recv(sockid,temp3,sizeof(temp3),0);
					//printf("%s\n",temp3 );

					printf("Last modified time is %s\n",temp3);
					bzero(temp3,sizeof(temp3));
					
				}
				else
				{
					printf("%s",temp3);
					printf("hello\n");
				}
			//bzero(buf,sizeof(buffer));
			
			

		}
		else if(strncmp(buffer,"PUT ",4)==0)
		{
			//printf("PUT action\n");
			get_filename(filename,relpath,http_version,buffer,4);
			//printf("%s\n",http_version);
			if(strcmp(http_version,"HTTP/1.1")==0)
			{	
				//printf("%s and relpath is %s\n",filename,relpath);
				FILE* cliptr=fopen(relpath,"r");
				if(cliptr!=NULL)
				{
					bzero(temp3,sizeof(temp3));
					strcpy(temp3,"Ready Receive\n");
					//printf("%s",temp3);
					send(sockid,temp3,sizeof(temp3),0);

					int fileval = file_transfer(filename,sockid,cliptr);
					bzero(buffer,sizeof(buffer));
					//close(sock_port1);
					fclose(cliptr);			
				
				}
				else
				{
					bzero(temp3,sizeof(temp3));
					strcpy(temp3,"Not REady\n");
					//printf("%s",temp3);
					send(sockid,temp3,sizeof(temp3),0);


					printf("//File not present\n");
					//fclose(cliptr);
				}
			}
			else
			{
					bzero(temp3,sizeof(temp3));
					printf("//G:Bad request\n");
					recv(sockid,temp3,sizeof(temp3),0);
					printf("%s\n",temp3 );

			}




		}
		else if(strncmp(buffer,"DELETE ",7)==0)
		{
			printf("DELETE action\n");
			bzero(temp3,sizeof(temp3));
			recv(sockid,temp3,sizeof(temp3),0);
			if(strcmp(temp3,"HTTP/1.1 200 ok\n")==0)
			{
				printf("//Deleted successfully\n");
				printf("%s\n",temp3);
			}
			else if(strcmp(temp3,"HTTP/1.1 404\n")==0)
			{
				printf("//Delete unsuccessfull\n");
				printf("%s\n",temp3);	
			}
			else if(strcmp(temp3,"HTTP/1.1 400\n")==0)
			{
				printf("D:Bad request\n");
				printf("%s\n",temp3 );
			}
			else if(strcmp(temp3,"HTTP/1.1 403\n")==0)
			{
				printf("D:Forbidden\n");
				printf("%s\n",temp3 );
			}
			else
			{
				printf("hoo-hoo\n");
			}

		} 
		else if(strcmp(buffer,"exit\n")==0)
		{
			
			printf("exit action\n");
			break;
		}

		else
		{
			printf("Enter valid method\n");
			bzero(temp3,sizeof(temp3));
			recv(sockid,temp3,sizeof(temp3),0);
			printf("%s\n",temp3);

		}
	}

	}
		
	
	return;
}

int main(){
	int sockid,socknew;
	struct sockaddr_in serveraddr,clientaddr;


	//socket creation
	if(!(sockid=socket(AF_INET,SOCK_STREAM,0))){
		printf("Socket Creation Failed!\n");
		exit(0);
	}
	printf("Socket Creation Successful...\n");
	bzero(&serveraddr,sizeof(serveraddr));



	//server address
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port=htons(PORT1);



	//connection to the server
	if(connect(sockid,(struct sockaddr *)&serveraddr , sizeof(serveraddr))){
		printf("Connection to Server Failed!\n");
		exit(0);
	}
	printf("Connection to Server Successful...\n");
	//communication from client to server
	comm_clienttoserver(sockid);
	
	close(sockid);
return 0;
}



