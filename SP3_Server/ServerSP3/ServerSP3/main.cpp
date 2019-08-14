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

int main() {
	Networking network;
	std::thread Thread;
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
	
	for (int i = 0; i < 255; i++)
	{
		CopyOwnIPv4.append(std::to_string(i));
		network.SPAM(CopyOwnIPv4, "Hi from server");
		//std::cout << CopyOwnIPv4 << std::endl;
		CopyOwnIPv4 = CopyCopyOwnIPv4;

	}











	system("pause>nul");
	return 0;
}