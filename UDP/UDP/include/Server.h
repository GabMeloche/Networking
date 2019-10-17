#pragma once
#include <Export.h>
#include <WinSock2.h>
#include <iostream>
#include <thread>
#include <map>
#include <vector>

extern "C"
{
	//1 client socket per client connection
	class UDP_API Server
	{
	public:
		Server() = default;
		~Server();

		void Init();
		void Bind(unsigned int p_port);
		void ReceiveThreaded();
		void Broadcast(const char* p_message);
		std::string GetAddress(sockaddr_in& addr);

		int m_id;
		unsigned int m_port;
		SOCKET m_socket;
		SOCKADDR_IN m_address;
		std::vector<sockaddr_in> m_clients;
	};
}