#pragma once
#include <iostream>
#include <string>

#include <Windows.h>
#include <winsock.h> // networking lib
#pragma comment(lib, "Ws2_32.lib")

#include <thread> // threading for the listenr in the networking 

#define MAXLEN   64      



struct IPv4
{
	unsigned char b1, b2, b3, b4;
};


class Networking
{
public:
	Networking();
	~Networking();

	void SendPackets(std::string);
	void listener();
	void FLOOD(std::string,std::string);
	std::string getIPv4();
private:

	WSADATA WinSockData;
	int		iWsaStartup;
	int		iWsaCleanup;

	SOCKET	UDPSocketClient;

	struct sockaddr_in UDPServer;
	int iBind;
	char Buffer[50000] = "Hello From Server!";
	int iSendto = 0;
	int iReceiveFrom = 0;

	int iBufferLen = strlen(Buffer) + 1;
	int iUDPServerLen = sizeof(UDPServer);
	int iCloseSocket;

	// Get IPv4
	WSADATA  wsadata;
	char    hostname[MAXLEN];
	int    ret;
	std::string ipaddressoutput;
};

