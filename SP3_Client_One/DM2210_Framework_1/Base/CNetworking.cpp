#include "CNetworking.h"

// server

CNetworking::CNetworking()
{
}


CNetworking::~CNetworking()
{
}

void CNetworking::TalktoServer(std::string ServerIP,std::string sms)
{

	iWsaStartup = WSAStartup(MAKEWORD(2, 2), &WinSockData); // NEW
	UDPServer.sin_family = AF_INET;// NEW
	UDPServer.sin_addr.s_addr = inet_addr(ServerIP.c_str());// the other computer ip(computer)
	UDPServer.sin_port = htons(8001);// NEW
	UDPSocketClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	strcpy(Buffer, sms.c_str()); // or pass &s[0]

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
	//std::cout << "This is my own IP Address : " << szLocalIP << std::endl;
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

		ServerIPv4 = Buffer;
		std::cout << "Message from Sever IP:" << ServerIPv4 << std::endl;
	}
}

void CNetworking::GetServerIPStart()
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

		ServerIPv4 = Buffer;
		std::cout << "Message from Sever IP:" << ServerIPv4 << std::endl;
	
}


std::string CNetworking::IPtoASCII() {

	std::string ip = CNetworking::getIPv4();

	int numberOfDots = 0;
	std::vector<int> dotPos;
	int ascci;
	char y;
	std::string output;


	for (std::string::size_type i = 0; i < ip.size(); i++)
	{
		if ('.' == ip[i])
		{
			numberOfDots++;
			dotPos.push_back(i);
		}	
	}


	


	// Doing Halfway (Convert the last address to ascii)
	std::string ThridIP = ip.substr(dotPos[1] + 1,  ip.size() - dotPos[2] ); // X.X 
	//std::cout << ThridIP << std::endl;

	ascci = stoi(ThridIP);
	y = ascci;
	//std::cout << y << std::endl;
	std::string s(1, y); //convert char to string
	output.append(s);


	std::string FourthIP = ip.substr(dotPos[2] + 1);
	std::cout << FourthIP << std::endl;
	ascci = stoi(FourthIP);
	y = ascci;
	//std::cout << y << std::endl;
	std::string k(1, y); //convert char to string
	output.append(k);


	std::cout << output << std::endl;

	return output;
}