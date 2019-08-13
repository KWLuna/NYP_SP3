#pragma once
#include <iostream>
#include <string>

#pragma comment(lib, "Ws2_32.lib")
#include <Windows.h> // forgot not sure yet
#include <winsock.h> // for networking 
#define MAXLEN   64      

struct IPv4
{
	unsigned char b1, b2, b3, b4;
};

class CNetworking
{
public:
	CNetworking();
	~CNetworking();

	void TalktoServer(std::string input);
	void ListenForPackets();
	void getIPv4();

	WSADATA WinSockData;
	int		iWsaStartup;
	int		iWsaCleanup;

	SOCKET	UDPSocketClient;

	struct sockaddr_in UDPServer;
	int iBind;
	char Buffer[5000000] = "Hello From Client!";
	int iSendto = 0;
	int iReceiveFrom = 0;

	int iBufferLen = strlen(Buffer) + 1;
	int iUDPServerLen = sizeof(UDPServer);
	int iCloseSocket;


	/// 
	WSADATA  wsadata;
	char    hostname[MAXLEN];
	int    ret;
	std::string ipaddressoutput;

	
};

