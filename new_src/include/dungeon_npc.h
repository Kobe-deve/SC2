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
			// if the coordinates are for the player, return the specific npc value
			if(isPlayer)
				return i;
			return 1;
		}
	}
	return 0;
}

// load npcs from a file based on the dungeon type 
void generateNPCs(struct gameState * state, int dungeonType)
{
	FILE *readFile;
	char * fileReader = malloc(128 * sizeof(char)); 
	
	int i;
	
	switch(dungeonType)
	{
		default:
		case 0:
		readFile = fopen(ORIGINAL_DUNGEON_NPC,"r");
		break;
	}
	
	if(!readFile)
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
			fscanf(readFile,"%s",fileReader);
			state->activeNPCs[i].x = atoi(fileReader);
		
			fscanf(readFile,"%s",fileReader);
			state->activeNPCs[i].y = atoi(fileReader);
			
			fscanf(readFile,"%s",fileReader);
			state->activeNPCs[i].floor = atoi(fileReader);
			
			fscanf(readFile,"%s",fileReader);
			state->activeNPCs[i].type = atoi(fileReader);
			
			fscanf(readFile,"%s",fileReader);
			state->activeNPCs[i].speed = atoi(fileReader);
			
			fscanf(readFile,"%s",fileReader);
			//state->activeNPCs[i].goal = (enum goals)atoi(fileReader);
			
			fscanf(readFile,"%s",fileReader);
			switch(atoi(fileReader))
			{
				default:
				state->activeNPCs[i].stats = generateCharacter(HUMAN); // set up stats 
				break;
			}
			
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

#endif