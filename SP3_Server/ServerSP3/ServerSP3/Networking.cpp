#include "Networking.h"



Networking::Networking()
{
}


Networking::~Networking()
{
}



void Networking::listener()
{
	while (true)
	{
		std::cout << "listing now" << std::endl;
		iWsaStartup = WSAStartup(MAKEWORD(2, 2), &WinSockData); // NEW
		UDPServer.sin_family = AF_INET;// NEW
		UDPServer.sin_addr.s_addr = inet_addr(Networking::getIPv4().c_str());// your own ip (laptop)
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
		
		std::cout << "Message from Client" << Buffer << std::endl;
		iCloseSocket = closesocket(UDPSocketClient);
		iWsaCleanup = WSACleanup();
	}
}

void Networking::FLOOD(std::string IPv4 ,std::string input)
{


	if (Networking::getIPv4() == IPv4)
	{
		std::cout << "Same " << std::endl;
	}
	else
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
		std::cout << "Flood Ipv4 : " << IPv4 << std::endl;

	}

		
	//	std::cout << "Sending all client this message" << input << std::endl;

	
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
	//std::cout << "This is my IPv4 address :  "<< szLocalIP << std::endl;
	WSACleanup();
	return szLocalIP;

}
