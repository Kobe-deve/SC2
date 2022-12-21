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

// state struct
#ifndef STATE_HANDLED
#define STATE_HANDLED
	// pauses for he number of seconds
	int pause(int he) 
	{
		time_t then;
		
		time(&then);
		while(difftime(time(NULL),then) < he);
		fflush(stdin); // clears any extra enter presses
	}

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
#endif