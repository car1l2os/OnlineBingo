// VisualClient.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "VisualClient.h"
#include "Client.h"
#include "Partida.h"
#include <set>
#include <algorithm>
#include <memory>


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
std::unique_ptr<Client> client;
std::unique_ptr<Partida<4, 1, 99, 3, 5>> partida;
HBRUSH hBrushLabel;

std::set<int>* windowsToChangeColorToEnable;
std::set<int>* windowsToChangeColorToDisable;
HMENU IDToAssign = 0;
HDC hdc;
int ctrlID = 5;

//Visual variables
HWND Wpoints, WcountDown, WlastBall, Wresults;
std::array<std::array<std::array<std::shared_ptr<Casilla>, 5>, 3>, 4> WcasillasParaCartones; //primera aproximación array multidimensional pero eso provoca más trabajo en las inyecciones de después
std::array<HWND, 4> Wbuttons;
std::array<HWND, 5> WinputInterface;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void SetTextInWindow(HWND win, std::string text);

void AddText(HWND hWnd);
void AddBuyButtons(HWND hWnd);
void AddCartonesSlots(HWND hWnd);
void ProcessServerOutput(std::string toProcess);
void HideShowButtons(bool show);
void InputInterface(HWND hWnd);
void StartGameInterface(HWND hWnd);


//definiciones
#define UN_CARTONES 10;
#define DOS_CARTONES 11;
#define TRES_CARTONES 12;
#define CUATRO_CARTONES 13;

#define ENABLED_COLOR_BG RGB(50, 50, 50);
#define DISABLED_COLOR_BG RGB(150, 150, 150)


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//Eneña consola en win32 app
	/*AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);*/

	// TODO: Place code here.
	windowsToChangeColorToEnable = new std::set<int>();
	windowsToChangeColorToDisable = new std::set<int>();

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_VISUALCLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VISUALCLIENT));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VISUALCLIENT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_VISUALCLIENT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}


