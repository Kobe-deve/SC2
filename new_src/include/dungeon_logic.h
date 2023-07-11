// for handling dungeon logic 
#include "base/information/filenames.h"

#ifndef DUNGEON_LOGIC_HANDLED
#define DUNGEON_LOGIC_HANDLED

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

// generate enemies on the floor 
void generateEnemies(struct gameState * state)
{
	// number of spawn points specifically on the map
	int numSpawnPoints = 0;
	int ix,iy;
	int i = 0;
	
	// set enemies on floor
	if(state->activeEnemies != NULL)
	{
		free(state->activeEnemies);
		state->activeEnemies = NULL;
	}
	
	// variable used to check how many predetermined spawn points there are vs generated number from file 
	int numGenerate = state->numEnemies;
	
	// allocate memory
	state->activeEnemies = malloc(state->numEnemies * sizeof(struct enemies));
	
	// check if the map has any specific enemy spawn points 
	for(iy=0;iy<state->dungeonSize;iy++)
	{	
		for(ix=0;ix<state->dungeonSize;ix++)
		{	
			if(state->d[state->floor][iy][ix] == E && numGenerate > 0)
			{
				state->activeEnemies[numGenerate-1].speed = rand()%3+1; 
				state->activeEnemies[numGenerate-1].startTicks = 0;
				state->activeEnemies[numGenerate-1].x = ix;
				state->activeEnemies[numGenerate-1].y = iy;
				state->activeEnemies[numGenerate-1].inCombat = 0;
				
				state->activeEnemies[numGenerate-1].active = 1;
				state->activeEnemies[numGenerate-1].type = rand()%3+1;
				state->activeEnemies[numGenerate-1].npcFighting = -1;
				state->activeEnemies[numGenerate-1].health = 5;
				numGenerate--;
			}
		}
	}
	
	// if there are no specific spawn point coordinates, randomly spawn the remaining number of enemies 
	if(numGenerate > 0)
	{
		// generate enemies to specific coordinates 
		for(i = 0;i<state->numEnemies;i++)
		{
			state->activeEnemies[i].speed = rand()%3+1; 
			state->activeEnemies[i].x = rand()%state->dungeonSize;
			state->activeEnemies[i].startTicks = 0;
			state->activeEnemies[i].y = rand()%state->dungeonSize;
			state->activeEnemies[i].inCombat = 0;
			while(state->d[state->floor][state->activeEnemies[i].y][state->activeEnemies[i].x] == 1)
			{
				state->activeEnemies[i].x = rand()%state->dungeonSize;
				state->activeEnemies[i].y = rand()%state->dungeonSize;	
			}
			state->activeEnemies[i].active = 1;
			state->activeEnemies[i].npcFighting = -1;
			state->activeEnemies[i].type = rand()%3+1;
			state->activeEnemies[i].health = 5;
		}
	}
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
	//generateEnemies(s);
	
	// initial display range 
	displayRange(state);
	
	if(state->graphicsMode == 0)
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
	}
	else
	{
		// initialize images used in dungeon crawling
		state->images = malloc(sizeof(struct image));
		state->numImages = 1;
			
		addImage(state,DUNGEON_SPRITE);
		state->images[0].x = SPRITE_SQUARE_SIZE;
		state->images[0].y = SPRITE_SQUARE_SIZE;
		state->images[0].scale = 2;
		
	}
	
	// generate npcs
	//generateNPCs(state->building);

	// set up status handling array 
	state->numStatusLines = 0;
	state->statusText = malloc(state->maxStatus * sizeof(char *));	
	for(iy=0;iy<state->maxStatus;iy++)
		state->statusText[iy] = NULL;
	
	// set initial status 
	//updateStatus(FIRST_FLOOR_TEXT,s);

	// start dungeon crawling music 
	//switchTrack(DUNGEON_MUSIC,s);
}

// function to check if an npc is at a certain coordinate 
int npcNearby(int x, int y, int f, int isPlayer)
{
	/*
	int i;
	
	for(i=0;i<numNPCs;i++)
	{
		if(!activeNPCs[i].passBy && activeNPCs[i].active && (activeNPCs[i].floor == f && (y == activeNPCs[i].y && x == activeNPCs[i].x)))
		{
			// if the coordinates are for the player, set the talked variable to this npc 
			if(isPlayer)
				npcTalked = i;
			return 1;
		}
	}
	*/
	return 0;
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
		if(state->playerY > 0 && state->d[state->floor][state->playerY-1][state->playerX] != 1 && !npcNearby(state->playerX,state->playerY-1,state->floor,0))
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
		if(state->playerY < state->dungeonSize-1 && state->d[state->floor][state->playerY+1][state->playerX] != 1 && !npcNearby(state->playerX,state->playerY+1,state->floor,0))
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
		if(state->playerX > 0 && state->d[state->floor][state->playerY][state->playerX-1] != 1 && !npcNearby(state->playerX-1,state->playerY,state->floor,0))
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
		if(state->playerX < state->dungeonSize-1 && state->d[state->floor][state->playerY][state->playerX+1] != 1  && !npcNearby(state->playerX+1,state->playerY,state->floor,0))
		{
			displayRange(state);
			state->playerX++;
			state->direction = 1;
			description(state);
			displayRange(state);
		}	
		break;
		
		case MENU: // toggle menu 
		break;
		
		case ENTER: // interact with nearby objects
	
		break;
		case BACKSPACE:
		break;
		default:
				
		break;
	}

}

#endif