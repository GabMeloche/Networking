#include <Server.h>

Server::~Server()
{
	for (auto& socket : m_cSock)
	{
		closesocket(socket);
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
	SOCKADDR_IN csin = { 0 };

	int sinsize = sizeof csin;
	m_cSock.push_back(accept(m_socket, (SOCKADDR*)& csin, &sinsize));


	if (m_cSock.back() == INVALID_SOCKET)
	{
		perror("accept()");
		std::cout << "INVALID_SOCKET" << std::endl;
		std::cin.get();
		return;
	}
	std::cout << "Accept" << std::endl;
}

void Server::Receive(SOCKET p_socket)
{
	char buffer[1024];
	int n = 0;

	if ((n = recv(p_socket, buffer, sizeof buffer - 1, 0)) < 0)
	{
		perror("recv()");
		exit(errno);
	}

	buffer[n] = '\0';
	std::cout << buffer << std::endl;
	const char* message = "8Received";

	send(p_socket, message, sizeof(char) * 9, 0);

}

