/*****
Brief Implementation of FTP Client
******/
//#define SOCK_STREAM
#include <winsock.h>


int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in sockaddr;

	memset(&sockaddr, 0, sizeof(sockaddr));
	sock = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&sockaddr, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(21); //Host to Network Short
	sockaddr.sin_addr.s_addr = inet_addr("..."); //IP
	if (connect(sock, (struct sockaddr *)&sockaddr, sizeof(sockaddr)))
	{
		//Handle Error
	}

	byte_count = recv(sock, buf, len, 0);

	byte_count = send(sock, buf, len, 0);

	return 0;
}