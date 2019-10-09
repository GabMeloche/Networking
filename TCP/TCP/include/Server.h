#pragma once
#include <WinSock2.h>
#include <iostream>

class Server
{
public:
	Server() = default;
	~Server();

	void Init();
	void Bind(unsigned int p_port);
	void Listen();
	void Accept();
	void Receive();

	bool m_listen = true;
	SOCKET m_socket;
	SOCKET m_cSock;
	SOCKADDR_IN m_address;
};