#include "Networking.h"


#include <fcntl.h>
#include <io.h>



		// The server will convert the ASCII back to IP address number 
			// Use the number to find it in the Vector and then send the message out 



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

		dotpos.clear();

		for (int i = 0; i < allClientIPv4.size(); i++)
		{
			std::cout << allClientIPv4[i] << std::endl;
		}

		inVector = false;

		//std::cout << "listing now" << std::endl;
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

		std::cout << iBufferLen << std::endl;

		if (iReceiveFrom == SOCKET_ERROR)
		{
			return;
		}
		
		//std::cout << "Message from Client" << Buffer << std::endl;
		iCloseSocket = closesocket(UDPSocketClient);
		iWsaCleanup = WSACleanup();





		// need to test this on another computer later 
		for (std::string::size_type i = 0; i < strlen(Buffer); i++)
		{
			if ('.' == Buffer[i])
			{
				dotpos.push_back(i);
			}
		}


		if (dotpos.size() != 3) // It is a normal pack coming in do convert back and check 
		{
			// step one convert the pack ip to ip and check aginst the vector

			std::cout << "step one convert the pack ip to ip and check aginst the vector" << std::endl;
			std::string firstSign = "BLANK";
			std::string secondSign = " ";
			std::string CopyOfBuffer = " ";

			CopyOfBuffer = Buffer;
			std::cout << " Copy of Buffer : " << CopyOfBuffer << std::endl;

			firstSign = CopyOfBuffer.substr(0, 1);
			std::cout << " First Sign : " << firstSign << std::endl;

			secondSign = CopyOfBuffer.substr(1, 1);
			std::cout << "Scound Sign : " << secondSign << std::endl;

			 //convert the two string to char 
			std::cout << "convert the two string to char " << std::endl;

			char cstr[2];
			strcpy_s(cstr, firstSign.c_str());    // or pass &s[0]
			std::cout << cstr << std::endl;

			char cstr2[2];
			strcpy_s(cstr2, secondSign.c_str());    // or pass &s[0]
			std::cout << cstr2 << std::endl;

			// char back to int 
			std::cout << "convert char back to int  " << std::endl;

		

			int x = static_cast<unsigned char>(cstr[0]);
			std::cout << x << std::endl;
			int y = static_cast<unsigned char>(cstr2[0]);
			std::cout << y << std::endl;


			break;
		}
		else if (dotpos.size() == 3) // to get the ip and store into a vector
		{
			if (allClientIPv4.size() == 0)
			{
				allClientIPv4.push_back(Buffer);
				inVector = true;

			}
			else
			{
				for (int i = 0; i < allClientIPv4.size(); i++)
				{
					if (allClientIPv4[i] == Buffer)
					{
						inVector = true;
					}
				}

			}

			if (!inVector)
			{
				allClientIPv4.push_back(Buffer);
			}
			else
			{
				continue;
			}
		}
		

	


	}
}

void Networking::FLOOD(std::string IPv4 ,std::string input)
{


	if (Networking::getIPv4() == IPv4)
	{
		//std::cout << "Same " << std::endl;
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
		//std::cout << "Flood Ipv4 : " << IPv4 << std::endl;

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
