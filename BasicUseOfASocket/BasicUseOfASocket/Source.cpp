#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#ifndef WINSOCK_VERSION
#define WINSOCK_VERSION MAKEWORD(2,2)
#endif
#pragma comment(lib, "ws2_32.lib")
#include <WS2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <winsock2.h>
#include <iostream>
#include <errno.h>
WSADATA wsaData;
using namespace std;

void doTheDoThing(int sock);

int main(int argc, char *argv[])
{
	//Initialisation
	//The address we get from server
	int address[6];
	//sock1
	int i;
	//sock2
	int f;
	//The sockets themselves
	int sock;
	int sock2;
	//Parts for ports
	int x;
	int y;
	int port;
	int x2;
	int y2;
	int port2;
	//Used to send/receive data
	short count;
	short len = 0;
	//Indicates the size of the buffer
	char buf[4096]; 
	//Thing needed for socket creation
	struct sockaddr_in sockaddr;

	//Not sure but seems vital
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* Winsock DLL.                                  */
		printf("WSAStartup failed with error: %d\n", err);
		return 1;
	}

	//
	memset(&sockaddr, 0, sizeof(sockaddr));
	sock = socket(AF_INET, SOCK_STREAM, 0); //Create socket. SOCK_STREAM is a type of socket, AF_INET is IPv4 protocols
	sockaddr.sin_family = AF_INET; // AF_INET: address family that is used for the socket you're creating
	sockaddr.sin_port = htons(21); //Host to Network Short
	sockaddr.sin_addr.s_addr = inet_addr("130.179.16.34"); //IP of ftp.csulb.edu

	//Error handling
	if (sock == INVALID_SOCKET)
	{
		cout << "Failed, Invalid Socket. \n";
		WSACleanup();
		cout << "Socket is " << sock << "\n";
		cout << "This is a problem";
		cin.get();
		return 1;
	}

	//Declares the socket as i
	i = connect(sock, (struct sockaddr*) &sockaddr, sizeof(sockaddr));

	//Error handling for socket, i
	if (i == SOCKET_ERROR)
	{
		cout << "Connection Failed. FML." << "\n";
		cout << "Error: " << strerror(errno) << std::endl;
		if (i == SOCKET_ERROR)
		{
			cout << "Fail to close!";
		}
		WSACleanup();
		return 1;
	}
	//What we want to see
	else
	{
		cout << "Please Wait. Establishing connection... \n";
	}
	
	//Expecting to reveive a value of 220
	doTheDoThing(sock);

	//To show that we are moving as we should be
	cout << "Connection complete. \n";

	//Sets the Buffer to the value for Username, then sends it
	sprintf(buf, "USER anonymous\r\n");
	cout << "Sending Username 'anonymous'. \n" << "Please wait. \n";
	count = send(sock, buf, strlen(buf), 0);

	cout << "Sent\n";

	//Expecting to receive a value of 331
	doTheDoThing(sock);

	//Here we set our password, it doesn't really matter as long as it looks like an E-Mail address
	sprintf(buf, "PASS nope@gmail.com\r\n");
	cout << "Sending Password: nope@gmail.com \n";
	count = send(sock, buf, strlen(buf), 0);

	//Expecting to receive a value of 230
	doTheDoThing(sock);

	//Setting mode to passive
	sprintf(buf, "PASV\r\n");
	cout << "Sending Passive \n";
	count = send(sock, buf, strlen(buf), 0);

	//Expecting to receive a value of 230
	doTheDoThing(sock);
	//Here we get the vital port number
	doTheDoThing(sock);

	//parse values from returned IP and port number
	char * pch;
	pch = strtok(buf, ",()");
	pch = strtok(NULL, ",()");
	for (i = 0; i<6; i++)
	{
		address[i] = atoi(strtok(NULL, ",()"));
	}
	//output values given for port no.
	printf("%d\n", address[3]);
	printf("%d\n", address[4]);
	///calculate port
	//use x *256 + y for port
	x = address[3];
	y = address[4];
	port = x * 256 + y;
	//output final port number
	cout << "Port is: " << port << "\n";

	//Connect using the second socket
	memset(&sockaddr, 0, sizeof(sockaddr));
	sock2 = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port); // Host to network short
	sockaddr.sin_addr.s_addr = inet_addr("130.179.16.34"); // IP
	
	//Error handling again, for sock2 this time
	if (sock2 == INVALID_SOCKET)
	{
		cout << "Failed, Invalid Socket. \n";
		WSACleanup();
		cout << "Socket is " << sock2 << "\n";
		cout << "This is a problem";
		cin.get();
		return 1;
	}

	//Set sock2 to be f
	f = connect(sock2, (struct sockaddr*) &sockaddr, sizeof(sockaddr));

	//Checks for errors in f
	if (f == SOCKET_ERROR)
	{
		cout << "Connection Failed. FML." << "\n";
		cout << "Error: " << strerror(errno) << std::endl;
		if (f == SOCKET_ERROR)
		{
			cout << "Fail to close!";
		}
		WSACleanup();
		return 1;
	}
	//What we want to see (De ja vu?)
	else
	{
		cout << "Please Wait. Establishing connection 2... \n";
	}

	//Call list using the first socket
	cout << "Requesting list \n";
	sprintf(buf, "LIST\r\n");
	count = send(sock, buf, strlen(buf), 0);
	cout << "Request sent \n";
	//Expect a 150 from sock
	doTheDoThing(sock);

	//Expect LIST from sock2
	do {
		count = recv(sock2, buf, sizeof(buf), 0);
		if (count > 0)
		{
			buf[count] = 0;
			cout << "Response: " << buf << "\n";
		}
	} while (count == (sizeof(buf)));

	//Close scoket No.2
	closesocket(sock2);

	//Expect 226
	doTheDoThing(sock);

	//Do all of this again for RETR

	//Checks for errors in f
	if (f == SOCKET_ERROR)
	{
		cout << "Connection Failed. FML." << "\n";
		cout << "Error: " << strerror(errno) << std::endl;
		if (f == SOCKET_ERROR)
		{
			cout << "Fail to close!";
		}
		WSACleanup();
		return 1;
	}
	//What we want to see (De ja vu?)
	else
	{
		cout << "Please Wait. Establishing connection 2... \n";
	}

	//Setting mode to passive
	sprintf(buf, "PASV\r\n");
	cout << "Sending Passive \n";
	count = send(sock, buf, strlen(buf), 0);

	//Expecting to receive a value of 230
	doTheDoThing(sock);

	//parse values from returned IP and port number
	pch = strtok(buf, ",()");
	pch = strtok(NULL, ",()");
	for (i = 0; i<6; i++)
	{
		address[i] = atoi(strtok(NULL, ",()"));

	}
	//output values given for port no.
	cout << "\n" << address[3];
	cout << "\n" << address[4];
	///calculate port
	//use x *256 + y for port
	x2 = address[3];
	y2 = address[4];
	port2 = x2 * 256 + y2;
	//output final port number
	cout << "Port is: " << port2 << "\n";

	//Connect using the second socket
	memset(&sockaddr, 0, sizeof(sockaddr));
	sock2 = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port2); // Host to network short
	sockaddr.sin_addr.s_addr = inet_addr("130.179.16.34"); // IP

	//Error handling again, for sock2 this time
	if (sock2 == INVALID_SOCKET)
	{
		cout << "Failed, Invalid Socket. \n";
		WSACleanup();
		cout << "Socket is " << sock2 << "\n";
		cout << "This is a problem";
		cin.get();
		return 1;
	}

	//Set sock2 to be f
	f = connect(sock2, (struct sockaddr*) &sockaddr, sizeof(sockaddr));

	//Checks for errors in f
	if (f == SOCKET_ERROR)
	{
		cout << "Connection Failed. FML." << "\n";
		cout << "Error: " << strerror(errno) << std::endl;
		if (f == SOCKET_ERROR)
		{
			cout << "Fail to close!";
		}
		WSACleanup();
		return 1;
	}
	//What we want to see (De ja vu?)
	else
	{
		cout << "Please Wait. Establishing connection 2... \n";
	}

	//Call retr using the first socket
	cout << "Requesting file \n";
	sprintf(buf, "RETR NOTICE\r\n");
	count = send(sock, buf, strlen(buf), 0);
	cout << "Request sent \n";
	//Expect a 150 from sock
	doTheDoThing(sock);

	//Expect File from sock2
	do {
		count = recv(sock2, buf, sizeof(buf), 0);
		if (count > 0)
		{
			buf[count] = 0;
			cout << "Response: " << buf << "\n";
		}
	} while (count == (sizeof(buf)));

	//Close scoket No.2
	closesocket(sock2);

	//Expect 226
	doTheDoThing(sock);

	sprintf(buf, "QUIT\r\n");
	cout << "Closing connection \n";
	count = send(sock, buf, strlen(buf), 0);
	//Expect 221
	doTheDoThing(sock);

	//Wait until user confirms before closing
	cin.get();
	return 0;
}

void doTheDoThing(int sock)
{
	short count;
	char buf[2048];
	do
	{
		count = recv(sock, buf, sizeof(buf), 0);
		if (count > 0)
		{
			buf[count] = 0;
			cout << "Response: " << buf << "\n";
		}
	} while (count == (sizeof(buf)));
}