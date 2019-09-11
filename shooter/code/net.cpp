#include "net.h"
#include <winsock2.h>

bool32 network_startup()
{
	WSADATA wsaData;
	return WSAStartup(MAKEWORD(2, 2), &wsaData) == NO_ERROR;
}

bool32 network_cleanup()
{
	return WSACleanup() == NO_ERROR;
}

int network_socket_create(uint16 port)
{
	int result = 0;

	result = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (result > 0)
	{
		struct sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(port);

		if (bind(result,
			(struct sockaddr *)&address, sizeof(address)) == 0)
		{
			unsigned long nonBlocking = 1;
			if (ioctlsocket(result, FIONBIO, &nonBlocking) == 0)
			{
				return result;
			}
		}
	}

	return 0;
}

void network_socket_destroy(int socket)
{
	if (socket)
	{
		closesocket(socket);
	}
}

int network_send(int socket, struct Address* to, void *data, int length)
{
	struct sockaddr_in address = {};
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(to->ip);
	address.sin_port = htons(to->port);

	int result = sendto(socket, (const char*)data, length,
		0, (struct sockaddr *)&address, sizeof(address));

	return result;
}

int network_recieve(int socket, struct Address* from, void *data, int length)
{
	struct sockaddr_in address = {};
	int addressSize = sizeof(address);
	address.sin_family = AF_INET;

	int result = recvfrom(socket, (char*)data, length, 0, (struct sockaddr *)&address, &addressSize);
	if (result > 0)
	{
		from->ip = ntohl(address.sin_addr.s_addr);
		from->port = ntohs(address.sin_port);
	}

	return result;
}