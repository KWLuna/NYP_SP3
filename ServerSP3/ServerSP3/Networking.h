#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <winsock.h> // networking lib
#pragma comment(lib, "Ws2_32.lib")

#include <thread> // threading for the listenr in the networking 
#define MAXLEN   64      




class Networking
{
public:
	Networking();
	~Networking();
	void SendPackClient(std::string,std::string);
	void listener();
	std::string getIPv4();
	std::vector<std::string> allClientIPv4;
	bool run;

private:
	int packet_ID = 0;

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

	// vector to store all incoming IPv4 from client
	bool inVector;
	std::vector<int> dotpos; // to use to check if the packet sent is a ip address packet only
	int foundtheOne;

	int oldLimitC1 = 0;
	int oldlimitC2 = 0;

	int findAnd;
	int NewPacketLimit;
};

