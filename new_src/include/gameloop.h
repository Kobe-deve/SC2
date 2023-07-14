#include "base/information/filenames.h"
#include "base/state.h"
#include "base/menu.h"
#include "title_screen.h"
#include "battle.h"
#include "character_menu.h"
#include "dungeon.h"

// how much the alpha will change per frame when transitioning
#define ALPHA_CHANGE 10

// general game loop operations
#ifndef GAMELOOP_HANDLED
#define GAMELOOP_HANDLED

	// handles general logic operations 
	void logicHandler(struct gameState * state)
	{	
		// initialize logic 
		if((state->megaAlpha == 0 && state->graphicsMode == 0) || (state->graphicsMode == 1 && state->numImages == -1))
		{
			state->numImages = 0;
			switch(state->gameSystem)
			{
				case DUNGEON_SCREEN:
				if(state->d == NULL) // check if the dungeon array is empty, if so initialize
					initDungeonFloor(state);
				else
					resetDungeon(state);
				break;
				case BATTLE_SCREEN:
				initBattle(state);
				break;
				case MENU_SCREEN:
				initCharacterMenu(state);
				break;
			}
		}
		else if(state->switchSystem != 1 && state->fadeIn == 0) // basic logic used
		{
			switch(state->gameSystem)
			{
				case TITLE_SCREEN:
				titleScreenHandler(state);
				break;
				case DUNGEON_SCREEN:
				dungeonLogic(state);
				break;
				case BATTLE_SCREEN:
				battleLogic(state);
				break;
				case MENU_SCREEN:
				characterMenuLogic(state);
				break;
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
			case BATTLE_SCREEN:
			battleDisplay(state);
			break;
			case MENU_SCREEN:
			characterMenuDisplay(state);
			break;
		}
		
		if(state->switchSystem == 1 && state->fadeIn == 0 &&  state->megaAlpha >= 0) // if switching to system, begin switch process with alpha handling 
		{
			state->megaAlpha-=ALPHA_CHANGE;
			
			// if alpha is at zero, change system and start fade in 
			if(state->megaAlpha <= 0)
			{
				state->switchSystem = 0;
				state->gameSystem = state->switchTo;
				
				if(state->graphicsMode == 1) // if sprite mode, start fading in 
				{
					state->fadeIn = 1;	
					clearImages(state);
					state->numImages = -1;
				}
				else
					system("cls");
				state->megaAlpha = 0;
			}
		}
		else if(state->megaAlpha <= 255 && state->fadeIn == 1) // fade into new system 
		{
			state->megaAlpha+=ALPHA_CHANGE;
			
			if((state->megaAlpha >= 255 && state->graphicsMode == 1))
			{
				state->megaAlpha = 255;
				state->fadeIn = 0;
			}
		}
		
		if(state->graphicsMode == 1 && state->numImages >= 0)
			setAlphaOfImages(state);
	}

#endif
