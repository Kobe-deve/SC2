#ifndef STATE_HANDLED
#include "state.h"
#endif

#ifndef GRAPHICS
#define GRAPHICS
#include <conio.h>
#include <windows.h>

typedef int colors; // for printing images
#define YELLOW 14
#define SILVER 8
#define GREEN 10
#define PINK 13
#define BABY_BLUE 11
#define BLUE 9
#define DARK_RED 4
#define DARK_BLUE 1
#define DARK_GREEN 2
#define DARK_BABY_BLUE 3
#define DARK_YELLOW 6
#define WHITE 15
#define BLACK 16

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
	
	//hide cursor 
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 1;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &info);
	
	SetConsoleScreenBufferSize(hConsole, new_screen_buffer_size);
	ShowWindow(consoleWindow,SW_SHOWMAXIMIZED);
	
}

int title[10][80] = { 0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					  0,4,0,0,0,4,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					  0,4,0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				      0,0,4,4,4,0,0,0,4,0,0,0,0,4,4,0,0,4,0,4,4,0,0,0,4,4,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					  0,0,0,0,0,4,0,0,4,0,0,0,4,0,0,4,0,4,4,0,0,4,0,4,0,0,4,0,0,0,1,0,0,0,0,0,0,0,1,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,0,0,1,0,1,1,0,0,0,0,0,0,0,
					  0,0,0,0,0,4,0,0,4,0,0,0,4,0,0,4,0,4,0,0,0,4,0,4,4,4,4,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,0,1,0,0,0,1,0,0,1,0,0,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1,1,0,0,1,0,0,0,0,0,0,
					  0,0,4,4,4,0,0,0,0,4,0,0,0,4,4,0,0,4,0,0,0,4,0,4,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,1,0,0,1,0,0,1,0,0,1,0,1,0,0,0,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,
					  0,4,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,4,0,0,0,4,4,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,
				      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0,1,1,1,0,1,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,
					  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0};

void setColor(int ForgC)
{
	WORD wColor;

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

    //We use csbi for the wAttributes word.
	if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
		//Mask out all but the background attribute, and add in the forgournd color
		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F); // 0xF0 is red
		SetConsoleTextAttribute(hStdOut, wColor);
	}
	return;
}

void setCursor(int x, int y) // sets the cursor to a position on the screen 
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD p = {x,y};
    SetConsoleCursorPosition(h,p);
}

void printPattern(int xPos, int yPos, int maxX, int maxY)
{
	int x;
	int y;
	for(y = 0;y<maxY;y++)
	{
		for(x = 0;x<maxX;x++)
		{
			setColor(title[y][x]);	
			setCursor(xPos+x,yPos+y);
			printf("%c",219);
		}
	}
	setColor(WHITE);
	printf("\n");
}


#endif