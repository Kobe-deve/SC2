// general functions used in the game code 
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

// ascii color handling 
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

// displaying errors 
void throwError(char * errorText)
{		
	HWND consoleWindow = GetConsoleWindow();
	
	// make sure command prompt is shown 
	ShowWindow(consoleWindow, SW_NORMAL);
	
	printf("%s",errorText);
	exit(0);
}

// set cursor at a position on the screen 
void setCursor(int x, int y) // sets the cursor to a position on the screen 
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD p = {x,y};
    SetConsoleCursorPosition(h,p);
}

// set color based on a given color value 
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
