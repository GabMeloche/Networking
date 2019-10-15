#include <stdafx.h>
#include <Client.h>

Client::~Client()
{
	std::cout << "close listen" << std::endl;
	closesocket(m_socket);
	std::cout << "close ori" << std::endl;
	WSACleanup();
	std::cout << "Done" << std::endl;
	std::cin.get();
}

void Client::Init(const char* p_name)
{
	m_name = std::string(p_name);
	
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

bool Client::Connect(unsigned p_port, const char* p_address)
{
	if (p_address[0] == '\0')
		p_address = "127.0.0.1";
	
	char request = '1';
	send(m_socket, &request, sizeof(char), 0);
	
	struct hostent* hostinfo = NULL;
	m_address = SOCKADDR_IN{ 0 }; /* initialise la structure avec des 0 */

	inet_pton(AF_INET, p_address, &m_address.sin_addr);/* On encode l'adresse dans la variable sin.sin_addr*/
	m_address.sin_port = htons(p_port); /* on utilise htons pour le port */
	m_address.sin_family = AF_INET;

	char str[15];
	inet_ntop(AF_INET, &(m_address.sin_addr), str, 15);
	std::cout << str << std::endl;

	while (connect(m_socket, (SOCKADDR*)&m_address, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		perror("connect() client");
		return false;
	}
	
	Send(m_name.c_str());
	std::cout << m_name << " is connected to " << p_address << " on port " << p_port << std::endl;
	return true;
}

void Client::Send(const char* p_message)
{
	
	std::string buffer{ p_message };

	if (send(m_socket, buffer.c_str(), buffer.length(), 0) < 0)
	{
		perror("send()");
	}

	if (p_message == "CONNECTED_USERS")
	{
		return;
	}
}

void Client::ReceiveNames()
{
	char buffer[1024];
	int n = 0;
	
	if ((n = recv(m_socket, buffer, sizeof buffer, 0)) < 0)
	{
		perror("recv() client");
		std::cin.get();
		exit(errno);
	}
	std::cout << buffer << std::endl;
}

void Client::Receive()
{
	while (true)
	{
		char buffer[1024];
		int n = 0;

		if ((n = recv(m_socket, buffer, sizeof buffer, 0)) < 0)
		{
			perror("recv()");
			return;
		}

		buffer[n] = '\0';

		std::cout << buffer << std::endl;
	}
}

void Client::CreateServer(const unsigned int p_port, const char* p_ip)
{
	m_server.Init();
	m_server.Bind(p_port);
	m_server.Listen();

	std::thread t{ &Server::Accept, m_server };
	t.detach();
	Connect(p_port, p_ip);
}