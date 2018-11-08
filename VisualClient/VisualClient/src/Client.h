#pragma once
#include <string>
#include <iostream>
#include <WS2tcpip.h>					//Winsock header
#pragma comment (lib,"ws2_32.lib")		//Winsock library

#define INVALID_INPUT_FROM_SERVER "INVALID"



class Client
{
private:
	std::string m_ip;
	int m_port;
	SOCKET sock;
	sockaddr_in hint;
	char buffer[100];
	std::string userInput;
	std::string joinMessage = "Hello";
	int bytesReceived;

	bool LoadWinsock();
	bool CreateSocket();
	bool ConectToSocket();
	void GetAddrsInfo(std::string ipAdress, int port);
	void CleanUp();


public:

	Client(std::string ip, int port);
	~Client();
	bool Init();
	void Run();
	std::string ReceiveFromServer();
	int SendToServer(std::string message);
	bool ValidSocket() const;
};



