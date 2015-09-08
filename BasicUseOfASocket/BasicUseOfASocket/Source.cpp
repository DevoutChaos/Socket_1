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

int main(int argc, char *argv[])
{
	//Initialisation
	int i;
	int sock;
	int sock2;
	short count;
	short len = 0;
	char buf[2048]; //Indicates the size of the buffer
	struct sockaddr_in sockaddr;

	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* Winsock DLL.                                  */
		printf("WSAStartup failed with error: %d\n", err);
		return 1;

	}

	memset(&sockaddr, 0, sizeof(sockaddr));
	sock = socket(AF_INET, SOCK_STREAM, 0); //Create socket. SOCK_STREAM is a type of socket, AF_INET is IPv4 protocols
	sock2 = socket(AF_INET, SOCK_STREAM, 0); //Create socket. SOCK_STREAM is a type of socket, AF_INET is IPv4 protocols
	sockaddr.sin_family = AF_INET; // AF_INET: address family that is used for the socket you're creating
	sockaddr.sin_port = htons(21); //Host to Network Short
	sockaddr.sin_addr.s_addr = inet_addr("130.179.16.34"); //IP of ftp.csulb.edu

	if (sock == INVALID_SOCKET)
	{
		printf("Failed, Invalid Socket. \n");
		WSACleanup();
		cout << "Socket is " << sock << "\n";
		cout << "This is a problem";
		cin.get();
		return 1;
	}

	i = connect(sock, (struct sockaddr*) &sockaddr, sizeof(sockaddr));

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
	else
	{
		cout << "Please Wait. Establishing connection... \n";
	}
	
	//Expecting to reveive a value of 220
	do
	{
		count = recv(sock, buf, sizeof(buf), 0);
		if (count > 0)
		{
			buf[count] = 0;
			cout << "Response: " << buf << "\n";
		}
	} while (count == (sizeof(buf)));

	cout << "Connection complete. \n";

	//Sets the Buffer to the value for Username, then sends it
	sprintf(buf, "USER anonymous\r\n");
	cout << "Sending Username 'anonymous'. \n" << "Please wait. \n";
	count = send(sock, buf, strlen(buf), 0);

	cout << "Sent\n";

	//Expecting to receive a value of 331
	do
	{
		count = recv(sock, buf, sizeof(buf), 0);
		buf[count] = 0;
		if (count > 0)
		{
			buf[count] = 0;
			cout << "Response: " << buf << "\n";
		}
	} while (count == (sizeof(buf)));

	sprintf(buf, "PASS nope@gmail.com\r\n");
	cout << "Sending Password: nope@gmail.com \n";
	count = send(sock, buf, strlen(buf), 0);

	//Expecting to receive a value of 230
	do
	{
		count = recv(sock, buf, sizeof(buf), 0);
		if (count > 0)
		{
			buf[count] = 0;
			cout << "Response: " << buf << "\n";
		}
	} while (count == (sizeof(buf)));

	//Setting mode to passive
	sprintf(buf, "PASV\r\n");
	cout << "Sending Passive \n";
	count = send(sock, buf, strlen(buf), 0);

	//Expecting to receive a value of 230
	do
	{
		count = recv(sock, buf, sizeof(buf), 0);
		if (count > 0)
		{
			buf[count] = 0;
			cout << "Response: " << buf << "\n";
		}
	} while (count == (sizeof(buf)));

	//Confirms passive mode
	do {
		count = recv(sock, buf, sizeof(buf), 0);
		if (count > 0)
		{
			buf[count] = 0;
			cout << "Response: " << buf << "\n";
		}
	} while (count == (sizeof(buf)));

	//Connect using the second socket
	//sockaddr.sin_addr.s_addr = inet_addr("130.179.16.34"); // IP

	
	if (connect(sock2, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) == -1)
	{
		cout << "error";//handle error
	}
	printf("Establishing connection...\n");
	do {
		count = recv(sock2, buf, sizeof(buf), 0);
		if (count > 0)
		{
			buf[count] = 0;
			cout << "Response: " << buf << "\n";
		}
	} while (count == (sizeof(buf)));
	

	//Call list using the first socket
	cout << "Requesting list \n";
	sprintf(buf, "LIST\r\n");
	count = send(sock, buf, strlen(buf), 0);
	cout << "Request sent \n";
	//Expect a 150 from sock1
	do {
		cout << "do";
		count = recv(sock, buf, sizeof(buf), 0);
		cout << "rec";
		if (count > 0)
		{
			cout << "if";
			buf[count] = 0;
			cout << "Response: " << buf << "\n";
		}
	} while (count == (sizeof(buf)));

	//Expect LIST from sock2
	do {
		count = recv(sock2, buf, sizeof(buf), 0);
		if (count > 0)
		{
			buf[count] = 0;
			cout << "Response: " << buf << "\n";
		}
	} while (count == (sizeof(buf)));

	cin.get();
	return 0;
}