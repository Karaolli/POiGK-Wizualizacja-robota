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
	int   GetWidth()       const { return width; }
	int   GetHeight()      const { return height; }
	float GetAspectRatio() const { 
		return (float)width / (float)height; }
private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	HWND hwnd;
	int width, height;
	LPCWSTR className = L"MainWindow";
};