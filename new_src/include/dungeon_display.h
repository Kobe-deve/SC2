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

// display status in dungeon 
void displayStatus(struct gameState * state)
{
	int i;
	
	switch(state->graphicsMode)
	{
		case 0:
		setCursor(1,30);
		printf("STATUS:");
	
		// reset displayed order 
		for(i=0;i<state->numStatusLines;i++)
		{
			setCursor(1,31+i);
			printf("\33[2K");
			setCursor(1,31+i);
			printf("%s",state->statusText[i]);
		}
		break;
		case 1:
		
		printText("STATUS:", 10, 400, state->fontHandler);
		
		for(i=0;i<state->numStatusLines;i++)
			printText(state->statusText[i], 10, 400+FONT_SIZE+i*FONT_SIZE, state->fontHandler);
		break;
	}
}

// update status text and display it 
void updateStatus(char * text,struct gameState * state)
{
	int i;

	// set order of text
	if(state->numStatusLines < state->maxStatus)
	{
		state->statusText[state->numStatusLines] = text;
		++state->numStatusLines;
	}
	else // move recent to bottom if max is filled 
	{
		if(state->graphicsMode == 0 && strlen(state->statusText[0]) > strlen(text))
		{
			setColor(BLACK);
			for(i=0;i<strlen(state->statusText[0])+1;i++)
			{
				setCursor(1+i,21);
				printf("%c",219);
			}
			setColor(WHITE);
		}
	
		free(state->statusText[0]);
		for(i=0;i<state->numStatusLines-1;i++)
			state->statusText[i] = state->statusText[i+1];
		
		state->statusText[state->numStatusLines-1] = text;
	}
	
	if(state->graphicsMode == 0)
		displayStatus(state);
}

// generate text status descriptions based on where the player is 
void description(struct gameState * s)
{	
}

#endif
