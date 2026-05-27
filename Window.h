#pragma once

#include <Windows.h>

class App;

class Window {
public:
	Window(int width, int height, LPCWSTR title);
	~Window();
	bool ProcessMessages();

	App* app = nullptr;
	
	HWND  GetHWND()        const { return hwnd; }
	bool  IsMinimized()    const { return IsIconic(hwnd); }
private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	HWND hwnd;
	LPCWSTR className = L"MainWindow";
};