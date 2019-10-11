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
	
	std::cout << "enter your name: ";
	std::string name;
	getline(std::cin, name);
	client.Init(name);

	client.Connect(PORT, "127.0.0.1");
	
	std::thread t{ &Client::ReceiveBroadcast, client };
	while(true)
	{
		//client.ReceivePing();
		std::string string;
		getline(std::cin, string);
		client.Send(string);

		char received[9];
		recv(client.m_socket, received, sizeof(char) * 9, 0);
		received[8] = '\0';
		std::cout << received << std::endl << std::endl;
	}
	t.join();
	return EXIT_SUCCESS;
}