#include "Networking.h"



Networking::Networking()
{
}


Networking::~Networking()
{
}

void Networking::SendPackets(std::string input)
{
	iWsaStartup = WSAStartup(MAKEWORD(2, 2), &WinSockData); // NEW
	UDPServer.sin_family = AF_INET;// NEW
	UDPServer.sin_addr.s_addr = inet_addr("127.0.0.1");// the other computer ip(computer)
	UDPServer.sin_port = htons(8001);// NEW
	UDPSocketClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);



	strcpy_s(Buffer, input.c_str());	// or pass &s[0]

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

void Networking::listener()
{
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
		
		iCloseSocket = closesocket(UDPSocketClient);
		iWsaCleanup = WSACleanup();
	}
}

void Networking::SPAM(std::string IPv4 ,std::string input)
{
	
		iWsaStartup = WSAStartup(MAKEWORD(2, 2), &WinSockData); // NEW
		UDPServer.sin_family = AF_INET;// NEW
		UDPServer.sin_addr.s_addr = inet_addr(IPv4.c_str());// the other computer ip(computer)
		UDPServer.sin_port = htons(8001);// NEW
		UDPSocketClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);



		strcpy_s(Buffer, input.c_str());	// or pass &s[0]

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
		

		std::cout << "Address : " << IPv4 << "Message : " << input << std::endl;
	
}


std::string Networking::getIPv4()
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
	//std::cout << szLocalIP << std::endl;
	WSACleanup();

	return szLocalIP;

}
