// for handling dungeon logic 
#include "base/information/filenames.h"

#ifndef DUNGEON_LOGIC_HANDLED
#define DUNGEON_LOGIC_HANDLED

#ifndef DUNGEON_NPC__HANDLED
#include "dungeon_npc.h"
#endif

#ifndef DUNGEON_DISPLAY_HANDLED
#include "dungeon_display.h"
#endif

// if a switch is flipped, change the dungeon 
void flippedSwitch(int switchStatus, struct gameState * state)
{
	int iy,ix;
	
	// set all hidden wall/mechanism blocks to a specific status based on the switch 
	for(iy=0;iy<state->dungeonSize;iy++)
	{
		for(ix=0;ix<state->dungeonSize;ix++)
		{	
			if(state->d[state->floor][iy][ix] == D || state->d[state->floor][iy][ix] == F)
			{
				switch(switchStatus)
				{
					case A: // close 
					state->d[state->floor][iy][ix] = D;
					break;
					case G: //open 
					state->d[state->floor][iy][ix] = F;
					break;
				}
			}
			
			// if in graphics mode, update the screen 
			if(state->graphicsMode == 0 && state->visible[state->floor][iy][ix] == 1)
			{
				setCursor(dungeonPrintCoordX+ix,dungeonPrintCoordY+iy);
				printf("%c",quickConvert(state->d[state->floor][iy][ix]));
			}
		}
	}
}

// return nearby blocks that the player can interact with (if action is more than 0 then perform action)
int nearbyBlocks(struct gameState * state, int action)
{	
	int * spots[4] = {NULL,NULL,NULL,NULL};
	int numSpots = 0;
	
	int val;
	
	if(state->playerY-1 >= 0)
	{
		spots[numSpots] = &state->d[state->floor][state->playerY-1][state->playerX];
		numSpots++;
	}
	
	if(state->playerY+1 < state->dungeonSize)
	{
		spots[numSpots] = &state->d[state->floor][state->playerY+1][state->playerX];
		numSpots++;
	}
	
	if(state->playerX+1 < state->dungeonSize)
	{
		spots[numSpots] = &state->d[state->floor][state->playerY][state->playerX+1];
		numSpots++;
	}
	
	if(state->playerX-1 >= 0)
	{
		spots[numSpots] = &state->d[state->floor][state->playerY][state->playerX-1];
		numSpots++;
	}
	
	int i;
	
	// check which spot has a value 
	for(i=0;i<numSpots;i++)
	{
		if(spots[i] != NULL)
		{		
			val = (*spots[i]);
			
			if(val >= A && val != E)
			{
				if(action == 1)	// if an action is being performed on the nearby block, perform it 
				{
					switch(val)
					{
						case B: // unlock door 
						
						if(state->keys > 0)
						{
							(*spots[i]) = 0; 
							
							updateStatus(UNLOCK_DOOR,state);
						}
						break;
						
						case A: // flip switch 
						(*spots[i]) = G;
						break;
						case G: // flip switch 
						(*spots[i]) = A; 						
						break;
					}
					if((*spots[i]) == A || (*spots[i]) == G)
					{
						flippedSwitch((*spots[i]),state);
					}
					
				}
				return val;
			}
		}
				
	}
	
	return 0;
}

// define dungeon based on file
void readDungeonFile(struct gameState * state, char * fileName)
{
	// the file to be read 
	FILE *readFile;
	char * fileReader = malloc(128 * sizeof(char)); 
	
	readFile = fopen(fileName,"r");
	
	int iy, iz, ix;
	int check;
	
	// check if file is opened
	if(readFile == NULL)
		throwError("ERROR:DUNGEON FILE COULD NOT BE READ");
	else
	{
		// read size of dungeon 
		fscanf(readFile,"%s",fileReader);
		state->dungeonSize = atoi(fileReader);
		
		// skip line
		fscanf(readFile,"%s",fileReader);
	
		// allocate size 
		state->d = malloc(state->dungeonSize * sizeof(int **));
		state->upStairCoords = malloc(state->dungeonSize * sizeof(int *));
		for(iz=0;iz<state->dungeonSize;iz++)
		{
			state->upStairCoords[iz] = malloc(sizeof(int[2])); // malloc stair coords 
			state->d[iz] = malloc(state->dungeonSize * sizeof(int *));
			for(iy=0;iy<state->dungeonSize;iy++)
			{	
				state->d[iz][iy] = malloc(state->dungeonSize * sizeof(int));
				for(ix=0;ix<state->dungeonSize;ix++)
					state->d[iz][iy][ix] = 0;
			}	
		}
		
		// read file data 
		for(iz=0;iz<state->dungeonSize;iz++)
		{
			for(iy=0;iy<state->dungeonSize;iy++)
			{	
				for(ix=0;ix<state->dungeonSize;ix++)
				{	
					fscanf(readFile,"%s",fileReader);
					
					// checks if character is a letter 
					if(fileReader[0] >= 'A' && (strcmp(fileReader," ") != 0))
						state->d[iz][iy][ix] = 10+(fileReader[0]-65);		
					else if(strcmp(fileReader," ") != 0)
						state->d[iz][iy][ix] = atoi(fileReader);
					
					if(state->d[iz][iy][ix] == 2)
					{
						state->upStairCoords[iz][0] = ix;
						state->upStairCoords[iz][1] = iy;
					}
				}
			}	
		}
		fclose(readFile);
	}
		
	free(fileReader);
}

