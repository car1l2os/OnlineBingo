#pragma once
#include <set>
#include <string>
#define NO_NUMBER -1

class Casilla
{
private:
	HWND m_window;
	DWORD m_wID;
	std::set<int>* m_setToEnable;
	std::set<int>* m_setToDisable;

	std::string GetLastErrorAsString();

public:
	Casilla(HWND window, std::set<int>* stackToEnable, std::set<int>* stackToDisable);
	DWORD WINAPI Enable();
	DWORD WINAPI Disable();
	bool Show();
	bool Hide();
	void SetTextInCasilla(std::string text);
};

