#pragma once

#include <Export.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <iostream>

#include <Server.h>

class UDP_API Client
{
public:
	Client() = default;
	~Client();

	void Init(std::string p_name);
	bool Connect(unsigned int p_port, std::string p_address);
	void Send(std::string p_message);
	void ReceiveNames();
	void Receive();
	void CreateServer(const unsigned int p_port, std::string p_ip);
	
	//inline Server& GetServer(){ return m_server; }
	inline std::string GetName() const { return m_name; }
	inline SOCKET GetSocket() const { return m_socket; }
	inline SOCKADDR_IN GetAddress() const { return m_address; }
	
public:
	std::string m_name;
	SOCKET m_socket{};
	SOCKADDR_IN m_address{};
	Server m_server;
	
};
