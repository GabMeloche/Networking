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

	void Init(const std::string& p_name);
	void Connect(unsigned int p_port, std::string& p_address);
	void Send(const std::string& p_message);
	void ReceiveNames();
	void ReceiveBroadcast();
	
public:
	std::string m_name;
	SOCKET m_socket;
	SOCKADDR_IN m_address;
	
};
