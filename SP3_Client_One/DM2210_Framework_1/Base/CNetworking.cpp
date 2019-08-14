#include "CNetworking.h"

// server

CNetworking::CNetworking()
{
}


CNetworking::~CNetworking()
{
}

void CNetworking::TalktoServer(std::string input)
{

	iWsaStartup = WSAStartup(MAKEWORD(2, 2), &WinSockData); // NEW
	UDPServer.sin_family = AF_INET;// NEW
	UDPServer.sin_addr.s_addr = inet_addr("172.27.184.100");// the other computer ip(computer)
	UDPServer.sin_port = htons(8001);// NEW
	UDPSocketClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);



	strcpy(Buffer, input.c_str());	// or pass &s[0]

	iSendto = sendto(
		UDPSocketClient,
		Buffer,
		iBufferLen,
		MSG_DONTROUTE,
		(SOCKADDR*)&UDPServer,
		sizeof(UDPServer)
	);


	iCloseSocket = closesocket(UDPSocketClient);
	iWsaCleanup = WSACleanup();
}

void CNetworking::ListenForPackets()
{
	//std::cout << "Start Run for ListsenForPacakets" << std::endl;

	while (true)
	{
		iWsaStartup = WSAStartup(MAKEWORD(2, 2), &WinSockData); // NEW
		UDPServer.sin_family = AF_INET;// NEW
		UDPServer.sin_addr.s_addr = inet_addr("127.0.0.1");// your own ip (laptop)
		UDPServer.sin_port = htons(8001);// NEW
		UDPSocketClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);


		iBind = bind(
			UDPSocketClient,
			(SOCKADDR*)&UDPServer,
			sizeof(UDPServer)
		);

		iReceiveFrom = recvfrom(
			UDPSocketClient,
			Buffer,
			iBufferLen,
			MSG_PEEK,
			(SOCKADDR*)&UDPServer,
			&iUDPServerLen
		);

		if (iReceiveFrom == SOCKET_ERROR)
		{
			return;
		}
		//std::cout << "Receve the message : " << Buffer << std::endl;

		//std::string user_info = Buffer;
		///*
		//
		//	Ones u get the information do what u need to do with it here 

		//*/


		//std::cout << "Recive Message " << user_info << std::endl;
		iCloseSocket = closesocket(UDPSocketClient);
		iWsaCleanup = WSACleanup();
	}
}







std::string CNetworking::getIPv4()
{
	// Init WinSock
	WSADATA wsa_Data;
	int wsa_ReturnCode = WSAStartup(0x101, &wsa_Data);

	// Get the local hostname
	char szHostName[255];
	gethostname(szHostName, 255);
	struct hostent *host_entry;
	host_entry = gethostbyname(szHostName);
	char * szLocalIP;
	szLocalIP = inet_ntoa(*(struct in_addr *)*host_entry->h_addr_list);
	std::cout << szLocalIP << std::endl;
	WSACleanup();
	return szLocalIP;

}

void CNetworking::GetServerIP()
{
	while (true)
	{
		std::cout << "Waiiting For Message from Server.........." << std::endl;
		iWsaStartup = WSAStartup(MAKEWORD(2, 2), &WinSockData); // NEW
		UDPServer.sin_family = AF_INET;// NEW
		UDPServer.sin_addr.s_addr = inet_addr(CNetworking::getIPv4().c_str());// your own ip (laptop)
		UDPServer.sin_port = htons(8001);// NEW
		UDPSocketClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);


		iBind = bind(
			UDPSocketClient,
			(SOCKADDR*)&UDPServer,
			sizeof(UDPServer)
		);

		iReceiveFrom = recvfrom(
			UDPSocketClient,
			Buffer,
			iBufferLen,
			MSG_PEEK,
			(SOCKADDR*)&UDPServer,
			&iUDPServerLen
		);

		if (iReceiveFrom == SOCKET_ERROR)
		{
			return;
		}

		iCloseSocket = closesocket(UDPSocketClient);
		iWsaCleanup = WSACleanup();

		std::cout << "Message from Sever:" << Buffer << std::endl;
	}
	
	
}
