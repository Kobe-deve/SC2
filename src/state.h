#include "event_handler.h"

// handles the state of the game itself 

// get basic music handling library
#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED

// main SDL2 handler
#include <SDL2/SDL.h>

// handles music with SDL2
#include <SDL2/SDL_mixer.h>
#endif

#ifndef FILENAMES
#include "filenames.h"
#endif

// state struct
#ifndef STATE_HANDLED
#define STATE_HANDLED

	#ifndef CHARACTER_HANDLED
	#include "stats.h"
	#endif

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
		
	};
	
	// data structure for handling the state/data of the game 
	struct gameState
	{
		Mix_Music * music; // the music playing
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
	};
	
	// initialize a new game 
	void initNewGame(struct gameState * s);
	void loadGame(struct gameState *s);
	
	// functions used for going between states
	void initBattle(void *data);
	void walkAround(void *data);
	void resetDungeon(void *data);
#endif

