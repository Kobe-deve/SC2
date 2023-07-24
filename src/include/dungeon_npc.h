// for handling npcs in dungeons 
#ifndef DUNGEON_LOGIC_HANDLED
#include "dungeon_logic.h"
#endif

#ifndef DUNGEON_DISPLAY_HANDLED
#include "dungeon_display.h"
#endif

#ifndef DUNGEON_NPC__HANDLED
#define DUNGEON_NPC__HANDLED

// function to check if an npc is at a certain coordinate 
int npcNearby(int x, int y, int f, int isPlayer, struct gameState * state)
{
	int i;
	
	for(i=0;i<state->numNPCs;i++)
	{
		if(!state->activeNPCs[i].passBy && state->activeNPCs[i].active && (state->activeNPCs[i].floor == f && (y == state->activeNPCs[i].y && x == state->activeNPCs[i].x)))
		{
			switch(isPlayer)
			{
				case 1: // player 
				state->nearestNPC = i;
				case 0: // checking 
				return 1;
				break;
				case -1: // enemy 
				return i;
				break;
			}
		}
	}
	
	switch(isPlayer)
	{
		case 1: // player 
		state->nearestNPC = 0;
		case 0: // checking 
		return 0;
		break;
		case -1: // enemy 
		return -1;
		break;
	}

	return 0;
}

// load npcs from a file based on the dungeon type 
void generateNPCs(struct gameState * state, int dungeonType)
{
	FILE *readFile = NULL;
	char * fileReader = malloc(128 * sizeof(char)); 
	
	int i;
	
	switch(dungeonType)
	{
		case 0:
		readFile = fopen(ORIGINAL_DUNGEON_NPC,"r");
		break;
	}
	
	if(!readFile && readFile != NULL)
	{
		printf("ERROR:DUNGEON NPC FILE COULD NOT BE READ");
		getchar();
		exit(0);
	}
	else
	{
		// read number of npcs
		fscanf(readFile,"%s",fileReader);
		state->numNPCs = atoi(fileReader);
		
		state->activeNPCs = malloc(state->numNPCs * sizeof(struct npc));
	
		fscanf(readFile,"%s",fileReader);
		for(i=0;i<state->numNPCs;i++)
		{
			// define variables from file 
			/*
				x
				y
				floor
				type
			*/
			
			fscanf(readFile,"%s",fileReader);
			state->activeNPCs[i].x = atoi(fileReader);
		
			fscanf(readFile,"%s",fileReader);
			state->activeNPCs[i].y = atoi(fileReader);
			
			fscanf(readFile,"%s",fileReader);
			state->activeNPCs[i].floor = atoi(fileReader);
			
			fscanf(readFile,"%s",fileReader);
			state->activeNPCs[i].type = atoi(fileReader);
			
			switch(state->activeNPCs[i].type)
			{
				default:
				state->activeNPCs[i].stats = generateCharacter(HUMAN); // set up stats 
				break;
			}
			
			fscanf(readFile,"%s",fileReader);
			state->activeNPCs[i].speed = atoi(fileReader);
			
			// set default for conditions
			state->activeNPCs[i].inCombat = 0;  
			state->activeNPCs[i].talking = 0;  
			state->activeNPCs[i].direction = 0;
			state->activeNPCs[i].active = 1; 
			state->activeNPCs[i].startTicks = 0;
			state->activeNPCs[i].enemyCombat = -1;
			state->activeNPCs[i].passBy = 0;
			
			state->activeNPCs[i].numSaved = 0;
			state->activeNPCs[i].numPassed = 0;
			
			// skip last line 
			fscanf(readFile,"%s",fileReader);
		}
	}
}

