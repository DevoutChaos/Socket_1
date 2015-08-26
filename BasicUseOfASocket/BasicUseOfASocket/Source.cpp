/*****
Brief Implementation of FTP Client
******/
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>


int main(int argc, char *argv[])
{
	int sock;
	short count;
	short len = 0;
	char buf[1024];

	struct sockaddr_in sockaddr;

	memset(&sockaddr, 0, sizeof(sockaddr));
	sock = socket(AF_INET, SOCK_STREAM, 0);
	//bzero(&sockaddr, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(21); //Host to Network Short
	sockaddr.sin_addr.s_addr = inet_addr("134.139.48.252"); //IP
	if (connect(sock, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) == -1)
	{
		//Handle Error
	}

	count = recv(sock, buf, len, 0);

	count = send(sock, buf, len, 0);

	return 0;
}