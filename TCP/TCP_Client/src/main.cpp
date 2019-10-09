#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <iostream>

#include <Client.h>

#define PORT 8755
#define MAXCHARS 1024

int	main()
{
	Client client;
	
	client.Init("Client 1");

	client.Connect(PORT, "127.0.0.1");

	while(true)
	{
		std::string string;
		getline(std::cin, string);
		client.Send(string);

		char chars[1];
		char received[9];
		
		std::cout << "receive1: " << recv(client.m_socket, chars, sizeof (char), 0) << std::endl;
		std::cout << "receive2: " << recv(client.m_socket, received, sizeof(char) * 8, 0) << std::endl;
		received[8] = '\0';
		std::cout << received << std::endl;
	}

	return EXIT_SUCCESS;
}