// handling npcs on the floor 
void npcHandler(struct gameState * state)
{
	int i,j;
	int movement = 0; // boolean variable for if the npc should move at all when their movement timer is up 
	int cx,cy;
	int xdirection,ydirection;
	
	// display/move npcs  
	for(i = 0;i<state->numNPCs;i++)
	{	
		movement = 1;
		
		if(state->activeNPCs[i].active && state->activeNPCs[i].floor == state->floor)
		{	
			// movement logic if movement timer is up 
			if(movement == 1 && state->activeNPCs[i].inCombat != 1 && state->activeNPCs[i].talking != 1 && ((int)(SDL_GetTicks() - state->activeNPCs[i].startTicks))/1000.f >= state->activeNPCs[i].speed)
			{	
				// erase/update current spot when moving 
				if(state->graphicsMode == 0 && !state->activeNPCs[i].talking && !state->activeNPCs[i].inCombat && movement == 1 && state->floor == state->activeNPCs[i].floor && state->visible[state->activeNPCs[i].floor][state->activeNPCs[i].y][state->activeNPCs[i].x] == 1)
				{
					setColor(WHITE);
					setCursor(dungeonPrintCoordX+state->activeNPCs[i].x,dungeonPrintCoordY+state->activeNPCs[i].y);
					printf("%c",quickConvert(state->d[state->floor][state->activeNPCs[i].y][state->activeNPCs[i].x]));	
				}
				
				
				// determine movement 
				
				// store original position in case we have to go back 
				cx = state->activeNPCs[i].x;
				cy = state->activeNPCs[i].y;
				
				// determine movement 
				if(state->upStairCoords[state->activeNPCs[i].floor][0] > state->activeNPCs[i].x)
					xdirection = 1;
				else if(state->upStairCoords[state->activeNPCs[i].floor][0] < state->activeNPCs[i].x)
					xdirection = 3;
				
				if(state->upStairCoords[state->activeNPCs[i].floor][1] > state->activeNPCs[i].y)
					ydirection = 2;
				else if(state->upStairCoords[state->activeNPCs[i].floor][1] < state->activeNPCs[i].y)
					ydirection = 0;
				
				// execute movement 
				switch(ydirection)
				{
					case 0: // UP
					if(state->activeNPCs[i].y - 1 >= 0 && passableBlock(state->activeNPCs[i].x,state->activeNPCs[i].y-1,state))
						state->activeNPCs[i].y = state->activeNPCs[i].y - 1;				
					break;
					case 2: // DOWN
					if(state->activeNPCs[i].y + 1 < state->dungeonSize && passableBlock(state->activeNPCs[i].x,state->activeNPCs[i].y+1,state))
						state->activeNPCs[i].y = state->activeNPCs[i].y + 1;
					break;
				}
				
				switch(xdirection)
				{				
					case 1: // RIGHT
					if(state->activeNPCs[i].x + 1 < state->dungeonSize && passableBlock(state->activeNPCs[i].x+1,state->activeNPCs[i].y,state))
						state->activeNPCs[i].x = state->activeNPCs[i].x + 1;				
					break;
					case 3: // LEFT
					if(state->activeNPCs[i].x - 1 >= 0 && passableBlock(state->activeNPCs[i].x-1,state->activeNPCs[i].y,state))
						state->activeNPCs[i].x = state->activeNPCs[i].x - 1;
					break;
				}
				
				// check if there isn't an overlap with another npc 
				for(j=0;j<state->numNPCs;j++)
				{
					if(j!=i && (state->activeNPCs[i].x == state->activeNPCs[j].x) && (state->activeNPCs[i].y == state->activeNPCs[j].y))
					{
						cx = state->activeNPCs[i].x;
						cy = state->activeNPCs[i].y;
						break;
					}
				}
				
				// check if there isn't any overlap with the player 
				if(state->activeNPCs[i].y == state->playerY && state->activeNPCs[i].x == state->playerX)
				{
					state->activeNPCs[i].x = cx;
					state->activeNPCs[i].y = cy;
				}						
				
				
				// go upstairs if at stairs 
				if(state->d[state->activeNPCs[i].floor][state->activeNPCs[i].y][state->activeNPCs[i].x] == 2)
					state->activeNPCs[i].floor++;
				
				state->activeNPCs[i].startTicks = SDL_GetTicks();
			}
			
			// display npc 
			if(state->visible[state->activeNPCs[i].floor][state->activeNPCs[i].y][state->activeNPCs[i].x] == 1 && state->floor == state->activeNPCs[i].floor)
			{
				switch(state->graphicsMode)
				{
					case 0:
					setCursor(dungeonPrintCoordX+state->activeNPCs[i].x,dungeonPrintCoordY+state->activeNPCs[i].y);
					switch(state->activeNPCs[i].type)
					{
						case 0:
						setColor(DARK_BABY_BLUE);
						break;
						case 1:
						setColor(SILVER);
						break;
					}
				
					if(state->activeNPCs[i].inCombat == 1) // shows a fight 
						printf("X");
					else
						printf("%c",1);
					break;
					case 1:
					
					break;
				}
			}
		}
	
	}
	setColor(WHITE);
}

// exiting an npc conversation 
void exitNPCConversation(struct gameState * state)
{
	updateStatus(NPC_DONE_TALK,state);
	deallocateMenu(state);	
	state->activeNPCs[state->nearestNPC].talking = 0;
}

// function for handling conversations with the npc 
void npcConversationHandler(struct gameState * state)
{
	// handles the main menu the player is using to interact with the npc 
	handleMenu(state);
	
	// handle input 
	if(state->input == ENTER)
	{
		switch(state->option)
		{
			case 2:
			state->activeNPCs[state->nearestNPC].passBy = 1;
			break;
		}
		
		exitNPCConversation(state);
	}
}

#endif