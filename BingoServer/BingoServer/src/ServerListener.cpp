#include "ServerListener.h"

ServerListener::ServerListener(std::string ip, int port, MessageHandler handler)
	: m_ip(ip), m_port(port), messageReceived(handler)
{

}

ServerListener::~ServerListener()
{
	CleanUp();
}

bool ServerListener::LoadWinsock()
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

std::string ServerListener::GeyMyIP()
{
	std::string line;
	std::ifstream IPFile;
	int offset;
	std::string search0 = "Direcci¢n IPv4. . . . . . . . . . . . . . :";
	system("ipconfig > ip.txt");
	IPFile.open("ip.txt");
	if (IPFile.is_open())
	{
		while (IPFile.eof() == false)
		{
			std::getline(IPFile, line);
			if ((offset = line.find(search0)) != std::string::npos)
			{
				line.erase(0, 39);
				std::cout << "IP ADDRESS: " << line << std::endl;
			}
		}
	}
	return line;
}

bool ServerListener::CreateAndBindSocket()
{
	m_listening = socket(AF_INET, SOCK_STREAM, 0);
	if (m_listening != INVALID_SOCKET)
	{
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(m_port);
		inet_pton(AF_INET, m_ip.c_str(), &hint.sin_addr);

		int bindOK = bind(m_listening, (sockaddr*)&hint, sizeof(hint));
		if (bindOK != SOCKET_ERROR)
		{
			int listenOK = listen(m_listening, SOMAXCONN);
			if (listenOK == SOCKET_ERROR)
				return false;
		}
		else
			if (m_listening == INVALID_SOCKET)
				return false;
		return true;
	}
	return false;
}

SOCKET ServerListener::WaitForConnection(SOCKET listening)
{
	SOCKET client = accept(listening, NULL, NULL);
	return client;
}

void ServerListener::SetSet()
{
	FD_ZERO(&master);
	// Add our first socket that we're interested in interacting with; the listening socket!
	// It's important that this socket is added for our server or else we won't 'hear' incoming
	// connections 
	FD_SET(m_listening, &master);
}

void ServerListener::Send(std::string message, int clientSocket)
{
	send(clientSocket, message.c_str(), message.size() + 1, 0);
}

void ServerListener::SendEveryone(std::string message)
{
	std::ostringstream ss;
	ss << message << "\r\n";
	std::string strOut = ss.str();

	// Send message to other clients, and definiately NOT the listening socket
	for (int i = 0; i < master.fd_count; i++)
	{
		SOCKET outSock = master.fd_array[i];
		if (outSock != m_listening)
		{
			send(outSock, strOut.c_str(), strOut.size() + 1, 0);
		}
	}
}

void ServerListener::SendEveryoneExceptEmissary(std::string message, SOCKET emissary)
{
	// Send message to other clients, buty NOT the listening socket
	for (int i = 0; i < master.fd_count; i++)
	{
		SOCKET outSock = master.fd_array[i];
		if (outSock != m_listening && outSock != emissary)
		{
			send(outSock, message.c_str(), message.size() + 1, 0); //re enviamos a los demás porque ha entrado una linea o un bingo
		}
	}
}


bool ServerListener::Init()
{
	if (LoadWinsock())
		if (CreateAndBindSocket())
			SetSet();
	return true;

	return false;
}

void ServerListener::Run()
{
	char buffer[MAX_BUFFER_SIZE];
	fd_set copy;
	SOCKET sock;
	bool running = true;
	gameStateMachine.ChangeState(GameState::Buying);

	struct timeval timeBetweenInputCheck;
	timeBetweenInputCheck.tv_sec = 0;
	timeBetweenInputCheck.tv_usec = 500000;

	while (running)
	{
		copy = master;
		// See who's talking to us
		int socketCount = select(0, &copy, nullptr, nullptr, &timeBetweenInputCheck);
		std::string to_send = gameStateMachine.Update();
		std::cout << to_send << std::endl;
		if (to_send != "")
			SendEveryone(to_send);

		// Loop through all the current connections / potential connect
		for (int i = 0; i < socketCount; i++)
		{
			// Makes things easy for us doing this assignment
			SOCKET sock = copy.fd_array[i];

			// Is it an inbound communication?
			if (sock == m_listening)
			{
				// Accept a new connection
				SOCKET client = accept(m_listening, nullptr, nullptr);

				// Add the new connection to the list of connected clients
				FD_SET(client, &master);

				std::cout << "Someone connected" << std::endl;
			}
			else // It's an inbound message
			{
				char buf[100];
				ZeroMemory(buf, 100);

				// Receive message
				int bytesIn = recv(sock, buf, 100, 0);
				//std::cout << "SOCKET #" << sock << ": " << buf << "\r\n";
				std::cout << buf << std::endl;

				if (bytesIn > 0)
				{
					std::ostringstream ss;
					ss << buf << sock;
					std::string strOut = ss.str();

					if (!gameStateMachine.HasPlayers())
					{
						if (strOut.find("PLY") != std::string::npos)
						{
							gameStateMachine.PlayerInGame();
						}
					}
					else
					{
						if (strOut.find("LIN") != std::string::npos) 
						{
							SendEveryoneExceptEmissary("$LIN", sock);
						}
						else if (strOut.find("BIN") != std::string::npos) //if BIN substring de lo recibido --> bingo de alguien --> paramos el juego y reseteamos
						{
							SendEveryoneExceptEmissary(gameStateMachine.ChangeState(GameState::Buying), sock);
						}
						else
						{
							SendEveryoneExceptEmissary(strOut, sock);
						}
					}
				}
				else
				{
					// Drop the client
					closesocket(sock);
					FD_CLR(sock, &master);
					gameStateMachine.PlayerLeavesGame();
					if (gameStateMachine.HasPlayers() == false)
					{
						gameStateMachine.ChangeState(GameState::Buying);
					}
				}
			}
		}
	}
}

void ServerListener::CleanUp()
{
	WSACleanup();
}
