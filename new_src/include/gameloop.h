#include "base/information/filenames.h"
#include "base/state.h"
#include "title_screen.h"
#include "dungeon.h"

// general game loop operations
#ifndef GAMELOOP_HANDLED
#define GAMELOOP_HANDLED

	// handles general logic operations 
	void logicHandler(struct gameState * state)
	{	
		if(state->switchSystem != 1) // if switching to system, begin switch process
		{
			if(state->input == ENTER)
			{		
				state->switchSystem = 1;
	
				switch(state->gameSystem)
				{
					case TITLE_SCREEN:
					state->switchTo = DUNGEON_SCREEN;
					break;
					case DUNGEON_SCREEN:
					state->switchTo = TITLE_SCREEN;
					break;
				}
			}
		}
	}

	// handles general display operations
	void displayHandler(struct gameState * state)
	{
		switch(state->gameSystem)
		{
			case TITLE_SCREEN:
			titleScreenDisplay(state);
			break;
			case DUNGEON_SCREEN:
			dungeonDisplay(state);
			break;
		}
		
		if(state->switchSystem == 1 && state->fadeIn == 0 &&  state->megaAlpha > 0) // if switching to system, begin switch process with alpha handling 
		{
			state->megaAlpha-=5;
			
			// if alpha is at zero, change system and start fade in 
			if(state->megaAlpha <= 0)
			{
				state->switchSystem = 0;
				state->gameSystem = state->switchTo;
				
				if(state->graphicsMode == 1) // if sprite mode, start fading in 
				{
					state->fadeIn = 1;
					clearImages(state);
				}
				else
					system("cls");
				state->megaAlpha = 0;
			}
		}
		else if(state->megaAlpha <= 255 && state->fadeIn == 1) // fade into new system 
		{
			state->megaAlpha+=5;
			
			if((state->megaAlpha >= 255 && state->graphicsMode == 1))
				state->fadeIn = 0;
		}
		
		setAlphaOfImages(state);
	}

#endif
