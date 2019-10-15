#pragma once
#include <Export.h>
#include <WinSock2.h>
#include <iostream>
#include <vector>
#include <thread>
#include <map>

//1 client socket per client connection
class UDP_API Server
{
public:
	Server() = default;
	~Server();

	void Init();
	void Bind(unsigned int p_port);
	void Listen();
	void Accept();
	void ReceiveThreaded(SOCKET p_socket, std::string p_name);
	void SendNames(SOCKET p_socket);
	void Broadcast(const char* p_message);
	std::string Receive(SOCKET p_socket, bool p_print);

	unsigned int m_port;
	SOCKET m_socket;
	SOCKADDR_IN m_address;
	std::map<std::string, SOCKET> m_cSock;
};
