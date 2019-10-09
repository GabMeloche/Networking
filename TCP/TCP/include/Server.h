#pragma once
#include <WinSock2.h>
#include <iostream>
#include <vector>
#include <thread>

//1 client socket per client connection
class Server
{
public:
	Server() = default;
	~Server();

	void Init();
	void Bind(unsigned int p_port);
	void Listen();
	void Accept();
	void Receive(SOCKET p_socket);

	bool m_listen = true;
	unsigned int m_port;
	SOCKET m_socket;
	SOCKADDR_IN m_address;
	std::vector<SOCKET> m_cSock;
	//std::vector<std::pair<SOCKET, std::thread>> m_cSocks;
};
