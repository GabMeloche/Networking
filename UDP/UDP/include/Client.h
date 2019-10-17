#pragma once

#include <Export.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <iostream>

extern "C"
{
	UDP_API void RunClient();
	UDP_API void Send(float p_x, float p_y, float p_z);
	
	class UDP_API Client
	{
	public:
		Client() = default;
		~Client();

		static void RunClient();
		void Init(int p_port);
		void Send(const char* p_message);
		void Receive();

	public:
		std::string m_name;
		SOCKET m_socket{};
		SOCKADDR_IN m_address{};
	};
}
