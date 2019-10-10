#include <WinSock2.h>
#include <iostream>
#include <chrono>
#include <Server.h>
#include <thread>
#define PORT 8755

int	main()
{
	Server server{};
	server.Init();

	server.Bind(PORT);

	server.Listen();

	std::thread t{ &Server::Accept, server };

	std::cout << "type EXIT to quit\n";

	while (true)
	{
		for (auto& socket: server.m_cSock)
		{
			if (!server.Ping(socket.second))
				std::cout << socket.first << " has disconnected\n";
		}
	}
	
	t.join();
	std::cin.get();
	return EXIT_SUCCESS;
}