// for handling dungeon display logic  
#include "base/information/filenames.h"

#ifndef DUNGEON_LOGIC_HANDLED
#include "dungeon_logic.h"
#endif

#ifndef DUNGEON_DISPLAY_HANDLED
#define DUNGEON_DISPLAY_HANDLED

// used to quickly displaying specific characters in ascii mode based on the dungeon tile value 
int quickConvert(int x)
{
	switch(x)
	{
		case M:
		return 194;
		break;
		
		case A:
		return '|';
		break;
		case G:
		return '_';
		break;
		
		case D:
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
		case 12:
		case 7:
		return 1;
		break;
		
		case B:
		return 186;
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
		
		case F:
		default:
		case 100:
		return 178;
		break;
	}
}

// clear display when player moves  
void clearDisplay(struct gameState * state)
{	
	if(state->input != 0 && state->graphicsMode == 0)
	{
		setCursor(dungeonPrintCoordX+state->playerX,dungeonPrintCoordY+state->playerY);
		printf("%c",quickConvert(state->d[state->floor][state->playerY][state->playerX]));	
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
			if((y >= 0 && x >= 0 && y < state->dungeonSize && x < state->dungeonSize))
			{
				if((state->visible[state->floor][y][x] == 0))
				{
					// update what is visible to the player 
					state->visible[state->floor][y][x] = 1;
					
					if(state->graphicsMode == 0 && !(x == state->playerX && y == state->playerY))
					{
						setCursor(dungeonPrintCoordX+x,dungeonPrintCoordY+y);
						printf("%c",quickConvert(state->d[state->floor][y][x]));
						setColor(WHITE);
					}
				}
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
							setCursor(dungeonPrintCoordX+state->activeEnemies[i].x,dungeonPrintCoordY+state->activeEnemies[i].y);
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
	char displayer[400]; // used in sprite mode to display stats 
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
		
		setCursor(0,0);
		printf("Keys: %d",state->keys);
		
		setCursor(20,0);
		printf("Sector - %d",state->building);
		
		
		break;
		case 1:
		sprintf(displayer,"Keys: %d",state->keys);	
		printText(displayer, 10, 0, state->fontHandler);
		
		sprintf(displayer,"Sector %d",state->building);	
		printText(displayer, 200, 0, state->fontHandler);
		
		
		printText("STATUS:", 10, 500, state->fontHandler);
		
		for(i=0;i<state->numStatusLines;i++)
			printText(state->statusText[i], 10, 500+FONT_SIZE+i*FONT_SIZE, state->fontHandler);
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
void description(struct gameState * state)
{	
	// check nearby npcs close to the player 
	int npcTalked = (npcNearby(state->playerX,state->playerY-1,state->floor,1,state) || npcNearby(state->playerX,state->playerY+1,state->floor,1,state) || npcNearby(state->playerX+1,state->playerY,state->floor,1,state) || npcNearby(state->playerX-1,state->playerY,state->floor,1,state));
	
	// get nearby blocks the player can interact with 
	// TODO fix crashing with this 
	int nearby = nearbyBlocks(state,0);
	
	// update status text based on where the player is at 
	switch(state->d[state->floor][state->playerY][state->playerX])
	{
		case 5:
		updateStatus(EMPTY_CHEST,state);
		break;
		case 4:
		updateStatus(CLOSED_CHEST,state);
		break;
		case 9:
		updateStatus(SHOP_ENTRANCE,state);
		break;
		default:
		// if an npc is nearby, update status 
		if(npcTalked && !state->activeNPCs[state->nearestNPC].inCombat && state->activeNPCs[state->nearestNPC].active == 1)
		{
			switch(state->activeNPCs[state->nearestNPC].type)
			{
				case 0:
				updateStatus(NPC1,state);	
				break;
				case 1:
				updateStatus(NPC2,state);	
				break;
			}
		}
		else if(npcTalked && state->activeNPCs[state->nearestNPC].inCombat )
			updateStatus(NPC_FIGHT,state);	
		else if(nearby == B) // locked door 
		{
			if(state->keys == 0)
				updateStatus(LOCKED_DOOR,state);	
			else if(state->keys > 0)
				updateStatus(UNLOCKED_DOOR,state);	
		}
		else if(nearby == M) // nearby signs
		{
			updateStatus(SECTOR_0_SIGN,state);	
		}
		
		break;
	}
}


// initialize the dungeon display 
void initDungeonDisplay(struct gameState * state)
{
	int ix,iy,i;
	
	switch(state->graphicsMode)
	{
		case 0: // ascii 
		// set position of the dungeon on the screen
		dungeonPrintCoordX = 80;
		dungeonPrintCoordY = 10;
	
		// display dungeon walls 
		for(iy = -1;iy < state->dungeonSize+1;iy++)
		{
			for(ix = -1;ix < state->dungeonSize+1;ix++)
			{
				setCursor(dungeonPrintCoordX+ix,dungeonPrintCoordY+iy);
				if((ix == -1 || ix == state->dungeonSize || iy == -1 || iy == state->dungeonSize))
				{	
					switch(state->building)
					{
						case 0:
						setColor(BLUE);
						break;
						case 1:
						setColor(DARK_RED);
						break;
					}
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
	
		// initial display range 
		displayRange(state);
	
		break;
		case 1: // sprites 
		// initialize images used in dungeon crawling
		state->images = malloc(sizeof(struct image));
		state->numImages = 1;
			
		addImage(state,DUNGEON_SPRITE);
		state->images[0].x = SPRITE_SQUARE_SIZE;
		state->images[0].y = SPRITE_SQUARE_SIZE;
		state->images[0].scale = 3; // scale of the images on the screen 
		
		// set position of the dungeon on the screen
		spriteDungeonPrintCoordX = (WINDOW_WIDTH-100)/2-state->dungeonSize*state->images[0].scale*SPRITE_SQUARE_SIZE/2;
		spriteDungeonPrintCoordY = (WINDOW_HEIGHT-100)/2-state->dungeonSize*state->images[0].scale*SPRITE_SQUARE_SIZE/2;
		
		// initial display range 
		displayRange(state);
		break; 
	}
}


#endif
