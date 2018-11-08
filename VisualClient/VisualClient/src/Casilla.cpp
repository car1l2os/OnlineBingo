#include "stdafx.h"
#include "Casilla.h"

Casilla::Casilla(HWND window, std::set<int>* stackToEnable, std::set<int>* stackToDisable)
{
	m_window = window;
	m_wID = GetDlgCtrlID(window);
	m_setToEnable = stackToEnable;
	m_setToDisable = stackToDisable;
}

DWORD WINAPI Casilla::Enable()
{
	InvalidateRect(m_window, NULL, TRUE);
	m_setToEnable->insert(m_wID);
	return 0;
}

DWORD WINAPI Casilla::Disable()
{
	InvalidateRect(m_window, NULL, TRUE);
	m_setToDisable->insert(m_wID);
	return 0;
}

bool Casilla::Show()
{
	return ShowWindow(m_window, SW_SHOW);
}

bool Casilla::Hide()
{
	return 	ShowWindow(m_window, SW_HIDE);
}

void Casilla::SetTextInCasilla(std::string text)
{
	std::wstring stemp = std::wstring(text.begin(),text.end());
	LPCWSTR sw = stemp.c_str();
	SetWindowText(m_window,sw);
	if (text == std::to_string(NO_NUMBER))
		Hide();
	else
		Show();
}




/*
//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
std::string Casilla::GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::string(); //No error message has been recorded

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}
*/