#ifndef STATE_HANDLED
#include "state.h"
#endif

#ifndef GRAPHICS
#define GRAPHICS

#ifndef SPRITE_DATA
#include "sprite_data.h"
#endif

#include <conio.h>
#include <windows.h>

// calculates ticks per frame for timers
int SCREEN_TICK_PER_FRAME = 8; 

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
	
	SetConsoleScreenBufferSize(hConsole, new_screen_buffer_size);
	ShowWindow(consoleWindow,SW_SHOWMAXIMIZED);
	
	//hide cursor 
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 1;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &info);
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

// set cursor at a position on the screen 
void setCursor(int x, int y) // sets the cursor to a position on the screen 
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD p = {x,y};
    SetConsoleCursorPosition(h,p);
}

// print specific array patterns 
void printPattern(enum sprites patternType, int xPos, int yPos, int maxX, int maxY)
{
	int x;
	int y;
	for(y = 0;y<maxY;y++)
	{
		for(x = 0;x<maxX;x++)
		{
			switch(patternType)
			{
				case BACK_ROOM:
				setColor(backRoom[y][x]);
				break;
				
				case LIBRARY:
				setColor(library[y][x]);
				break;
				
				case FLAMES:
				setColor(flames[y][x]);
				break;
				
				case TITLE:
				setColor(title[y][x]);	
				break;
				
				case MAIN_CAMP:
				setColor(mainCamp[y][x]);
				break;
				
				case SHOP:
				setColor(shop[y][x]);
				break;
				
				case PUB:
				setColor(pub[y][x]);
				break;
				
				case GARLAND:
				setColor(garlan[y][x]);
				break;
				
				case M1:
				setColor(m1[y][x]);	
				break;
				
				case M2:
				setColor(m2[y][x]);	
				break;
				
				case M3:
				setColor(m3[y][x]);	
				break;
				
				case M4:
				setColor(m4[y][x]);	
				break;
				
				case M5:
				setColor(m5[y][x]);	
				break;
				
				case M6:
				setColor(m6[y][x]);	
				break;
				
				case M7:
				setColor(m7[y][x]);	
				break;
				
				case M8:
				setColor(m8[y][x]);	
				break;
				
				case M9:
				setColor(m9[y][x]);	
				break;
				
				case M10:
				setColor(m10[y][x]);	
				break;
				
				case M12:
				setColor(m12[y][x]);	
				break;
				
				case M13:
				setColor(m13[y][x]);	
				break;
				
				case M14:
				setColor(m14[y][x]);	
				break;
				
				case M15:
				setColor(m15[y][x]);	
				break;
				
				case M16:
				setColor(m16[y][x]);	
				break;
				
				case M17:
				setColor(m17[y][x]);	
				break;
				
				case SWORD:
				setColor(swordr[y][x]);	
				break;
				
				case LANCE:
				setColor(lancer[y][x]);	
				break;
				
				case AXE:
				setColor(axer[y][x]);	
				break;
				
				case GUN:
				setColor(gunr[y][x]);	
				break;
				
				case BOW:
				setColor(bowr[y][x]);	
				break;
				
				case COMB:
				setColor(combr[y][x]);	
				break;
			}
			setCursor(xPos+x,yPos+y);
			printf("%c",219);
		}
	}
	setColor(WHITE);
	printf("\n");
}


#endif