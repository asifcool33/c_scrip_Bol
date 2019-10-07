#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<time.h>

//Create a Socket for server communication
short SocketCreate(void)
{

        short hSocket;
        printf("Create the socket\n");
        hSocket = socket(AF_INET, SOCK_STREAM, 0);
        return hSocket;
}

//try to connect with server
int SocketConnect(int hSocket)
{

        int iRetval=-1;
        int ServerPort = 80;
        struct sockaddr_in remote={0};

        remote.sin_addr.s_addr = inet_addr("127.0.0.1"); //Local Host
        remote.sin_family = AF_INET;
        remote.sin_port = htons(ServerPort);

        iRetval = connect(hSocket , (struct sockaddr *)&remote , sizeof(struct sockaddr_in));


        return iRetval;
}


// Send the data to the server and set the timeout of 20 seconds
int SocketSend(int hSocket,char* Rqst,short lenRqst)

{

        int shortRetval = -1;
        struct timeval tv;
        tv.tv_sec = 10;  /* 20 Secs Timeout */
        tv.tv_usec = 0;  

        if(setsockopt(hSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv,sizeof(tv)) < 0)
        {
          printf("Time Out\n");
          return -1;
        }
        shortRetval = send(hSocket , Rqst , lenRqst , 0);

        return shortRetval;
 }



 
 //main driver program
int main(int argc , char *argv[])
{
	int hSocket, read_size;
	struct sockaddr_in server;
	//char SendToServer[100] = {"GET /"};
	char server_reply[200] = {0};

		char generated_request[1000];
		int number;
	
	srand(time(0));

char *acceptall[] = {
		"GET / \r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\n\r\n\r\n",
		"GET / \r\nAccept-Encoding: gzip, deflate\r\n\r\n\r\n",
		"GET / \r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\n\r\n\r\n",
		"GET / \r\nAccept: text/html, application/xhtml+xml, application/xml;q=0.9, */*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Charset: iso-8859-1\r\nAccept-Encoding: gzip\r\n\r\n\r\n",
		"GET /ksjdl?=123 HTTP/1.0\r\nAccept: application/xml,application/xhtml+xml,text/html;q=0.9, text/plain;q=0.8,image/png,*/*;q=0.5\r\nAccept-Charset: iso-8859-1\r\n\r\n\r\n",
		"GET /About HTTP/1.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Encoding: br;q=1.0, gzip;q=0.8, *;q=0.1\r\nAccept-Language: utf-8, iso-8859-1;q=0.5, *;q=0.1\r\nAccept-Charset: utf-8, iso-8859-1;q=0.5\r\n\r\n\r\n",
		"GET /theams HTTP/1.0\r\nAccept: image/jpeg, application/x-ms-application, image/gif, application/xaml+xml, image/pjpeg, application/x-ms-xbap, application/x-shockwave-flash, application/msword, */*\r\nAccept-Language: en-US,en;q=0.5\r\n\r\n\r\n",
		"GET /thenos HTTP/1.0\r\nAccept: text/html, application/xhtml+xml, image/jxr, */*\r\nAccept-Encoding: gzip\r\nAccept-Charset: utf-8, iso-8859-1;q=0.5\r\nAccept-Language: utf-8, iso-8859-1;q=0.5, *;q=0.1\r\n\r\n\r\n",
		"POST /parameter=value&also=another HTTP/1.0\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/webp, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Encoding: gzip\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Charset: utf-8, iso-8859-1;q=0.5\r\n\r\n\r\n",
		"GET /182aqo HTTP/1.0\r\nAccept: text/html, application/xhtml+xml, application/xml;q=0.9, */*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\n\r\n\r\n",
		"GET /thbaqo HTTP/1.0\r\nAccept-Charset: utf-8, iso-8859-1;q=0.5\r\nAccept-Language: utf-8, iso-8859-1;q=0.5, *;q=0.1\r\n\r\n\r\n",
		"GET /tkajsklq HTTP/1.0\r\nAccept: text/html, application/xhtml+xml\r\n\r\n\r\n",
		"GET /kajsdkjl/jsjk HTTP/1.0\r\nAccept-Language: en-US,en;q=0.5\r\n\r\n\r\n",
		"GET /hjhsdkafe HTTP/1.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Encoding: br;q=1.0, gzip;q=0.8, *;q=0.1\r\n\r\n\r\n",
		"GET /jaklsdi HTTP/1.0\r\nAccept: text/plain;q=0.8,image/png,*/*;q=0.5\r\nAccept-Charset: iso-8859-1\r\n\r\n\r\n"};





while(1){

	number = rand()%15;
//printf("%d",number);

	strcpy(generated_request, acceptall[number]);

	//Create socket
	hSocket = SocketCreate();
	if(hSocket == -1)
	{
	  printf("Could not create socket\n");
	  return 1;
	}

	

	//Connect to remote server
	if (SocketConnect(hSocket) < 0)
	{
		perror("connect failed.\n");
		return 1;
	}

	printf("Sucessfully conected with server\n");

	printf("Enter the Message: ");
	

	//Send data to the server
	SocketSend(hSocket , generated_request, strlen(generated_request));


	//close(hSocket);
	//shutdown(hSocket,0);
	//shutdown(hSocket,1);
	//shutdown(hSocket,2);
    //return 0;
}
}
