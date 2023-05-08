
// handles the state of the game itself 

// get basic music handling library
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

#ifndef EVENT_HANDLING
#include "event_handler.h"
#endif

#ifndef FILENAMES
#include "filenames.h"
#endif

#ifndef STATE_HANDLED
#include "stats.h"
#endif

#ifndef FONT_HANDLED
#include "font.h"
#endif

#ifndef IMAGE_HANDLED
#include "image.h"
#endif

// state struct
#ifndef STATE_HANDLED
#define STATE_HANDLED
	typedef struct text text;

	// global variable to show when testing mode is active
	int testMode = 0;

	// pauses for he number of seconds
	int pause(int he) 
	{
		time_t then;
		
		time(&then);
		while(difftime(time(NULL),then) < he);
		fflush(stdin); // clears any extra enter presses
	}
	
	// data structure for handling battles 
	struct battle
	{
		int turns; // number of turns passed in the game
		struct character * enemies; // enemies in combat 
		int numEnemies; // number of enemies in combat 
		
		struct character * turnOrder; // turn order of combat 
		int numChars; // number of characters in combat 
	};
	
	// data structure for handling the state/data of the game 
	struct gameState
	{
		Mix_Music * music; // the music playing
		char * track; // the song that is playing 
		int input; // input integer
		struct EventHandler *listeners[MAX_EVENTS]; // event listeners
		
		// menu variables
		int option; // menu option 
		char ** options; // strings for options 
		int numOptions; // number of options 
		int menuX, menuY; // menu option coordinates
		
		// dungeon crawling variables
		int playerX; // x coord in dungeon 
		int playerY; // y coord in dungeon 
		int floor; // floor in dungeon 
		
		int building; // what building the player is in 
	
		struct character protag_stats; // stats of the main character 
		
		int partySize;
		struct character * party; // the player's party 
		
		struct battle currentBattle; // the current battle the player is in 
	
		// for rendering in sprite mode 
		SDL_Window* window; // window 
		SDL_Renderer* renderer; // window renderer
	
		struct text * fontHandler; // handles text display
		
		int allImageAlpha; // the alpha of all images, used for fading in/out
		struct image * images; // displayed images used in sprite mode
		int imageStackSize; // number of images in array 
		int numImages; // max size of array 
	};
	
	
	// add to images used in current state 
	void addImage(struct gameState * s,char * filePath)
	{
		if(s->imageStackSize < s->numImages)
		{
			s->images[s->imageStackSize] = initImage(filePath, s->renderer);
			s->imageStackSize++;
		}
	}
	
	// fading out all images 
	void fadeOutImages(struct gameState * s)
	{
		int i;
		for(i=0;i<s->numImages;i++)
			s->images[i].alpha = s->allImageAlpha;
		
		s->allImageAlpha--;
		if(s->allImageAlpha < 0)
			s->allImageAlpha = 0;
	}
	
	// fading in all images 
	void fadeInImages(struct gameState * s)
	{
		int i;
		for(i=0;i<s->numImages;i++)
			s->images[i].alpha = s->allImageAlpha;
	
		s->allImageAlpha++;
		if(s->allImageAlpha > 255)
			s->allImageAlpha = 255;
	}
	
	// clear image array 
	void clearImages(struct gameState * s)
	{
		s->allImageAlpha = 0;
		int i;
		
		for(i=0;i<s->numImages;i++)
		{
			deallocateImage(&s->images[i]);
		}
		
		s->images = NULL;
		
		s->imageStackSize = 0;
		s->numImages = 0;
	}
	
	// adding a party member to the party 
	void addPartyMember(struct character stats, struct gameState * s)
	{
		if(s->party == NULL)
		{
			s->party = malloc(4 * sizeof(struct character));
			s->party[0] = stats;
			s->partySize++;
		}
		else if(s->partySize < 4)
		{
			s->party[s->partySize] = stats;
			s->partySize++;
		}
	}
	
	// deallocating state 
	void clearState(struct gameState * s)
	{
		if(s->party != NULL)
		{
			free(s->party);
		}
		free(s->track);
	}
	
	// load state save data
	void loadState(char * fileName)
	{
		FILE *dataFile;
		
		dataFile = fopen(fileName,"r");
		
		
	};
	
	// save state data 
	void saveState(struct gameState * data)
	{
		FILE *dataFile;
	};
	
	// debug mode toggle
	int debug = 0;
	
	// initialize a new game 
	void initNewGame(struct gameState * s);
	
	// functions used for going between states
	void initBattle(void *data);
	void walkAround(void *data);
	void resetDungeon(void *data);
	void initShop(struct gameState * s);
	
	// free data functions
	void freeDungeonData(void *data);
#endif

