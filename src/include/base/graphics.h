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

#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 800

// initialize graphics handling 
void init()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	HWND consoleWindow = GetConsoleWindow();
	
	// disable minimizing and maximizing screen
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
	
	// set window size 
	RECT rect = {100, 100, WINDOW_WIDTH, WINDOW_HEIGHT};
	MoveWindow(consoleWindow, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top,TRUE);
	
	// disable scroll wheel by setting screen buffer size	
    CONSOLE_SCREEN_BUFFER_INFO SBInfo;
    COORD new_screen_buffer_size;
	GetConsoleScreenBufferInfo(hConsole, &SBInfo);
	
	new_screen_buffer_size.X = SBInfo.srWindow.Right - SBInfo.srWindow.Left + 1;
	new_screen_buffer_size.Y = SBInfo.srWindow.Bottom - SBInfo.srWindow.Top + 1;
	
	SetConsoleScreenBufferSize(hConsole, new_screen_buffer_size);
	
	//hide cursor 
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 1;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &info);
	
	// load libraries 
	LoadLibrary("./libfreetype-6.dll");
	LoadLibrary("./libgcc_s_dw2-1.dll");
	LoadLibrary("./libgcc_s_sjlj-1.dll");
	LoadLibrary("./libjpeg-9.dll");
	LoadLibrary("./libpng16-16.dll");
	LoadLibrary("./libstdc++-6.dll");
	LoadLibrary("./libtiff-5.dll");
	LoadLibrary("./libwebp-7.dll");
	LoadLibrary("./libwinpthread-1.dll");
	LoadLibrary("./zlib1.dll");
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
	int color;
	
	for(y = 0;y<maxY;y++)
	{
		for(x = 0;x<maxX;x++)
		{
			switch(patternType)
			{
				case BACK_ROOM:
				color = backRoom[y][x];
				break;
				
				case LIBRARY:
				color = library[y][x];
				break;
				
				case FLAMES:
				color = flames[y][x];
				break;
				
				case TITLE:
				color = title[y][x];	
				break;
				
				case MAIN_CAMP:
				color = mainCamp[y][x];
				break;
				
				case SHOP:
				color = shop[y][x];
				break;
				
				case PUB:
				color = pub[y][x];
				break;
				
				case GARLAND:
				color = garlan[y][x];
				break;
				
				case M1:
				color = m1[y][x];	
				break;
				
				case M2:
				color = m2[y][x];	
				break;
				
				case M3:
				color = m3[y][x];	
				break;
				
				case M4:
				color = m4[y][x];	
				break;
				
				case M5:
				color = m5[y][x];	
				break;
				
				case M6:
				color = m6[y][x];	
				break;
				
				case M7:
				color = m7[y][x];	
				break;
				
				case M8:
				color = m8[y][x];	
				break;
				
				case M9:
				color = m9[y][x];	
				break;
				
				case M10:
				color = m10[y][x];	
				break;
				
				case M12:
				color = m12[y][x];	
				break;
				
				case M13:
				color = m13[y][x];	
				break;
				
				case M14:
				color = m14[y][x];	
				break;
				
				case M15:
				color = m15[y][x];	
				break;
				
				case M16:
				color = m16[y][x];	
				break;
				
				case M17:
				color = m17[y][x];	
				break;
				
				case SWORD_SPRITE:
				color = swordr[y][x];	
				break;
				
				case LANCE_SPRITE:
				color = lancer[y][x];	
				break;
				
				case AXE_SPRITE:
				color = axer[y][x];	
				break;
				
				case GUN_SPRITE:
				color = gunr[y][x];	
				break;
				
				case BOW_SPRITE:
				color = bowr[y][x];	
				break;
				
				case COMB_SPRITE:
				color = combr[y][x];	
				break;
			}
			if(color != 0)
			{
				setColor(color);
				setCursor(xPos+x,yPos+y);
				printf("%c",219);
			}
		}
	}
	setColor(WHITE);
	printf("\n");
}

// display borders used in game 
void borders()
{
	int i = 0;
	for(i = 21;i<41;i++)
	{
		// for text description 
		setCursor(109,i);
		printf("|");
	}
}

#endif