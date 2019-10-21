#include <Client.h>
#include <Server.h>

#define PORT 8755

int main()
{	
	std::string option;
	std::getline(std::cin, option);

	if (option == "s")
	{
		std::cout << "server selected\n";
		Server* server = RunServer();
		while (true)
		{	
			std::string ip;
			std::getline(std::cin, ip);
			std::cout << "people connected: " << NumberOfConnections(server) << std::endl;
		}
	}
	else
	{
		std::cout << "client selected; enter IP address of server:\n";
		std::string ip;
		std::getline(std::cin, ip);
		Client* client = RunClient();
		while (true)
		{
			Send(client, 1.05f, 2.08f, 3.12f);
			std::string message;
			std::getline(std::cin, message);

			client->Send(message.c_str());
		}
	}

	
	return 0;
}