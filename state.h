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
	struct gameState
	{
		Mix_Music * music; // the music playing
		int input; // input integer
		struct EventHandler *listeners[MAX_EVENTS]; // event listeners
	};
#endif