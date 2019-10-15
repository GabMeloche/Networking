#include <stdafx.h>
#include <Server.h>

Server::~Server()
{
	for (auto& socket : m_cSock)
	{
		closesocket(socket.second);
		std::cout << "close listen socket" << std::endl;
	}

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

	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
	{
		perror("socket()");
		std::cout << "INVALID_SOCKET" << std::endl;
		std::cin.get();
		return;
	}
	std::cout << "init socket" << std::endl;
}

void Server::Bind(unsigned int p_port)
{
	m_address = SOCKADDR_IN{ 0 };
	m_address.sin_addr.s_addr = htonl(INADDR_ANY); /* nous sommes un serveur, nous acceptons n'importe quelle adresse */
	m_address.sin_family = AF_INET;
	m_address.sin_port = htons(p_port);

	if (bind(m_socket, (SOCKADDR*)& m_address, sizeof m_address) == SOCKET_ERROR)
	{
		perror("bind()");
		std::cout << "SOCKET_ERROR" << std::endl;
		std::cin.get();
		return;
	}

	m_port = p_port;
	std::cout << "Bind" << std::endl;
}

void Server::Listen()
{
	if (listen(m_socket, 1) == SOCKET_ERROR)
	{
		perror("listen()");
		std::cout << "SOCKET_ERROR" << std::endl;
		std::cin.get();
		return;
	}
	std::cout << "Listen" << std::endl;
}

void Server::Accept()
{
	while (true)
	{
		SOCKADDR_IN csin = { 0 };

		int sinsize = sizeof csin;
		SOCKET cSock = accept(m_socket, (SOCKADDR*)& csin, &sinsize);

		if (cSock == INVALID_SOCKET)
		{
			perror("accept()");
			std::cout << "INVALID_SOCKET" << std::endl;
			std::cin.get();
			return;
		}

		std::string name = Receive(cSock, false);
		Broadcast(name + " has joined the server\n", name);
		
		m_cSock.try_emplace(std::string(name), cSock);

		std::thread t{ &Server::ReceiveThreaded,  this, m_cSock[name], name };
		t.detach();
	}
}

std::string Server::Receive(SOCKET p_socket, bool p_print)
{

	char buffer[1024];
	int n = 0;

	if ((n = recv(p_socket, buffer, sizeof buffer, 0)) < 0)
	{
		perror("recv()");
		std::cin.get();
		exit(errno);
	}

	buffer[n] = '\0';

	std::string tmp = buffer;

	if (p_print)
		std::cout << tmp << std::endl;

	return tmp;
}

void Server::ReceiveThreaded(SOCKET p_socket, std::string p_name)
{
	while (true)
	{
		char buffer[1024];
		int n = 0;

		if ((n = recv(p_socket, buffer, sizeof buffer, 0)) < 0)
		{
			Broadcast(p_name + " has disconnected from server\n", p_name);
			closesocket(p_socket);
			m_cSock.erase(p_name);
			return;
		}

		buffer[n] = '\0';

		std::string tmp = buffer;
;

		if (tmp == "CONNECTED_USERS")
		{
			std::cout << p_name << " asked for CONNECTED_USERS\n";
			SendNames(p_socket);
			continue;
		}

		Broadcast(p_name + ": " + tmp, p_name);
	}
}

void Server::SendNames(SOCKET p_socket)
{
	std::string names = "Connected users: ";
	for (auto& socket : m_cSock)
	{
		names += socket.first + ", ";
	}

	send(p_socket, names.c_str(), sizeof(names), 0);
}

void Server::Broadcast(const std::string& p_message, std::string p_name)
{
	for (auto& socket: m_cSock)
	{
		send(socket.second, p_message.c_str(), sizeof(p_message), 0);
	}
}
