#pragma once
#include <WinSock2.h>
#include <iostream>
#include <vector>
#include <thread>
#include <map>

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
	static std::string Receive(SOCKET p_socket, bool p_print);
	void ReceiveThreaded(SOCKET p_socket, std::string p_name);
	char* ReceiveMessage(std::pair<const std::string, SOCKET>& p_cSock);
	void Print(const char* p_message);
	bool Ping(SOCKET p_socket);
	void ReceiveAll();
	void SendNames(SOCKET p_socket);
	void Broadcast(const std::string& p_message, std::string p_name);
	
	bool m_listen = true;
	unsigned int m_port;
	SOCKET m_socket;
	SOCKADDR_IN m_address;
	std::map<std::string, SOCKET> m_cSock;
	//std::vector<std::pair<SOCKET, std::thread>> m_cSocks;
};