// used for start of dungeon crawling 
void initDungeonFloor(struct gameState * state)
{
	// reset visibility array if it is empty/deleted
	int iz,ix,iy;
	
	// the dungeon file that needs to be read 
	char * fileName; 

	// set specific conditions based on what dungeon is being initialized 	
	switch(state->building)
	{
		default:
		case 0:
		fileName = ORIGINAL_DUNGEON;
		break;
		case 1:
		fileName = SECTOR_2_FILE;
		break;
		case 2:
		fileName = SECTOR_3_FILE;
		break; 
	}
	
	// read dungeon file 
	readDungeonFile(state,fileName);
	
	// check if dungeon array is filled with data 
	if(state->d == NULL)
		throwError("ERROR:DUNGEON DATA NOT LOADED");
	
	// allocate visible array 
	if(state->visible == NULL)
	{
		state->visible = malloc(state->dungeonSize * sizeof(int **));
		for(iz=0;iz<state->dungeonSize;iz++)
		{
			state->visible[iz] = malloc(state->dungeonSize * sizeof(int *));
			for(iy=0;iy<state->dungeonSize;iy++)
			{	
				state->visible[iz][iy] = malloc(state->dungeonSize * sizeof(int));
				for(ix=0;ix<state->dungeonSize;ix++)
					state->visible[iz][iy][ix] = 0;
			}	
		}
	}		
	
	// reset status text
	if(state->statusText != NULL)
	{
		for(iy=0;iy<state->maxStatus;iy++)
		{
			free(state->statusText[iy]);
		}
		free(state->statusText);
		state->statusText = NULL;
	}
	
	// set up status handling array 
	state->numStatusLines = 0;
	state->statusText = malloc(state->maxStatus * sizeof(char *));	
	for(iy=0;iy<state->maxStatus;iy++)
		state->statusText[iy] = NULL;
	
	// generate enemies on the first floor 
	generateEnemies(state);
	
	// generate npcs
	generateNPCs(state,state->building);
	
	// initialize dungeon display
	
	initDungeonDisplay(state);
	
	// set initial status 
	switch(state->building)
	{
		case 0:
		updateStatus(FIRST_FLOOR_TEXT,state);
		break;
	}
	
	// start dungeon crawling music 
	//switchTrack(DUNGEON_MUSIC,s);
}

// deallocate dungeon assets 
void deallocateDungeon(struct gameState * state)
{
	int iy, iz, ix;

	
	if(state->d != NULL && state->visible != NULL)
	{
		// free dungeon and visibility array 
		for(iz=0;iz<state->dungeonSize;iz++)
		{
			for(iy=0;iy<state->dungeonSize;iy++)
			{		
				free(state->d[iz][iy]);
				free(state->visible[iz][iy]);
			}
			free(state->upStairCoords[iz]);
			free(state->visible[iz]);
			free(state->d[iz]);
		}
	}
	
	// free enemies/NPCs
	if(state->activeEnemies != NULL)
		free(state->activeEnemies);
	
	//if(state->activeNPCs != NULL)
	//	free(state->activeNPCs);

	// free status
	if(state->statusText != NULL)
	{
		for(iy=0;iy<state->maxStatus;iy++)
		{
			if(state->statusText[iy] != NULL)
				free(state->statusText[iy]);
		}
		free(state->statusText);
	}
	
	state->statusText = NULL;
	state->d = NULL;
	state->visible = NULL;
	//state->activeNPCs = NULL;
	state->activeEnemies = NULL;
}

// reset after encounter or moving to a new floor
void resetDungeon(struct gameState * state)
{
	int counterx,countery;
	int i, iy, iz, ix;
	
	// clear screen
	system("cls");	

	// initialize display 
	initDungeonDisplay(state);
	
	// reset passBy variable for npcs 
	//for(i=0;i<state->numNPCs;i++)
	//	state->activeNPCs[i].passBy = 0;
	
	// switch back to dungeon track 
	//switchTrack(DUNGEON_MUSIC,s);
	
	// get dungeon status back up if in ascii mode 
	if(state->graphicsMode == 0)
		displayStatus(state);
}

