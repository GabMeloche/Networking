#include <WinSock2.h>
#include <iostream>

#include <Server.h>
#include <thread>
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
		char buffer[1];
		int n = recv(server.m_socket, buffer, sizeof(char), 0);
		if (n > 0)
		{
			server.Accept();
			/*std::thread t{ &Server::Receive, server, server.m_cSock.back() };
			t.detach();*/
		}
		
		server.Receive(server.m_cSock.back());
	}
	
	return EXIT_SUCCESS;
}