#include <iostream>
#include <string>
#include "Networking.h"
#include <thread>
#include <sstream>



/*
	Send Packets to the specifer IP address
*/



/*
	Listen for packets that u get from the other client 
*/




int main() {
	Networking network;
	


	int NumberofClient = 2;
	std::string user_input;


	//std::cout << "Number of Clients" << std::endl;
	//std::cin >> NumberofClient;

	/*for (int i = 0; i < NumberofClient; i++)
	{
		std::cout << "Enter Clients " << i << "IP" ;
		std::cin >> user_input;
		network.allClientIPv4.push_back(user_input);
	}*/
	
	network.allClientIPv4.push_back("172.21.133.111"); //client 1
	network.allClientIPv4.push_back("172.21.133.75"); // client 2


	// listen for any incoming message 
	std::thread t2(&Networking::listener, network);
	t2.detach();



	while (true)
	{
		for (int i = 0; i < NumberofClient; i++)
		{
			if (i == 0)
			{
				//std::cout << "Send out a trigger : !" << std::endl;
				network.SendPackClient(network.allClientIPv4[i], "!" + std::to_string(i));
				//Sleep(1000);
			}


			if (i == 1)
			{
				//std::cout << "Send out a trigger : !" << std::endl;
				network.SendPackClient(network.allClientIPv4[i], "!" + std::to_string(i));
				//Sleep(2000);
			}

			Sleep(1000);
		}
	}




	system("pause>nul");
	return 0;
}