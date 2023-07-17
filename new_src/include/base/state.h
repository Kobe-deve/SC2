// for defining/handling the state data of the program

#include "information/text.h"
#include "information/filenames.h"

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

#include "graphics/font.h"
#include "graphics/image.h"
#include "information/stats.h"
#include "information/npc.h"

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
	
	// game system enumeration used 
	enum gameSystem
	{
		TITLE_SCREEN = 1,
		DUNGEON_SCREEN = 2,
		BATTLE_SCREEN = 3,
		SHOP_SCREEN = 4,
		GAMEOVER_SCREEN = 5,
		CUTSCENE_SCREEN = 6,
		MENU_SCREEN = 7,
	};

	// data structure for handling the state/data of the game 
	struct gameState
	{
		int testMode; // for testing the game 
		
		int graphicsMode; // the graphics mode of the game (0 - ascii 1 - sprite)
		int input; // input variable used 
		int gameSystem; // the current game system that is running 
		int switchTo; // system to switch to
		int switchSystem; // boolean variable for switching (0 - don't switch, 1 - switch)

		Mix_Music * music; // the music playing
		char * track; // the song that is playing
	
		// for rendering in sprite mode 
		SDL_Window* window; // window 
		SDL_Renderer* renderer; // window renderer
		struct text * fontHandler; // handles text display
		
		// used for window handling 
		SDL_Event * e;
		const Uint8* keyStates;
		int frameRateTracker; // used for frame rate 
		int colors[4]; // renderer color 
		int megaAlpha; // graphic transition variable used in sprite mode (used in ascii for when a new screen is displayed)
		int fadeIn; // used in graphics mode for alpha changing
		
		// images used in sprite mode 
		struct image * images; // displayed images used in sprite mode
		int imageStackSize; // number of images in array 
		int numImages; // max size of array 
		
		// background asset used for sprite mode 
		struct image backgroundAsset;
		
		// dungeon crawling variables
		int playerX; // x coord in dungeon 
		int playerY; // y coord in dungeon 
		int floor; // floor in dungeon 
		int building; // the building the player is in 
		int direction; // the direction the player is facing in the dungeon 
		
		// npc information for dungeon 
		int numNPCs;
		struct npc * activeNPCs;
		int nearestNPC; // the npc the player is closest to
		
		// array of visible tiles in an area
		int *** visible;

		// width/height of the current dungeon
		int dungeonSize;

		// enemy movement information for dungeon 
		int numEnemies;
		struct enemies * activeEnemies;

		// status text lines 
		int maxStatus; // max number of status lines visible 
		char ** statusText;
		int numStatusLines;

		// is the player going to sleep?
		int sleeping;
		int tired;

		// dungeon array pointer 
		int *** d;

		// coordinates of stairs going up for AI 
		int ** upStairCoords;
		
		// for sprite rendering a section of a spirte sheet 
		SDL_Rect spriteClip; 	
		
		// menu variables
		int option; // menu option 
		char ** options; // strings for options 
		int numOptions; // number of options 
		int menuX, menuY; // menu option coordinates
		
	};
	
	
	// frame handling variables

	// max FPS constant 
	int SCREEN_FPS = 30; 
	// calculates ticks per frame for timers
	int SCREEN_TICK_PER_FRAME;  // 1000 / SCREEN_FPS
	
	// initialize the game state 
	void init(struct gameState * state)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		HWND consoleWindow = GetConsoleWindow();
	
		// make sure test mode is off
		state->testMode = 0;
	
		// initialize state variables
		state->input = 0;
		
		// initialize music 
		state->music = NULL;	
		state->track = NULL;
		
		// if sprite mode, initialize graphics/music
		if(state->graphicsMode == 1)
		{
			// set frame handling variables
			SCREEN_FPS = 40; 
			SCREEN_TICK_PER_FRAME = 25;  // 1000 / SCREEN_FPS

			// hide console window 
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

			state->megaAlpha = 255;
			state->spriteClip = (SDL_Rect){0,0,SPRITE_SQUARE_SIZE,SPRITE_SQUARE_SIZE};
			
			// initialize text handler
			state->fontHandler = malloc(sizeof(struct text));
			initFont(state->fontHandler, state->renderer);
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
			
			state->megaAlpha = 0;
		}
		
		// initialize fade in display variable 
		state->fadeIn = 0;
		
		state->switchSystem = 0;
		
		// initialize menu options
		state->option = 0; // menu option 
		state->options = NULL; // strings for options 
		state->numOptions = 0; // number of options 
		state->menuX = 0; 
		state->menuY = 0; // menu option coordinates
		state->nearestNPC = 0;
		
		// initialize array of displayed images 
		state->images = NULL;
		state->imageStackSize = 0;
		state->numImages = 0;
		
		// set up current game system 
		state->gameSystem = TITLE_SCREEN;
		
		// set up dungeon crawling variables 
		state->playerX = 0; // x coord in dungeon 
		state->playerY = 0; // y coord in dungeon 
		state->floor = 0; // floor in dungeon
		state->building = 0;
		state->direction = 0;
		state->numNPCs = 0;
		state->activeNPCs = NULL;	
		
		state->visible = NULL;

		state->dungeonSize = 0;

		state->numEnemies = 0;
		state->activeEnemies = NULL;

		state->maxStatus = 10; // max number of status lines visible 
		state->statusText = NULL;
		state->numStatusLines = 0;

		state->sleeping = 0;
		state->tired = 0;

		state->d = NULL;

		state->upStairCoords = NULL;
		
	}

	// add to images used in current state 
	void addImage(struct gameState * s,char * filePath)
	{
		if(s->imageStackSize == 0)
		{
			s->images = malloc(sizeof(struct image));
			s->numImages = 1;
			s->images[s->imageStackSize] = initImage(filePath, s->renderer);
		}
		else if(s->imageStackSize < s->numImages)
		{
			s->images[s->imageStackSize] = initImage(filePath, s->renderer);
			s->imageStackSize++;
		}
	}
	
	// sets the alpha of all images/fonts 
	void setAlphaOfImages(struct gameState * state)
	{
		int i;
		
		// set alpha of all images used 
		for(i=0;i<state->numImages;i++)
		{
			SDL_SetTextureBlendMode(state->images[i].texture, SDL_BLENDMODE_BLEND);
			SDL_SetTextureAlphaMod(state->images[i].texture, state->megaAlpha);
		}
		
		// set alpha of text 
		state->fontHandler->textColor.a = state->megaAlpha;
	}
	
	// clear image array and free up memory
	void clearImages(struct gameState * s)
	{
		int i;
		
		for(i=0;i<s->numImages;i++)
		{
			deallocateImage(&s->images[i]);
		}
		
		s->images = NULL;
		
		s->imageStackSize = 0;
		s->numImages = 0;
	}

	// deallocate the game state 
	void deallocate(struct gameState * state)
	{
		// deallocate data used for sprite mode 
		if(state->graphicsMode == 1)
		{
			
			// deallocate images
			clearImages(state);
			/*
			// deallocate background image
			deallocateImage(&backgroundAsset);
			*/
			// deallocate font 
			deallocateFont(state->fontHandler);
			
			
			// deallocate renderer and window 
			SDL_DestroyRenderer(state->renderer);
			SDL_DestroyWindow(state->window);
			
			// quit sdl
			SDL_Quit();
		}
	}
	
#endif
