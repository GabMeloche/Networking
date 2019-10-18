#pragma once

#include <Export.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <iostream>

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

		inline void SetId(int p_id) { m_id = p_id; }

		inline int GetId() const { return m_id; }
	public:
		int m_id;
		std::string m_name;
		SOCKET m_socket{};
		SOCKADDR_IN m_address{};
	};
	
	UDP_API Client* RunClient();
	UDP_API void Send(Client* p_client, float p_x, float p_y, float p_z);
	UDP_API void ReceiveUnity(int p_id, float p_x, float p_y, float p_z);
}
