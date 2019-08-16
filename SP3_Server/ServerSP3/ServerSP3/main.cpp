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


int print(const std::string &s)
{

	int numberOFdots = 0;

	for (std::string::size_type i = 0; i < s.size(); i++) {
		//std::cout << s[i] << ' ';

		if ('.' == s[i])
		{
			numberOFdots++;
		}


		if (numberOFdots == 3)
		{
			return i;
		}
	}

}




void ServerStatus(std::string CopyOwnIPv4, std::string deafulIPv4, Networking network) {


	// tell everyone i am ONLINE
	while (true)
	{
		for (int i = 0; i < 255; i++)
		{
				network.FLOOD(CopyOwnIPv4.append(std::to_string(i)), network.getIPv4());
				CopyOwnIPv4 = deafulIPv4;
		}

		Sleep(5000);
	}
	

}

int main() {
	Networking network;
	std::string ownIPv4;
	std::string CopyOwnIPv4;

	// get own IPV4
	ownIPv4 = network.getIPv4();
	CopyOwnIPv4 = ownIPv4;
	//std::cout << ownIPv4 << std::endl;
	
	int posofLastdot = print(CopyOwnIPv4);
	//std::cout << "Number is : " << posofLastdot << std::endl;

	// break string until last dot pos
	CopyOwnIPv4 = CopyOwnIPv4.substr(0, posofLastdot + 1);
	//std::cout << CopyOwnIPv4 << std::endl;

	std::string CopyCopyOwnIPv4;
	CopyCopyOwnIPv4 = CopyOwnIPv4;


	// Send my IP to all client every x sec
	std::thread t1(ServerStatus, CopyCopyOwnIPv4, CopyOwnIPv4, network);
	t1.detach();

	// listen for any incoming message 
	std::thread t2(&Networking::listener, network);
	t2.detach();

	// where to send out to now 






	system("pause>nul");
	return 0;
}