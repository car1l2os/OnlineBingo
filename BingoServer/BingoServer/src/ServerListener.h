#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <WS2tcpip.h>					//Winsock header
#pragma comment (lib,"ws2_32.lib")		//Winsock library
#include "GameStateMachine.h"

#define MAX_BUFFER_SIZE 49152

//Forward declaration 
class ServerListener;

typedef void(*MessageHandler)(ServerListener* listener, int socketID, std::string message);

class ServerListener
{
private:	
	std::string m_ip;
	int m_port; 
	MessageHandler messageReceived;
	SOCKET m_listening;
	fd_set master;
	int m_secondsToBuy;
	GameStateMachine<1,99> gameStateMachine;

	bool LoadWinsock();
	std::string GeyMyIP();
	bool CreateAndBindSocket();
	SOCKET WaitForConnection(SOCKET listening);
	void SetSet();
	void CleanUp();

public:
	ServerListener(std::string ip,int port, MessageHandler handler);
	~ServerListener();
	void Send(std::string message, int clientSocket);
	void SendEveryone(std::string message);
	void SendEveryoneExceptEmissary(std::string message, SOCKET emissary);
	bool Init();
	void Run();
};