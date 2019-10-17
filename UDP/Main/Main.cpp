#include <Client.h>
#include <Server.h>

#define PORT 8755

int main()
{
	Server server{};
	Client client{};
	
	std::string option;
	std::getline(std::cin, option);

	if (option == "s")
	{
		std::cout << "server selected\n";
		RunServer();
		while (true)
		{
			
		}
	}
	else
	{
		std::cout << "client selected; enter IP address of server:\n";
		std::string ip;
		std::getline(std::cin, ip);
		client.Init(PORT);

		while (true)
		{
			std::string message;
			std::getline(std::cin, message);

			client.Send(message.c_str());
		}
	}

	
	return 0;
}