#include "base/information/filenames.h"
#include "base/information/stats.h"
#include "base/state.h"
#include "base/menu.h"
#include "base/graphics/music.h"
#include "base/graphics/sprite_data.h"
#include "shops.h"
#include "title_screen.h"
#include "battle.h"
#include "character_menu.h"
#include "dungeon.h"
#include "settings.h"

// how much the alpha will change per frame when transitioning
#define ALPHA_CHANGE 15

// general game loop operations
#ifndef GAMELOOP_HANDLED
#define GAMELOOP_HANDLED

	// initialize game
	void initializeGame(struct gameState * state)
	{
		// initialize the game state 
		init(state);
		
		// initialize music handler
		initMusic(state);
		
		// set up SDL input handling if graphics mode enabled 
		if(state->graphicsMode == 1)
		{
			// initialize event handler for SDL2 events and renderer color 
			state->e = malloc(sizeof(SDL_Event));
			state->colors[0] = 0;
			state->colors[1] = 0;
			state->colors[2] = 100;
			state->colors[3] = 0;
			
			// initialize background assets
			state->backgroundAsset = initImage(BATTLE_BACKGROUND_ASSET,state->renderer);
			state->backgroundAsset.scale = 4;	
		}
	}

	// deallocate game 
	void deallocateGame(struct gameState * state)
	{
		// deallocate based on the current system being used 
		switch(state->gameSystem)
		{
			case DUNGEON_SCREEN:
			deallocateDungeon(state);
			break;
		}
		
		// deallocate background image if in spirte mode 
		if(state->graphicsMode == 1)
		{
			deallocateImage(&state->backgroundAsset);
		}
		
		// deallocate menu if it is being used
		if(state->options != NULL)
			deallocateMenu(state);
		
		deallocate(state);
	}


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
				case SHOP_SCREEN:
				shopLogic(state);
				break;
				case SETTINGS_SCREEN:
				settingScreenHandler(state);
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
			case SHOP_SCREEN:
			shopScreenDisplay(state);
			break;
			case SETTINGS_SCREEN:
			settingScreenDisplay(state);
			break; 
		}
		
		if(state->switchSystem == 1 && state->fadeIn == 0 &&  state->megaAlpha >= 0) // if switching to system, begin switch process with alpha handling 
		{
			state->megaAlpha-=ALPHA_CHANGE;
			
			// if alpha is at zero, change system and start fade in 
			if(state->megaAlpha <= 0)
			{
				state->megaAlpha = 0;
				state->switchSystem = 0;
				state->gameSystem = state->switchTo;
				
				if(state->graphicsMode == 1) // if sprite mode, start fading in 
				{
					state->fadeIn = 1;	
					clearImages(state);
					state->numImages = -1;
					
					// set new background asset
					deallocateImage(&state->backgroundAsset);
					switch(state->gameSystem)
					{
						default:
						case TITLE_SCREEN:
						case DUNGEON_SCREEN:
						state->backgroundAsset = initImage(DUNGEON_BACKGROUND_ASSET,state->renderer);
						break;
						case BATTLE_SCREEN:
						state->backgroundAsset = initImage(BATTLE_BACKGROUND_ASSET,state->renderer);
						break;
					}
					state->backgroundAsset.scale = 4;	
				}
				else
					system("cls");
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
		
		if(state->graphicsMode == 1)
			setAlphaOfImages(state);
	}

#endif
