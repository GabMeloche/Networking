#include <WinSock2.h>
#include <iostream>
#include <chrono>
#include <Server.h>
#include <thread>
#include <string>

#define PORT 8755

int	main()
{
	Server server{};
	server.Init();

	server.Bind(PORT);

	server.Listen();

	std::thread t{ &Server::Accept, server };
	
	while (server.m_listen)
	{		
	}
	
	t.join();
	return EXIT_SUCCESS;
}