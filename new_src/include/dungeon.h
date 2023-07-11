// for general handling dungeon crawling in the game 

// convert specific tiles in dungeon to type
int quickConvert(int x)
{
	switch(x)
	{
		case 1:
		return 219;
		break;
		
		case 2:
		case 3:
		return 220;
		break;
		
		case 4:
		return 233;
		break;
		
		case 5:
		return 237;
		break;
		
		case 6:
		case A:
		case B:
		case 12:
		case D:
		case F:
		case G:
		case 7:
		return 1;
		break;
		
		case 8:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		return 234;
		break;
		
		case 9:
		return 206;
		break;
		
		default:
		case 100:
		return 178;
		break;
	}
}

// struct for movable enemies
struct enemies 
{
	int x; // coordinates
	int y;
	int active; // is the enemy alive 
	int type; // the type of enemy for the encounter 
	int inCombat; // is the enemy fighting an npc 
	int npcFighting; // what npc the enemy is fighting 
	int speed; // what interval does the enemy move 
	int startTicks;
	int health; // overworld health of enemy 
};

// coord to print dungeon at 
int dungeonPrintCoordX = 1;
int dungeonPrintCoordY = 1;

#ifndef DUNGEON_LOGIC_HANDLED
#include "dungeon_logic.h"
#endif

#ifndef DUNGEON_DISPLAY_HANDLED
#include "dungeon_display.h"
#endif

#ifndef DUNGEON_HANDLED
#define DUNGEON_HANDLED

// dungeon display function 
void dungeonDisplay(struct gameState * state)
{	
	switch(state->graphicsMode)
	{
		case 0:
		if(state->megaAlpha == 0) // update screen to add title screen 
		{	
			state->megaAlpha = 1;
		}		
		// display player
		setCursor(dungeonPrintCoordX+state->playerX,dungeonPrintCoordY+state->playerY);
		printf("%c",1);	
		break;
		case 1:
		// get dungeon assets if screen is cleared out
		if(state->numImages == 0)
		{
			addImage(state,BACKGROUND_ASSET);
			state->images[0].x = 20*12;
			state->images[0].y = 10*12;
		}
		else
			renderImage(&state->images[0], state->renderer, NULL);
		break;
	}
}

// logic handler for dungeon crawling 
void dungeonLogic(struct gameState * state)
{
	// clear the screen before movement 
	if(state->graphicsMode == 0)
		clearDisplay(state);
	
	// player movement logic 
	dungeonMovement(state);
}

#endif