void CALLBACK CheckServer()
{
	ProcessServerOutput(client->ReceiveFromServer());
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	//cambiados valores para hacer más pequeña la ventana
	/*
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
		*/
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 700, 700, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}


	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case 1:
		case 2:
		case 3:
		case 4:
			partida->StartNewGame(wParam);
			partida->SetGoingToPlayNextGame(true);
			client->SendToServer("#PLY");
			SetTextInWindow(Wpoints, std::to_string(partida->points));
			HideShowButtons(false);
			break;

		case 10:
			StartGameInterface(hWnd);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_CREATE:
		
		InputInterface(hWnd);

		break;
	case WM_CTLCOLORSTATIC:
		ctrlID = GetDlgCtrlID((HWND)lParam);
		if (std::binary_search(windowsToChangeColorToEnable->begin(), windowsToChangeColorToEnable->end(), ctrlID))
		{
			windowsToChangeColorToEnable->erase(ctrlID);

			hdc = reinterpret_cast<HDC>(wParam);
			SetBkColor(hdc, RGB(20, 150, 100));
			if (!hBrushLabel) hBrushLabel = CreateSolidBrush(RGB(20, 150, 100));
			return reinterpret_cast<LRESULT>(hBrushLabel);
		}

		else if (std::binary_search(windowsToChangeColorToDisable->begin(), windowsToChangeColorToDisable->end(), ctrlID))
		{
			windowsToChangeColorToDisable->erase(ctrlID);

			hdc = reinterpret_cast<HDC>(wParam);
			SetBkColor(hdc, RGB(255, 150, 100));
			if (!hBrushLabel) hBrushLabel = CreateSolidBrush(RGB(255, 150, 100));
			return reinterpret_cast<LRESULT>(hBrushLabel);
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		if (hBrushLabel) DeleteObject(hBrushLabel);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void InputInterface(HWND hWnd)
{
	WinputInterface[0] = CreateWindowW(L"static", L"IP para conectar:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 10, 40, 250, 25, hWnd, IDToAssign, NULL, NULL);
	WinputInterface[1] = CreateWindowW(L"EDIT", L"", WS_BORDER | WS_VISIBLE | WS_CHILD, 10, 80, 400, 25, hWnd, NULL, NULL, NULL);
	WinputInterface[2] = CreateWindowW(L"static", L"Puerto en el que conectar:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 10, 120, 250, 25, hWnd, IDToAssign, NULL, NULL);
	WinputInterface[3] = CreateWindowW(L"EDIT", L"", WS_BORDER | WS_VISIBLE | WS_CHILD, 10, 160, 400, 25, hWnd, NULL, NULL, NULL);
	WinputInterface[4] = CreateWindowW(L"Button", L"Conectar", WS_BORDER | WS_VISIBLE | WS_CHILD, 10, 200, 400, 25, hWnd, (HMENU)10, NULL, NULL);
}


void AddBuyButtons(HWND hWnd)
{
	//se podría hacer con un vector o algo así para tenerlo en general pero para la interfaz vamos a aprovechar los valores seguros que tenemos 
	Wbuttons[0] = CreateWindowW(L"Button", L"1", WS_VISIBLE | WS_CHILD, 210, 40, 50, 50, hWnd, (HMENU)1, NULL, NULL);
	Wbuttons[1] = CreateWindowW(L"Button", L"2", WS_VISIBLE | WS_CHILD, 275, 40, 50, 50, hWnd, (HMENU)2, NULL, NULL);
	Wbuttons[2] = CreateWindowW(L"Button", L"3", WS_VISIBLE | WS_CHILD, 340, 40, 50, 50, hWnd, (HMENU)3, NULL, NULL);
	Wbuttons[3] = CreateWindowW(L"Button", L"4", WS_VISIBLE | WS_CHILD, 410, 40, 50, 50, hWnd, (HMENU)4, NULL, NULL);
}

void AddText(HWND hWnd)
{
	CreateWindowW(L"static", L"Puntos", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 10, 10, 100, 25, hWnd, IDToAssign, NULL, NULL);
	IDToAssign++;
	Wpoints = CreateWindowW(L"static", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 115, 10, 50, 25, hWnd, IDToAssign, NULL, NULL);
	IDToAssign++;

	CreateWindowW(L"static", L"Segundos para comprar:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 200, 10, 200, 25, hWnd, IDToAssign, NULL, NULL);
	IDToAssign++;
	WcountDown = CreateWindowW(L"static", L"60", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 400, 10, 50, 25, hWnd, IDToAssign, NULL, NULL);
	IDToAssign++;

	CreateWindowW(L"static", L"Ultima bola:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 10, 40, 100, 25, hWnd, IDToAssign, NULL, NULL);
	IDToAssign++;
	WlastBall = CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 115, 40, 50, 50, hWnd, IDToAssign, NULL, NULL);
	IDToAssign++;

	CreateWindowW(L"static", L"Ultimo evento:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 10, 125, 100, 25, hWnd, IDToAssign, NULL, NULL);
	IDToAssign++;
	Wresults = CreateWindowW(L"static", L" ", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 120, 125, 400, 25, hWnd, IDToAssign, NULL, NULL);
	IDToAssign++;
}

void StartGameInterface(HWND hWnd)
{
	for(int i=0;i<5;i++)
		ShowWindow(WinputInterface[i], SW_HIDE);

	wchar_t ipToConvert[20];
	GetWindowTextW(WinputInterface[1], ipToConvert, 20);
	wchar_t portToConvert[10];
	GetWindowTextW(WinputInterface[3], portToConvert, 10);

	std::wstring wsIP(ipToConvert);
	std::string ip(wsIP.begin(), wsIP.end());

	std::wstring wsPort(portToConvert);
	std::string ipString(wsPort.begin(), wsPort.end());
	int port = std::stoi(ipString);

	client = std::make_unique<Client>(ip, port);
	client->Init();
	SetTimer(NULL, 0, 1000, (TIMERPROC)&CheckServer);
	hBrushLabel = NULL;
	AddBuyButtons(hWnd);
	AddText(hWnd);
	AddCartonesSlots(hWnd);
}

void AddCartonesSlots(HWND hWnd)
{
	for (int j = 0;j < 4;j++) //cartones
	{
		for (int i = 0;i < 15;i++) //casillas por carton
		{
			HWND tmp = CreateWindowW(L"static", L"1", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 10 + (i % 5 * 52) + (300 * (j % 2)), 200 + (i / 5 * 52) + (200 * (j / 2)), 50, 50, hWnd, IDToAssign, NULL, NULL);
			WcasillasParaCartones[j][i / 5][i % 5] = std::make_shared<Casilla>(tmp, windowsToChangeColorToEnable, windowsToChangeColorToDisable);
			IDToAssign++;
		}
	}
	partida = std::make_unique<Partida<4, 1, 99, 3, 5>>();
	partida->InjectCasillas(WcasillasParaCartones);
}

void ProcessServerOutput(std::string toProcess)
{
	if (client->ValidSocket())
	{
		char type = toProcess[0];
		std::string order = toProcess.substr(1, 3);

		if (type == '!') //orden
		{
			if (order == "STR")
			{
				partida->SetReady();
				//desabilita botones si no se ha comprado 
				HideShowButtons(false);
				SetTextInWindow(Wresults, "La partida empieza");
				if (!partida->GetGoingToPlayNextGame())
					partida->StartNewGame(0);
			}
			else if (order == "FIN")
			{
				//habilita botones 
				partida->SetReady();
				SetTextInWindow(Wresults, "Puedes comprar ahora");
				HideShowButtons(true);
				partida->SetGoingToPlayNextGame(false);
			}
		}
		else if (type == '$')
		{
			std::string toSet = "";
			std::string player = toProcess.substr(3, toProcess.size());
			if (order == "LIN") //linea
			{
				toSet.append("Otro jugador ha hecho linea");
				partida->lastPrize = LINEA;
			}
			else if (order == "BIN") //bingo
			{
				toSet.append("Otro jugador ha hecho bingo");
				partida->lastPrize = BINGO;
				HideShowButtons(true);
			}
			SetTextInWindow(Wresults, toSet);
		}
		else if (type == '#')
		{
			if (order == "TIM")
			{
				partida->SetReady();
				SetTextInWindow(WcountDown, std::string(toProcess.begin() + 4, toProcess.end()));
			}
			if (order == "NUM" && partida->Ready())
			{
				SetTextInWindow(WlastBall, std::string(toProcess.begin() + 4, toProcess.end()));
				int result = partida->InsertNumber(std::stoi(toProcess.substr(4, 5)));

				if (result == LINEA)
				{
					client->SendToServer("$LIN");
					SetTextInWindow(Wpoints, std::to_string(partida->points));
					SetTextInWindow(Wresults, "Has hecho linea");
				}
				else if (result == BINGO)
				{
					client->SendToServer("$BIN");
					SetTextInWindow(Wpoints, std::to_string(partida->points));
					HideShowButtons(true);
					SetTextInWindow(Wresults, "Has hecho bingo");
					partida->SetGoingToPlayNextGame(false);
				}

			}
			else if (!partida->Ready())
			{
				HideShowButtons(false);
				SetTextInWindow(Wresults, "Se está jugando otra partida. Espera a que termine");
			}
		}
	}
}


void HideShowButtons(bool show)
{
	if (show)
	{
		for (int i = 0;i < Wbuttons.size();i++)
			ShowWindow(Wbuttons[i], SW_SHOW);
	}
	else
	{
		for (int i = 0;i < Wbuttons.size();i++)
			ShowWindow(Wbuttons[i], SW_HIDE);
	}
}

void SetTextInWindow(HWND wnd, std::string text)
{
	std::wstring stemp = std::wstring(text.begin(), text.end());
	LPCWSTR sw = stemp.c_str();
	SetWindowText(wnd, sw);
}