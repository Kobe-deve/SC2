#ifndef GRAPHICS
#define GRAPHICS
#include <conio.h>
#include <windows.h>

// initialize graphics handling 
void init()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	HWND consoleWindow = GetConsoleWindow();
	
	// disable minimizing and maximizing screen
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
	
	// disable scroll wheel by setting screen buffer size	
    CONSOLE_SCREEN_BUFFER_INFO SBInfo;
    COORD new_screen_buffer_size;
	GetConsoleScreenBufferInfo(hConsole, &SBInfo);
	
	new_screen_buffer_size.X = SBInfo.srWindow.Right - SBInfo.srWindow.Left + 1;
	new_screen_buffer_size.Y = SBInfo.srWindow.Bottom - SBInfo.srWindow.Top + 1;
	
	SetConsoleScreenBufferSize(hConsole, new_screen_buffer_size);
	ShowWindow(consoleWindow,SW_SHOWMAXIMIZED);
	
}

#endif