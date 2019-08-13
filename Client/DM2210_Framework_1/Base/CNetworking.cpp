#include "CNetworking.h"



// client

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
	UDPServer.sin_addr.s_addr = inet_addr("127.0.0.1");// the other computer ip(computer)
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
		UDPServer.sin_addr.s_addr = inet_addr("172.27.184.100");// your own ip (laptop)
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

		std::string user_info = Buffer;
		playerPosX = user_info.substr(0, user_info.find(",")); // get the x pos
		playerPosZ = user_info.substr(user_info.find(",") + 1, user_info.size()); // get the z pos

		std::cout << playerPosX << std::endl;
		std::cout << playerPosZ << std::endl;

		
		iCloseSocket = closesocket(UDPSocketClient);
		iWsaCleanup = WSACleanup();
	}
}
