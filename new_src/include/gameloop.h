#include "base/information/filenames.h"
#include "base/state.h"
#include "title_screen.h"

// general game loop operations
#ifndef GAMELOOP_HANDLED
#define GAMELOOP_HANDLED

	// handles general logic operations 
	void logicHandler(struct gameState * state)
	{
		if(state->switchSystem == 1) // if switching to system, begin switch process
		{
		}
		else
		{
		}
	}

	// handles general display operations
	void displayHandler(struct gameState * state)
	{
		titleScreenDisplay(state);
		
		if(state->switchSystem == 1) // if switching to system, begin switch process with alpha handling 
		{
			
		}
		else
		{
			
		}
	}

#endif
