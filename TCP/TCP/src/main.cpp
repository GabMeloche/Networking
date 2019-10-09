#include <WinSock2.h>
#include <iostream>

#include <Server.h>

#define PORT 8755

int	main()
{
	Server server{};
	server.Init();

	server.Bind(PORT);

	server.Listen();
	server.Accept();

	std::cout << "type EXIT to quit\n";
	while(server.m_listen)
	{
		server.Receive();	
	}
	
	return EXIT_SUCCESS;
}