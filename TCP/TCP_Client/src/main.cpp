#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <iostream>

#include <Client.h>
#include <thread>

#define PORT 8755
#define MAXCHARS 1024

int	main()
{
	Client client;
	
	std::cout << "Enter your name: ";
	std::string name;
	getline(std::cin, name);
	client.Init(name);
	
	std::cout << "Enter IP address of server (enter nothing for default local server): ";
	std::string ip;
	getline(std::cin, ip);

	client.Connect(PORT, ip);
	
	std::thread t{ &Client::ReceiveBroadcast, client };
	std::cout << "Type CONNECTED_USERS to see who's currently connected to the server\n";
	
	while(true)
	{
		std::string string;
		getline(std::cin, string);
		client.Send(string);
	}
	t.join();
	return EXIT_SUCCESS;
}