#include <Server.h>
#include <string>

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

		std::string name = Receive(cSock);
		m_cSock.try_emplace(std::string(name), cSock);

		const char size = '9';
		const char* message = "Connected";
		//send(cSock, &size, sizeof(char), 0);
		send(cSock, message, sizeof(char) * 10, 0);

		std::thread t{ &Server::ReceiveThreaded,  m_cSock.begin()->second };
		t.detach();
	}
}

std::string Server::Receive(SOCKET p_socket)
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
		
		std::cout << tmp << std::endl;

		const char* message = "Received";

		send(p_socket, message, sizeof(char) * 9, 0);
		return tmp;
}

void Server::ReceiveThreaded(SOCKET p_socket)
{
	while (true)
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

		std::cout << buffer << std::endl;

		const char* message = "Received";

		send(p_socket, message, sizeof(char) * 9, 0);
	}
}

char* Server::ReceiveMessage(std::pair<const std::string, SOCKET>& p_cSock)
{
	char buffer[1024];
	int n = 0;

	if ((n = recv(p_cSock.second, buffer, sizeof buffer, 0)) < 0)
	{
		perror("recv()");
		std::cin.get();
		exit(errno);
	}

	buffer[n] = '\0';
	std::cout << p_cSock.first << ": " << buffer << std::endl;

	const char* message = "Received";

	send(p_cSock.second, message, sizeof(char) * 9, 0);
	return buffer;
}

void Server::ReceiveAll()
{
	//while (true)
	{
		for (auto& socket : m_cSock)
		{
			ReceiveMessage(socket);
		}
	}
}
void Server::Print(const char* p_message)
{
	
}

bool Server::Ping(SOCKET p_socket)
{
	std::string buffer = "~";

	if (send(p_socket, buffer.c_str(), buffer.length(), 0) <= 0)
	{
		closesocket(p_socket);
		return false;
	}
	return true;
}


