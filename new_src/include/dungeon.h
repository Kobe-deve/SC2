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
	// reset visibility array if it is empty/deleted
	int iz,ix,iy;
	
	switch(state->graphicsMode)
	{
		case 0:
		if(state->megaAlpha == 0) // update screen to add title screen 
		{					
			// display dungeon walls 
			for(iy = -1;iy < state->dungeonSize+1;iy++)
			{
				for(ix = -1;ix < state->dungeonSize+1;ix++)
				{
					setCursor(dungeonPrintCoordX+ix,dungeonPrintCoordY+iy);
					if((ix == -1 || ix == state->dungeonSize || iy == -1 || iy == state->dungeonSize))
					{	
						setColor(BLUE);
						printf("%c",219);
					}
					else if(state->visible[state->floor][iy][ix] == 1)
					{
						setColor(WHITE);
						printf("%c",quickConvert(state->d[state->floor][iy][ix]));
					}
				}
			}		
			setColor(WHITE);
		
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
			// initialize images used in dungeon crawling
			state->images = malloc(sizeof(struct image));
			state->numImages = 1;
				
			addImage(state,DUNGEON_SPRITE);
			state->images[0].x = SPRITE_SQUARE_SIZE;
			state->images[0].y = SPRITE_SQUARE_SIZE;
			state->images[0].scale = 2;
		}
		else
		{
			// display player 
			state->spriteClip.x = 0;
			state->spriteClip.y = 0;
							
			state->images[0].x = state->playerX*SPRITE_SQUARE_SIZE*state->images[0].scale;
			state->images[0].y = state->playerY*SPRITE_SQUARE_SIZE*state->images[0].scale;
			renderImage(&state->images[0], state->renderer,&state->spriteClip);
		}			
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
