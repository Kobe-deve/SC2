// for handling dungeon display logic  
#include "base/information/filenames.h"

#ifndef DUNGEON_LOGIC_HANDLED
#include "dungeon_logic.h"
#endif

#ifndef DUNGEON_DISPLAY_HANDLED
#define DUNGEON_DISPLAY_HANDLED

// clear display when player moves  
void clearDisplay(struct gameState * s)
{	
	if(s->input != 0)
	{
		setCursor(dungeonPrintCoordX+s->playerX,dungeonPrintCoordY+s->playerY);
		printf("%c",quickConvert(s->d[s->floor][s->playerY][s->playerX]));	
	}
}

// updating display range when moving  
void displayRange(struct gameState * state)
{
	int i;
	int x,y;
	
	for(y=state->playerY-2;y<state->playerY+3;y++)
	{
		for(x=state->playerX-2;x<state->playerX+3;x++)
		{	
			if(!(x == state->playerX && y == state->playerY))
			{
				if((y >= 0 && x >= 0 && y < state->dungeonSize && x < state->dungeonSize))
				{
					if((state->visible[state->floor][y][x] == 0))
					{
						setCursor(dungeonPrintCoordX+x,dungeonPrintCoordY+y);
			
						// update what is visible to the player 
						state->visible[state->floor][y][x] = 1;
						
						if(state->graphicsMode == 0 )
							printf("%c",quickConvert(state->d[state->floor][y][x]));
					}
				}
				
				setColor(WHITE);
			}
			
			// if in ascii mode, update display if enemies are visible 
			if(state->graphicsMode == 0 )
			{
				// checks if any enemies are visible with the updated range
				if((y >= 0 && x >= 0 && y < state->dungeonSize && x < state->dungeonSize))
				{
					for(i = 0;i<state->numEnemies;i++)
					{
						if(state->activeEnemies[i].inCombat == 0 && state->activeEnemies[i].active == 1 && state->activeEnemies[i].x == x && state->activeEnemies[i].y == y)
						{
							setCursor(dungeonPrintCoordX+state->activeEnemies[i].x,dungeonPrintCoordX+state->activeEnemies[i].y);
							printf("+");
						}
					}
				}
			}
		}
	}
}

// generate text status descriptions based on where the player is 
void description(struct gameState * s)
{	
}

#endif
