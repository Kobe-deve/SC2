// for handling dungeon display logic  
#include "base/information/filenames.h"

#ifndef DUNGEON_LOGIC_HANDLED
#include "dungeon_logic.h"
#endif

#ifndef DUNGEON_DISPLAY_HANDLED
#define DUNGEON_DISPLAY_HANDLED

// display range when moving  
void displayRange(struct gameState * state)
{
	int i;
	int enemyHere = 0;
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
			
						state->visible[state->floor][y][x] = 1;
						
						printf("%c",quickConvert(state->d[state->floor][y][x]));
					}
				}
				
				setColor(WHITE);
			}
			
			// checks if any enemies are visible with the updated range
			if((y >= 0 && x >= 0 && y < state->dungeonSize && x < state->dungeonSize))
			{
				for(i = 0;i<state->numEnemies;i++)
				{
					if(state->graphicsMode == 0 && state->activeEnemies[i].inCombat == 0 && state->activeEnemies[i].active == 1 && state->activeEnemies[i].x == x && state->activeEnemies[i].y == y)
					{
						setCursor(dungeonPrintCoordX+state->activeEnemies[i].x,dungeonPrintCoordX+state->activeEnemies[i].y);
						printf("+");
					}
				}
			}
		}
	}
}


#endif
