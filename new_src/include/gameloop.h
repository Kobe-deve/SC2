#include "base/information/filenames.h"
#include "base/state.h"

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
		switch(state->graphicsMode)
		{
			case 0: // ascii mode 
			break;
			case 1: // sprite mode
			break;
		}
		
		if(state->switchSystem == 1) // if switching to system, begin switch process with alpha handling 
		{
			
		}
		else
		{
			
		}
	}

#endif
