#pragma once

#include <Export.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <iostream>
#include <Vector3.h>

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
		Vector3 m_posBuffer;
	};
	
	UDP_API Client* RunClient();
	UDP_API void Send(Client* p_client, float p_x, float p_y, float p_z);
	UDP_API void SendToUnity(Client* p_client, float& p_x, float& p_y, float& p_z);
	UDP_API void DestroyClient(Client* p_client);
}
