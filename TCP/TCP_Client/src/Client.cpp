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

void Client::Init()
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

void Client::Connect(unsigned p_port)
{
	struct hostent* hostinfo = NULL;
	m_address = SOCKADDR_IN{ 0 }; /* initialise la structure avec des 0 */

	inet_pton(AF_INET, "127.0.0.1", &m_address.sin_addr);/* On encode l'adresse dans la variable sin.sin_addr*/
	m_address.sin_port = htons(p_port); /* on utilise htons pour le port */
	m_address.sin_family = AF_INET;

	char str[15];
	inet_ntop(AF_INET, &(m_address.sin_addr), str, 15);
	std::cout << str << std::endl;

	while (connect(m_socket, (SOCKADDR*)&m_address, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		perror("connect()");

	}
}

void Client::Send(const std::string& p_message)
{
	std::string buffer{ p_message };

	if (send(m_socket, buffer.c_str(), buffer.length(), 0) < 0)
	{
		perror("send()");
	}

	std::cout << "sending: " << buffer << std::endl;
	std::cin.get();
}