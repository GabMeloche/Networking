#include <stdafx.h>
#include <Server.h>
#include <WS2tcpip.h>
#include <Vector3.h>

#define PORT 8755
//Server* Server::m_instance = nullptr;

extern "C"
{
	
	Server* RunServer()
	{
		//TODO: delete server
		Server* server = new Server{};
		server->Init();
		return server;
	}

	int NumberOfConnections(Server* p_server)
	{
		return static_cast<int>(p_server->m_clients.size()); 
	}

	Server::~Server()
	{
		closesocket(m_socket);
		std::cout << "close socket" << std::endl;
		WSACleanup();
		std::cout << "Done" << std::endl;
	}

	void Server::Init()
	{
		WSADATA wsa;
		int err = WSAStartup(MAKEWORD(2, 2), &wsa);
		if (err < 0)
		{
			std::cout << "WSAStartup failed !" << std::endl;
			std::cin.get();
			return;
		}
		std::cout << "init" << std::endl;

		m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (m_socket == INVALID_SOCKET)
		{
			perror("socket()");
			std::cout << "INVALID_SOCKET" << std::endl;
			std::cin.get();
			return;
		}

		Bind(PORT);
		std::thread t{ &Server::ReceiveThreaded, this };
		t.detach();
		std::cout << "init socket" << std::endl;
	}

	void Server::Bind(unsigned int p_port)
	{
		m_address = SOCKADDR_IN{ 0 };
		m_address.sin_family = AF_INET;
		m_address.sin_addr.s_addr = INADDR_ANY; /* nous sommes un serveur, nous acceptons n'importe quelle adresse */
		m_address.sin_port = htons(p_port);

		if (bind(m_socket, reinterpret_cast<SOCKADDR*>(&m_address), sizeof m_address) == SOCKET_ERROR)
		{
			perror("bind()");
			std::cout << "SOCKET_ERROR" << std::endl;
			std::cin.get();
			return;
		}

		m_port = p_port;
		std::cout << "Bind" << std::endl;
	}

	void Server::ReceiveThreaded()
	{
		while (true)
		{
			char buffer[1024];
			int n = 0;

			sockaddr_in from;
			socklen_t fromLen = sizeof(from);
			if ((n = recvfrom(m_socket, buffer, sizeof buffer, 0, reinterpret_cast<sockaddr*>(&from), &fromLen)) < 0)
			{
				std::cout << "server recvfrom() error\n";
				return;
			}

			buffer[n] = '\0';

			std::string tmp = buffer;
			std::cout << "received: " << tmp << " from " << GetAddress(from) << std::endl;


			size_t i = 0;
			while (i != m_clients.size())
			{
				if (GetAddress(m_clients[i]) == GetAddress(from))
				{
					break;
				}
				++i;
			}

			if (i == m_clients.size())
			{
				std::cout << "new connection: " << GetAddress(from) << std::endl;
				m_clients.push_back(from);
			}

			Broadcast(buffer);
			if (tmp == "CONNECTED_USERS")
			{
				std::cout << " asked for CONNECTED_USERS\n";
			}
		}
	}

	std::string Server::GetAddress(sockaddr_in& addr)
	{
		char buff[INET6_ADDRSTRLEN] = { 0 };
		return inet_ntop(addr.sin_family, static_cast<void*>(&(addr.sin_addr)), buff, INET6_ADDRSTRLEN);
	}

	void Server::Broadcast(const char* p_message)
	{
		std::string buffer{ p_message };

		for (auto& client : m_clients)
		{
			sockaddr_in to = client;
			socklen_t toLen = sizeof(to);
			int n = sendto(m_socket, buffer.c_str(), buffer.length(), 0, reinterpret_cast<sockaddr*>(&to), toLen);

			if (n < 0)
			{
				perror("server sendto()");
			}

		}
	}
}