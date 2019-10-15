#include <Client.h>

#define PORT 8755

int main()
{
	Client client{};
	std::cout << "Enter your name: ";
	std::string name;
	getline(std::cin, name);
	client.Init(name.c_str());

	std::cout << "Enter IP address of server (enter nothing for default local server): ";
	std::string ip;
	getline(std::cin, ip);

	if (!client.Connect(PORT, ip.c_str()))
	{
		std::cout << name << " must create server\n";

		client.m_server.Init();
		client.m_server.Bind(PORT);
		client.m_server.Listen();

		std::thread t{ &Server::Accept, client.m_server };
		t.detach();
		client.Connect(PORT, ip.c_str());
	}

	std::thread t2{ &Client::Receive, client };
	t2.detach();
	
	while (true)
	{
		std::string string;
		getline(std::cin, string);
		client.Send(string.c_str());
	}
	return 0;
}