// for defining/handling the state data of the program

#include "information/text.h"
#include "information/filenames.h"
#include "information/quests.h"

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

// default screen dimensions
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
		BACKSPACE = 8,
		ACTIVATE_BATTLE = -1111
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
		SETTINGS_SCREEN = 8,
	};

	// data structure for handling the state/data of the game 
	struct gameState
	{
		// settings data 
		int fullScreen; // is full screen toggled?
		int screenW; // resolution
		int screenH;
		
		int testMode; // for testing the game 
		
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
		int megaAlpha; // graphic transition variable used in sprite mode
		int fadeIn; // used in graphics mode for alpha changing
		
		// images used in sprite mode 
		struct image * images; // displayed images used in sprite mode
		int imageStackSize; // number of images in array 
		int numImages; // max size of array 
		
		// background asset used for sprite mode 
		struct image backgroundAsset;
		
		// for sprite rendering a section of a spirte sheet 
		SDL_Rect spriteClip; 	
		
		// menu variables
		int option; // menu option 
		char ** options; // strings for options 
		int numOptions; // number of options 
		int menuX, menuY; // menu option coordinates
		
		// npc information for dungeon 
		int numNPCs;
		struct npc * activeNPCs;
		int nearestNPC; // the npc the player is closest to
		
		int conversationPhase; // used for keeping track of conversations in the game 
		
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
		
		// is the current section the player is in a safe zone where they can walk around and such? (toggles full visibility)
		int safeZone;
		
		// dungeon crawling variables
		int playerX; // x coord in dungeon 
		int playerY; // y coord in dungeon 
		int floor; // floor in dungeon 
		int building; // the building the player is in 
		int direction; // the direction the player is facing in the dungeon 
		int keys; // keys dropped by enemies that can be used to unlock doors 
		int money; // how much money the player has 
		
		// is this a new game? 
		int newGame; 
		
		// for specific menu levels in certain systems 
		int menuLevel;
		
		int calledSystem;
		
		int enemyType; // the type of enemy the player is fighting 
		
		int numQuests; // number of quests the player has 
		struct quest currentQuests[100]; // current quests the player has open 
		
		struct character stats; // stats of the player character 
	};
	
	void accessDungeonData(struct gameState * state, int writeMode);
	void loadSettings(struct gameState * state);
	
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
		
		state->menuLevel = 0;
		
		// initialize music 
		state->music = NULL;	
		state->track = NULL;
		
		state->fullScreen = 0;
		state->screenW = WINDOW_WIDTH-100;
		state->screenH = WINDOW_HEIGHT-100;
		
		// initialize graphics/music

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
		
		state->stats = generateCharacter(HUMAN);
		
		// set player name 
		strcpy(state->stats.name,"Player Character");
		
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
		
		state->numQuests = 0;
		
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
		state->keys = 1;
		state->money = 0;
		
		state->safeZone = 0;
		
		state->visible = NULL;

		state->dungeonSize = 0;

		state->numEnemies = 0;
		state->activeEnemies = NULL;

		state->maxStatus = 5; // max number of status lines visible 
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
		
		SDL_SetTextureBlendMode(s->images[s->imageStackSize].texture, SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(s->images[s->imageStackSize].texture, s->megaAlpha);
		
	}
	
	// adding a quest when the player takes one on 
	void addQuest(int questNum, struct gameState * state)
	{
		state->currentQuests[state->numQuests].questType = questNum;
		
		state->numQuests++;
	}
	
	// sets the alpha of all images/fonts 
	void setAlphaOfImages(struct gameState * state)
	{
		int i;
	//	printf("\n%d",state->megaAlpha);
		
		if(state->megaAlpha < 0)
			state->megaAlpha = 0;
		else if(state->megaAlpha > 255)
			state->megaAlpha = 255;
		
		// set alpha of all images used 
		for(i=0;i<state->numImages;i++)
		{
			SDL_SetTextureBlendMode(state->images[i].texture, SDL_BLENDMODE_BLEND);
			SDL_SetTextureAlphaMod(state->images[i].texture, state->megaAlpha);
		}
		
		// set alpha of background asset 
		SDL_SetTextureBlendMode(state->backgroundAsset.texture, SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(state->backgroundAsset.texture, state->megaAlpha);
		
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
		free(state->currentQuests);
		
		// deallocate data used			
		// deallocate images
		clearImages(state);
		
		// deallocate background image
		deallocateImage(&state->backgroundAsset);
		
		// deallocate font 
		deallocateFont(state->fontHandler);
		
		// deallocate renderer and window 
		SDL_DestroyRenderer(state->renderer);
		SDL_DestroyWindow(state->window);
		
		// quit sdl
		SDL_Quit();
	}
	
	// saving data to a file 
	void saveData(struct gameState * state)
	{
		FILE *file;
		char * fileReader = malloc(128 * sizeof(char)); 
		file = fopen(MAIN_SAVE_FILE,"w+");
		
		if(file != NULL)
		{
			// save stats 
			fwrite(state, sizeof(struct gameState), 1, file);
			fclose(file);
			
			// save current dungeon data 
			accessDungeonData(state,0);
		}
		else
			throwError("ERROR: COULD NOT SAVE DATA");
		
	}
	
	// loading data to a file 
	void loadData(struct gameState * state)
	{
		FILE *file;
		char * fileReader = malloc(128 * sizeof(char)); 
		file = fopen("./data/saveData.dat","r");
		int i;
		
		struct gameState * getData = malloc(sizeof(struct gameState));
		
		// get data from the file 
		if (file != NULL) 
		{
			fread(getData, sizeof(struct gameState), 1, file);
			fclose(file);
		}
		
		state->playerX = getData->playerX; // x coord in dungeon 
		state->playerY = getData->playerY; // y coord in dungeon 
		state->floor = getData->floor; // floor in dungeon 
		state->building = getData->building; // the building the player is in 
		state->direction = getData->direction; // the direction the player is facing in the dungeon 
		state->keys = getData->keys; // keys dropped by enemies that can be used to unlock doors 
		state->money = getData->money;
		
		state->stats = getData->stats;
		
		state->numQuests = getData->numQuests;
		
		if(state->numQuests != 0 )
		{
			for(i=0;i<state->numQuests;i++)
			{
				state->currentQuests[i].questType = getData->currentQuests[i].questType;
				state->currentQuests[i].progress = getData->currentQuests[i].progress;
				state->currentQuests[i].maxProg = getData->currentQuests[i].maxProg;
				state->currentQuests[i].completed = getData->currentQuests[i].completed;
			}
		}
		
		
		free(getData);
	}
	
#endif
