#include "Networking.h"


#include <fcntl.h>
#include <io.h>






Networking::Networking()
{

	
}


Networking::~Networking()
{
}

void Networking::SendPackClient(std::string IP, std::string input)
{
		iWsaStartup = WSAStartup(MAKEWORD(2, 2), &WinSockData); // NEW
		UDPServer.sin_family = AF_INET;// NEW
		UDPServer.sin_addr.s_addr = inet_addr(IP.c_str());// the other computer ip(computer)
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
		
		// Client 1 send : # $ 10 50 20

		std::cout << "Message from Client" << Buffer << std::endl;


		/*
		std::string firstSign;
		std::string secondSign;
		std::string CopyOfBuffer;

		CopyOfBuffer = Buffer;

		firstSign = CopyOfBuffer.substr(0, 1);

		secondSign = CopyOfBuffer.substr(1, 1);

		//convert the two string to char 

		char cstr[2];
		strcpy_s(cstr, firstSign.c_str());    // or pass &s[0]
		std::cout << cstr << std::endl;

		char cstr2[2];
		strcpy_s(cstr2, secondSign.c_str());    // or pass &s[0]
		std::cout << cstr2 << std::endl;

		// char back to int 



		int x = static_cast<unsigned char>(cstr[0]);
		int y = static_cast<unsigned char>(cstr2[0]);

		std::string backIP;

		backIP.append(std::to_string(x));
		backIP.append(".");
		backIP.append(std::to_string(y));


		std::cout << backIP << std::endl;

		// find which string in the vector has the ip address

		for (int i = 0; i < allClientIPv4.size(); i++)
		{
			std::size_t found = allClientIPv4[i].find(backIP);
			if (found != std::string::npos)
			{
				foundtheOne = i;
			}
		}

		std::cout << foundtheOne << std::endl;
		*/

		std::string messageFromClient = Buffer;


		if (oldLimitC1 > 90)
		{
			oldLimitC1 = 0;
		}
		if (oldlimitC2 > 90)
		{
			oldlimitC2 = 0;
		}

			// Message from Client 2 - ^
		if (messageFromClient.substr(0, 1) == "2") // Client 2
		{
			std::cout << "Send message to Client 1 " << std::endl;

			if (messageFromClient.substr(1,1) == "@")
			{
				// (2)(124) (,) (125) (F)
				// 2124, 125 f
				// 01234 567 8
				/*findAnd = messageFromClient.find(",");
				int xpos = stoi(messageFromClient.substr(2, findAnd - 2));
				int zpos = stoi(messageFromClient.substr(findAnd + 1, messageFromClient.size() - findAnd - 2));
				std::string item = messageFromClient.substr(messageFromClient.size() - 1);
				std::string packet = "@" + std::to_string(xpos) + std::to_string(zpos) + item;*/
				std::cout << "Send message to Client 1 " << std::endl;

				SendPackClient(allClientIPv4[0], Buffer);


				//for debug 
			/*	std::cout << messageFromClient.substr(2, findAnd - 2) << std::endl;
				std::cout << messageFromClient.substr(findAnd + 1, messageFromClient.size() - findAnd -2 ) << std::endl;
				std::cout << messageFromClient.substr(messageFromClient.size() - 1) << std::endl;*/
			}
			else
			{
				// player positoion packet
				findAnd = messageFromClient.find("^");
				//std::cout << messageFromClient.substr(findAnd + 1) << std::endl;

				NewPacketLimit = stoi(messageFromClient.substr(findAnd + 1));

				if (NewPacketLimit > oldlimitC2)
				{
					//std::cout << "Send the Packet out it is NEW ONE" << std::endl;
					oldlimitC2 = NewPacketLimit;

					// break the packet later
					SendPackClient(allClientIPv4[0], Buffer);
				}
			}
			

		}
		else // Client 1 - &
		{
			if (messageFromClient.substr(1, 1) == "I")
			{
				// (2)(124) (,) (125) (F)
				// 2124, 125 f
				// 01234 567 8
				/*findAnd = messageFromClient.find(",");
				int xpos = stoi(messageFromClient.substr(2, findAnd - 2));
				int zpos = stoi(messageFromClient.substr(findAnd + 1, messageFromClient.size() - findAnd - 2));
				std::string item = messageFromClient.substr(messageFromClient.size() - 1);
				std::string packet = "@" + std::to_string(xpos) + std::to_string(zpos) + item;*/
				std::cout << "Send message to Client 2 " << std::endl;
				SendPackClient(allClientIPv4[1], Buffer);


				//for debug 
			/*	std::cout << messageFromClient.substr(2, findAnd - 2) << std::endl;
				std::cout << messageFromClient.substr(findAnd + 1, messageFromClient.size() - findAnd -2 ) << std::endl;
				std::cout << messageFromClient.substr(messageFromClient.size() - 1) << std::endl;*/
			}
			else
			{
				//std::cout << "Send message to Client 2 " << std::endl;
				findAnd = messageFromClient.find("&");
				//std::cout << messageFromClient.substr(findAnd + 1) << std::endl;

				NewPacketLimit = stoi(messageFromClient.substr(findAnd + 1));

				if (NewPacketLimit > oldLimitC1)
				{
				//	std::cout << "Send the Packet out it is NEW ONE" << std::endl;
					oldLimitC1 = NewPacketLimit;

					// break the packet later
					SendPackClient(allClientIPv4[1], Buffer);
				}
			}
		}
		iCloseSocket = closesocket(UDPSocketClient);
		iWsaCleanup = WSACleanup();
	}
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
	WSACleanup();
	return szLocalIP;

}
