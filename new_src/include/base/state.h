// for defining/handling the state data of the program

#ifndef TEXT_HANDLED
#include "information/text.h"
#endif 

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

// screen dimensions in ascii mode 
#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 800

#ifndef STATE_HANDLED
#define STATE_HANDLED

	// input handling enumerations 
	enum input
	{
		UP = 72,
		DOWN = 80,
		LEFT = 75,
		RIGHT = 77,
		ENTER = 13,
		ESC = 27,
		MENU = 109, 
		BACKSPACE = 8
	};

	// data structure for handling the state/data of the game 
	struct gameState
	{
		int graphicsMode; // the graphics mode of the game (0 - ascii 1 - sprite)
		int input;
	
		// for rendering in sprite mode 
		SDL_Window* window; // window 
		SDL_Renderer* renderer; // window renderer
		
		// used for window handling 
		SDL_Event * e;
		const Uint8* keyStates;
		int frameRateTracker; // used for frame rate 
		int colors[4]; // renderer color 
		//struct image backgroundAsset;
	};
	
	// frame handling variables

	// max FPS constant 
	int SCREEN_FPS = 30; 
	// calculates ticks per frame for timers
	int SCREEN_TICK_PER_FRAME;  // 1000 / SCREEN_FPS

	// displaying error 
	void throwError(char * errorText)
	{		
		HWND consoleWindow = GetConsoleWindow();
		
		// make sure command prompt is shown 
		ShowWindow(consoleWindow, SW_NORMAL);
		
		printf("%s",errorText);
		exit(0);
	}
	
	// initialize the game state 
	void init(struct gameState * state)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		HWND consoleWindow = GetConsoleWindow();
	
		// initialize state variables
		state->input = 0;
		
		// initialize SDL2 for music 
		
		// if sprite mode, initialize graphics/music
		if(state->graphicsMode == 1)
		{
			// set frame handling variables
			SCREEN_FPS = 40; 
			SCREEN_TICK_PER_FRAME = 25;  // 1000 / SCREEN_FPS

			// hide console window 
			//if(testMode == 0)
			ShowWindow(consoleWindow, SW_HIDE);
			
			// initialize window and SDL handling
			if(SDL_Init( IMG_INIT_JPG | IMG_INIT_PNG | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) >= 0)
			{
				//set texture filtering to linear
				//sets render quality to where images aren't blurry when resized
				SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
					
				// create window 
				state->window = SDL_CreateWindow( GAME_WINDOW_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH-100, WINDOW_HEIGHT-100, SDL_WINDOW_SHOWN);
				if(state->window != NULL)
				{
					// creates renderer
					state->renderer = SDL_CreateRenderer(state->window, -1, SDL_RENDERER_ACCELERATED); 	
						
					//Initialize renderer draw color
					SDL_SetRenderDrawColor(state->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
							
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
		}
		else // if ascii, set screen information
		{
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
	}
	
	// deallocate the game state 
	void deallocate(struct gameState * state)
	{
	}
	

#endif
