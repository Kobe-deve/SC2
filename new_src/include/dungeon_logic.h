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
		
		// read number of enemies spawned per floor 
		fscanf(readFile,"%s",fileReader);
		state->numEnemies = atoi(fileReader);
		
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
	
	// generate enemies on the first floor 
	generateEnemies(state);
	
	// initial display range 
	displayRange(state);
	
	// generate npcs
	//generateNPCs(state->building);

	// set up status handling array 
	state->numStatusLines = 0;
	state->statusText = malloc(state->maxStatus * sizeof(char *));	
	for(iy=0;iy<state->maxStatus;iy++)
		state->statusText[iy] = NULL;
	
	// set initial status 
	updateStatus(FIRST_FLOOR_TEXT,state);

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

	// allocate sprites back if they're gone 
	if(state->graphicsMode == 1 && state->images == NULL)
	{
		state->images = malloc(sizeof(struct image));
		state->numImages = 1;
			
		addImage(state,DUNGEON_SPRITE);
		state->images[0].x = SPRITE_SQUARE_SIZE;
		state->images[0].y = SPRITE_SQUARE_SIZE;
		state->images[0].scale = 2;
	}
	else if(state->graphicsMode == 0 && state->megaAlpha != 0) // if in ascii mode, reset view 
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
	
		// display visible enemies 
		for(i = 0;i<state->numEnemies;i++)
		{
			if(state->visible[state->floor][state->activeEnemies[i].y][state->activeEnemies[i].x] == 1 && state->activeEnemies[i].active == 1)
			{
				setCursor(dungeonPrintCoordX+state->activeEnemies[i].x,dungeonPrintCoordX+state->activeEnemies[i].y);
				printf("+");
			}
		}
	}
	
	// check main display range around the player 
	displayRange(state);
	
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
	// input handling with moving the player and other commands 
	switch(state->input)
	{
		// movement 
		case UP:
		state->direction = 0;
		if(state->playerY > 0 && state->d[state->floor][state->playerY-1][state->playerX] != 1 && !npcNearby(state->playerX,state->playerY-1,state->floor,0,state))
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
		if(state->playerY < state->dungeonSize-1 && state->d[state->floor][state->playerY+1][state->playerX] != 1 && !npcNearby(state->playerX,state->playerY+1,state->floor,0,state))
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
		if(state->playerX > 0 && state->d[state->floor][state->playerY][state->playerX-1] != 1 && !npcNearby(state->playerX-1,state->playerY,state->floor,0,state))
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
		if(state->playerX < state->dungeonSize-1 && state->d[state->floor][state->playerY][state->playerX+1] != 1  && !npcNearby(state->playerX+1,state->playerY,state->floor,0,state))
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
		generateEnemies(state);
		resetDungeon(state);
		break;
	}
}

#endif