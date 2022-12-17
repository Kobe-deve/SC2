// handles the state of the game itself 

// get basic music handling library
#define SDL_MAIN_HANDLED

// main SDL2 handler
#include <SDL2/SDL.h>

// handles music with SDL2
#include <SDL2/SDL_mixer.h>

#ifndef STATE_HANDLED
#define STATE_HANDLED
	struct gameState
	{
		Mix_Music * music;
	};
#endif