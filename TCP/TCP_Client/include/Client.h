#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <iostream>

class Client
{
public:
	Client() = default;
	~Client();

	void Init();
	void Connect(unsigned int p_port);
	void Send(const std::string& p_message);
	
public:
	SOCKET m_socket;
	SOCKADDR_IN m_address;
};
