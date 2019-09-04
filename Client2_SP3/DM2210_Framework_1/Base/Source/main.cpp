
#include <iostream>
#include <exception>
#include "Application.h"

int main( void )
{
	try
	{
		Application &app = Application::GetInstance();
		app.Init();
		app.Run();
		app.Exit();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << '\n';
		//throw;
	}
	
	
}