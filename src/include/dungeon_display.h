// for handling dungeon display logic  
#include "base/information/filenames.h"

#ifndef DUNGEON_LOGIC_HANDLED
#include "dungeon_logic.h"
#endif

#ifndef DUNGEON_DISPLAY_HANDLED
#define DUNGEON_DISPLAY_HANDLED

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
	
	sprintf(displayer,"Keys: %d",state->keys);	
	printText(displayer, 10, 0, state->fontHandler);
		
	sprintf(displayer,"Gold: %d",state->money);	
	printText(displayer, 200, 0, state->fontHandler);
	
	sprintf(displayer,"Sector %d",state->building);	
	printText(displayer, 390, 0, state->fontHandler);

	sprintf(displayer,"Floor: %d",state->floor);	
	printText(displayer, 580, 0, state->fontHandler);
	
	printText("STATUS:", 10, state->screenH-200, state->fontHandler);
	
	for(i=0;i<state->numStatusLines;i++)
		printText(state->statusText[i], 10, state->screenH-200+FONT_SIZE+i*FONT_SIZE, state->fontHandler);
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
		free(state->statusText[0]);
		for(i=0;i<state->numStatusLines-1;i++)
			state->statusText[i] = state->statusText[i+1];
		
		state->statusText[state->numStatusLines-1] = text;
	}
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
	
	// initialize images used in dungeon crawling
	state->images = malloc(sizeof(struct image));
	state->numImages = 1;
		
	addImage(state,DUNGEON_SPRITE);
	state->images[0].x = SPRITE_SQUARE_SIZE;
	state->images[0].y = SPRITE_SQUARE_SIZE;
	switch(state->screenW)
	{
		case 1400:
		state->images[0].scale = 2; // scale of the images on the screen 
		break;
		case 1200:
		state->images[0].scale = 2; // scale of the images on the screen 
		break;
		default:
		state->images[0].scale = 2; // scale of the images on the screen 
		break;
	}

	// initial display range 
	displayRange(state);
}


#endif
