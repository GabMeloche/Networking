#include <stdafx.h>
#include <Client.h>
#include <Vector3.h>
#include <thread>

extern "C"
{
	void RunClient()
	{
		Client client{};
		client.Init(8755);
	}

	void Send(float p_x, float p_y, float p_z)
	{
		
	}
	
	void Client::RunClient()
	{
		Client client{};
		client.Init(8755);
		while (true)
		{

		}
	}

	Client::~Client()
	{
		std::cout << "close listen" << std::endl;
		closesocket(m_socket);
		std::cout << "close ori" << std::endl;
		WSACleanup();
		std::cout << "Done" << std::endl;
		std::cin.get();
	}


	void Client::Init(int p_port)
	{
		//m_name = std::string(p_name);

		WSADATA wsa;
		int err = WSAStartup(MAKEWORD(2, 2), &wsa);
		if (err < 0)
		{
			std::cout << "WSAStartup failed !" << std::endl;
			std::cin.get();
			return;
		}
		std::cout << "init WSA" << std::endl;

		m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (m_socket == INVALID_SOCKET)
		{
			perror("socket()");
			std::cout << "INVALID_SOCKET" << std::endl;
			std::cin.get();
			return;
		}

		m_address = SOCKADDR_IN{ 0 };
		m_address.sin_family = AF_INET;
		//m_address.sin_addr.s_addr = reinterpret_cast<ULONG>("10.5.5.104");
		m_address.sin_port = htons(p_port);

		std::cout << "init socket" << std::endl;
		if (inet_pton(AF_INET, "127.0.0.1", &m_address.sin_addr) <= 0)
		{
			std::cout << "client inet_pton error\n";
			return;
		}

		char str[15];
		inet_ntop(AF_INET, &(m_address.sin_addr), str, 15);
		Send("1");
		std::thread t{ &Client::Receive, this };
		t.detach();
	}

	void Client::Send(const char* p_message)
	{
		std::string buffer{ p_message };
		int n = sendto(m_socket, buffer.c_str(), buffer.length(), 0, reinterpret_cast<SOCKADDR*>(&m_address), sizeof(m_address));

		if (n < 0)
		{
			perror("client sendto()");
		}
	}

	void Client::Receive()
	{
		while (true)
		{
			char buffer[1024];
			int n = 0;
			int length = sizeof(m_address);
			if ((n = recvfrom(m_socket, buffer, sizeof buffer - 1, 0, reinterpret_cast<SOCKADDR*>(&m_address), &length)) < 0)
			{
				perror("client recvfrom()");
				return;
			}

			buffer[n] = '\0';

			std::cout << buffer << std::endl;
		}
	}
}
