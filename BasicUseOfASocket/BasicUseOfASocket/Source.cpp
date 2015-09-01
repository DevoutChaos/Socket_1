#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <errno.h>


using namespace std;

int main(int argc, char *argv[])
{
	int sock;
	short count;
	short len = 0;
	char buf[1024];

	struct sockaddr_in sockaddr;
	memset(&sockaddr, 0, sizeof(sockaddr));
	sock = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(21); //Host to Network Short
	sockaddr.sin_addr.s_addr = inet_addr("130.179.16.34"); //IP
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (connect(sock, (struct sockaddr*) &sockaddr, sizeof(sockaddr)) == -1)
	{
		cout << "Connection Failed. FML." << "\n";
		cout << "error: " << strerror(errno) << std::endl;
		if (err != 0) {
			/* Tell the user that we could not find a usable */
			/* Winsock DLL.                                  */
			printf("WSAStartup failed with error: %d\n", err);
			return 1;
		}
	}
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* Winsock DLL.                                  */
		printf("WSAStartup failed with error: %d\n", err);
		return 1;
	}

	count = recv(sock, buf, len, 0);
	cout << "Result/count = " << count;
	count = send(sock, buf, len, 0);
	
	cin.get();

	return 0;
}