#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
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
	short count;
	short len = 0;
	char buf[1024];
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

	//cout << i << "\n";

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
	count = recv(sock, buf, len, 0);
	
	cout << "Connection complete. \n" << "Press any key to continue.";
	cin.get();

	count = send(sock, buf, len, 0);
	
	return 0;
}