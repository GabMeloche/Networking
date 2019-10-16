#pragma once

#include <Export.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <iostream>

#include <Server.h>

extern "C"
{
	class UDP_API Client
	{
	public:
		Client() = default;
		~Client();

		void Init(int p_port);
		void Send(const char* p_message);
		void Receive();

	public:
		std::string m_name;
		SOCKET m_socket{};
		SOCKADDR_IN m_address{};
	};
}