// handles the player's movement in the dungeon 
void dungeonMovement(struct gameState * state)
{
	int npcNear = (npcNearby(state->playerX,state->playerY-1,state->floor,1,state) || npcNearby(state->playerX,state->playerY+1,state->floor,1,state) || npcNearby(state->playerX+1,state->playerY,state->floor,1,state) || npcNearby(state->playerX-1,state->playerY,state->floor,1,state));
	
	// input handling with moving the player and other commands 
	switch(state->input)
	{
		// movement 
		case UP:
		state->direction = 0;
		if(state->playerY > 0 && passableBlock(state->playerX,state->playerY-1,state) && !npcNearby(state->playerX,state->playerY-1,state->floor,0,state))
		{
			displayRange(state);
			state->playerY--;
			state->direction = 0;
			description(state);
			displayRange(state);
		}
		break;
		case DOWN:
		state->direction = 2;
		if(state->playerY < state->dungeonSize-1 && passableBlock(state->playerX,state->playerY+1,state) && !npcNearby(state->playerX,state->playerY+1,state->floor,0,state))
		{
			displayRange(state);
			state->playerY++;
			state->direction = 2;
			description(state);
			displayRange(state);
		}
		break;
		case LEFT:
		state->direction = 3;
		if(state->playerX > 0 && passableBlock(state->playerX-1,state->playerY,state) && !npcNearby(state->playerX-1,state->playerY,state->floor,0,state))
		{
			displayRange(state);
			state->playerX--;
			state->direction = 3;
			description(state);
			displayRange(state);
		}
		break;
		case RIGHT:
		state->direction = 1;
		if(state->playerX < state->dungeonSize-1 && passableBlock(state->playerX+1,state->playerY,state) && !npcNearby(state->playerX+1,state->playerY,state->floor,0,state))
		{
			displayRange(state);
			state->playerX++;
			state->direction = 1;
			description(state);
			displayRange(state);
		}	
		break;
		
		case MENU: // toggle menu 
		
		state->switchSystem = 1;
		state->switchTo = MENU_SCREEN;
		
		break;
		
		case ENTER: // interact with nearby objects
		
		switch(state->d[state->floor][state->playerY][state->playerX])
		{
			case 4: // chest
			state->d[state->floor][state->playerY][state->playerX] = 5;
			updateStatus(OPENED_CHEST,state);
			break;
			case 9: // shop 
			state->switchSystem = 1;
			state->switchTo = SHOP_SCREEN;
			break;
			default: // interacting with movable entities
			
			if(state->activeNPCs[state->nearestNPC].active == 1 && npcNear && !state->activeNPCs[state->nearestNPC].inCombat) // interact with npc
			{
				// set up menu and variables for talking 
				updateStatus(TALK_TO_NPC,state);
				
				state->activeNPCs[state->nearestNPC].talking = 1;
				
				char ** array = malloc(3 * sizeof(char*));
				array[0] = G_COMMAND;
				array[1] = Q_COMMAND;
				array[2] = P_COMMAND;
				
				initMenu(state,3,array,70,45);
				
				free(array);
				
			}	
			else if(state->activeNPCs[state->nearestNPC].active == 1 && npcNear && state->activeNPCs[state->nearestNPC].inCombat) // help npc in combat 
			{
				state->activeEnemies[state->activeNPCs[state->nearestNPC].enemyCombat].active = 0;
				state->activeEnemies[state->activeNPCs[state->nearestNPC].enemyCombat].inCombat = 0;
				
				state->activeNPCs[state->nearestNPC].inCombat = 0;
				state->activeNPCs[state->nearestNPC].numSaved++;
				
				// start encounter 
				state->switchSystem = 1;
				state->switchTo = BATTLE_SCREEN;
			}
			else // interacting with nearby objects  
			{
				switch(nearbyBlocks(state,1))
				{
					case B: // unlocking a door 
					if(state->keys > 0)
						state->keys--;
					break;
				}
			}	
			break;
		}
		
		break;
		case BACKSPACE:
		break;
		default:
				
		break;
	}
	
	// check what kind of tile the player is on 
	switch(state->d[state->floor][state->playerY][state->playerX])
	{
		case 2:
		case 3:
		system("cls");
		switch(state->d[state->floor][state->playerY][state->playerX])
		{
			case 2:
			updateStatus(WALK_UP,state);
			state->floor = state->floor+1;
			break;
			case 3:
			updateStatus(WALK_DOWN,state);
			state->floor = state->floor-1;
			break;
		}	
		
		// moving to another section based on floor position  
		if(state->floor < 0 || state->floor == state->dungeonSize) 
		{
			switch(state->building)
			{
				case 0:
				if(state->floor < 0 ) // to sector 1 
				{
					state->building = 1;
					state->floor = 14;
				}
				else // to sector 3
				{
					state->building = 2;
					state->floor = 0;
				}
				break;
				
				case 1: 
				if(state->floor >= state->dungeonSize) // to sector 0
				{
					state->building = 0;
					state->floor = 0;
				}
				
				break;
				case 2:
				if(state->floor < 0) // to sector 0
				{
					state->building = 0;
					state->floor = 9;
				}
				
				break; 
			}
			deallocateDungeon(state);
			initDungeonFloor(state);
		}
		else
		{
			generateEnemies(state);
			resetDungeon(state);
		}
		break;
	}
}

#endif