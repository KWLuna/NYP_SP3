#pragma once
#include <iostream>
#include <string>
#include <vector>

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

	void TalktoServer(std::string IPv4, std::string sms);

	void GetServerIPStart(); // just to run ones
	std::string IPtoASCII();
	void GetServerIP();
	std::string getIPv4();

	WSADATA WinSockData;
	int		iWsaStartup;
	int		iWsaCleanup;

	SOCKET	UDPSocketClient;

	// max buffer size is 19 

	struct sockaddr_in UDPServer;
	int iBind;
	char Buffer[1024] = "Hello From Client!";
	int iSendto = 0;
	int iReceiveFrom = 0;

	int iBufferLen = strlen(Buffer) + 1;
	int iUDPServerLen = sizeof(UDPServer);
	int iCloseSocket;


	/// 
	WSADATA  wsadata;
	char    hostname[MAXLEN];
	int    ret;

	// Networking 
	std::string ServerIPv4 = "BLANK";

	

	
};

