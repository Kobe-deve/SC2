#ifndef GRAPHICS
#define GRAPHICS

#ifndef TEXT_DEFINED
#include "text.h"
#endif

#ifndef SPRITE_DATA
#include "sprite_data.h"
#endif

#include <conio.h>
#include <windows.h>

// will be used for toggling between ascii/sprite modes 
// 0 - ascii
// 1 - sprites 
int graphicsMode = 1;

#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED

// main SDL2 handler
#include <SDL2/SDL.h>

// handles pngs with SDL2
#include <SDL2/SDL_image.h>

// handles music with SDL2
#include <SDL2/SDL_mixer.h>

// font handling with SDL2 
#include <SDL2/SDL_ttf.h>

#endif

#ifndef MUSIC_HANDLED
#include "music.h"
#endif

#ifndef FONT_HANDLED
#include "font.h"
#endif

#ifndef IMAGE_HANDLED
#include "image.h"
#endif

// frame handling variables

// max FPS constant 
int SCREEN_FPS = 30; 
// calculates ticks per frame for timers
int SCREEN_TICK_PER_FRAME;  // 1000 / SCREEN_FPS

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

#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 800

void initFont(struct text * t, SDL_Renderer * r);
				
// initialize graphics handling 
void init(struct gameState * s)
{
	SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;
	
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	HWND consoleWindow = GetConsoleWindow();
	
	if(graphicsMode == 0) // initialize ascii mode
	{
		#define ASCII_FUNCT
		
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
	}
	else if(graphicsMode == 1) // initialize sprite mode 
	{
		#define SPRITE_FUNCT

		// set frame handling variables
		SCREEN_FPS = 40; 
		SCREEN_TICK_PER_FRAME = 25;  // 1000 / SCREEN_FPS

		// hide console window 
		if(testMode == 0)
			ShowWindow(consoleWindow, SW_HIDE);
		
		// initialize window and SDL handling
		if(SDL_Init( IMG_INIT_JPG | IMG_INIT_PNG | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) >= 0)
		{
			//set texture filtering to linear
			//sets render quality to where images aren't blurry when resized
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
				
			// create window 
			s->window = SDL_CreateWindow( GAME_WINDOW_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH-100, WINDOW_HEIGHT-100, SDL_WINDOW_SHOWN);
			if(s->window != NULL)
			{
				// creates renderer
				s->renderer = SDL_CreateRenderer(s->window, -1, SDL_RENDERER_ACCELERATED); 	
					
				//Initialize renderer draw color
				SDL_SetRenderDrawColor(s->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
						
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ))
					throwError("SDL_image could not initialize! SDL_image");
				
				//Initialize text handling 
				if( TTF_Init() == -1 )
					throwError("SDL_ttf could not initialize!");
				
				//Initialize music handling 
				if( Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024*2 ) < 0 )
					throwError("SDL_mixer could not initialize!");
				
			}
		}
		
		// initialize array of displayed images 
		s->images = NULL;
		s->imageStackSize = 0;
		s->numImages = 0;
	}
	
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

// ascii graphics handling functions 

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
			// handle background color for certain areas 
			if(color == 0 && patternType == SHOP)
			{
				setColor(DARK_BABY_BLUE);
				setCursor(xPos+x,yPos+y);
				printf("%c",219);
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


// sprite graphics handling functions 

#endif