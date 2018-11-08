#include "stdafx.h"
#include "Client.h"
bool Client::LoadWinsock()
{
	WSAData data;
	WORD version = MAKEWORD(2, 2);

	int wsInit = WSAStartup(version, &data);
	int wsResult = WSAStartup(version, &data);
	if (wsResult != 0)
	{
		std::cerr << "Error. No se puede iniciar Winsock: " << wsResult << std::endl;
	}
	return wsInit == 0;
}

bool Client::CreateSocket()
{
	SOCKET tmp = socket(AF_INET, SOCK_STREAM, 0);
	if (tmp == INVALID_SOCKET)
	{
		WSACleanup();
		std::cerr << "Error. No se puede crear el socket: " << WSAGetLastError() << std::endl;
		return false;
	}
	sock = tmp;
	return true;
}

bool Client::ConectToSocket()
{
	int conResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (conResult == SOCKET_ERROR)
	{
		std::cerr << "Error. No se puede conectar con el servidor: " << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return false;
	}
	return true;
}

Client::Client(std::string ip, int port)
	: m_ip(ip), m_port(port)
{
}

Client::~Client()
{
	CleanUp();
}

int Client::SendToServer(std::string message)
{
	int sendResult = send(sock, message.c_str(), message.size() + 1, 0);
	return sendResult;
}

bool Client::ValidSocket() const
{
	return bytesReceived != INVALID_SOCKET; //si bytes == INVALID_SOCKET --> desconectado o error
}

std::string Client::ReceiveFromServer()
{
	//wait for response
	ZeroMemory(buffer, 100);
	bytesReceived = recv(sock, buffer, 100, 0);
	//std::cout << std::string(buffer, 0, bytesReceived) << std::endl;
	std::string received(buffer, 0, bytesReceived);
	return received;
}

bool Client::Init()
{
	if (LoadWinsock())
		if (CreateSocket())
		{
			GetAddrsInfo(m_ip, m_port);
			if (ConectToSocket())
				return true;
		}
	return false;
}

void Client::GetAddrsInfo(std::string ipAdress, int port)
{
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAdress.c_str(), &hint.sin_addr);
}

void Client::Run()
{
	do
	{
		std::getline(std::cin, userInput);
		if (userInput.size() > 0) //Mira que el usuario haya escrito algo
		{
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			ReceiveFromServer();
		}

	} while (true);
}

void Client::CleanUp()
{
	closesocket(sock);
	WSACleanup();
}

