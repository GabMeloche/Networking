#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <iostream>

#include <Client.h>

#define PORT 8755


int	main()
{
	Client client;
	
	client.Init();

	client.Connect(PORT);
	
	client.Send("hello world");
	client.Send("hello world2");
	client.Send("hello world3");

	return EXIT_SUCCESS;
}