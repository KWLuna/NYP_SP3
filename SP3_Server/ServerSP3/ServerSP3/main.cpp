#include <iostream>
#include <string>
#include "Networking.h"
#include <thread>


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
		std::cout << s[i] << ' ';

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




void ServerStatus(std::string CopyOwnIPv4, std::string CopyCopyOwnIPv4, Networking network) {


	// tell everyone i am ONLINE
	while (true)
	{
		for (int i = 0; i < 256; i++)
		{
			CopyOwnIPv4.append(std::to_string(i));
			network.FLOOD(CopyOwnIPv4, "ServerAddress:" + network.getIPv4());
			//std::cout << CopyOwnIPv4 << std::endl;
			CopyOwnIPv4 = CopyCopyOwnIPv4;

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
	std::cout << ownIPv4 << std::endl;

	// Step 2
	// SPAM Packets to all IP on the network 



	// sub string the to find the last part of it 
	
	int posofLastdot = print(CopyOwnIPv4);
	std::cout << "Number is : " << posofLastdot << std::endl;

	// break string until last dot pos
	CopyOwnIPv4 = CopyOwnIPv4.substr(0, posofLastdot + 1);
	std::cout << CopyOwnIPv4 << std::endl;


	// Make a copy of it to Another copy to replace it 
	// everytime it appened to get the normal one with out 
	// last . somthing 
	// 
	std::string CopyCopyOwnIPv4;
	CopyCopyOwnIPv4 = CopyOwnIPv4;


	// detach thread send server ip every 10 secs
	std::thread Thread (ServerStatus,CopyOwnIPv4, CopyCopyOwnIPv4, network);
	Thread.detach();
	











	system("pause>nul");
	return 0;
}