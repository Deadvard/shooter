#ifndef NET_H
#define NET_H

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef unsigned char int8;
typedef unsigned short int16;
typedef unsigned int int32;
typedef float float32;
typedef unsigned int bool32;

struct Address
{
	uint32 ip;
	uint16 port;
};

bool32 network_startup();
bool32 network_cleanup();

int network_socket_create(uint16 port);
void network_socket_destroy(int socket);

int network_send(int socket, struct Address* to, void *data, int length);
int network_recieve(int socket, struct Address* from, void *data, int length);

#endif
