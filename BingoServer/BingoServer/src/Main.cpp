#include <iostream>
#include <string>
#include "ServerListener.h"
#include "CartonGenerator.h"

void Listener_MessageReceived(ServerListener* listener, int client, std::string message);

std::string GetIpInput()
{
	std::string userInput;
	std::cout << "IP del servidor:" << std::endl;
	std::getline(std::cin, userInput);
	return userInput;
}

int GetPortInput()
{
	std::string userInput;
	std::cout << "PUERTO del servidor:" << std::endl;
	std::getline(std::cin, userInput);
	return std::stoi(userInput);
}

void main()
{
	std::string ip = GetIpInput();
	int port = GetPortInput();
	ServerListener server(ip, port, Listener_MessageReceived);
	std::cout << "Conectando..." << std::endl;
	std::cout << ">" << std::endl;

	if (server.Init())
	{
		server.Run();
	}
}

void Listener_MessageReceived(ServerListener* listener, int client, std::string message)
{
	//lo que se envia al cliente
	listener->Send(message, client);
}
