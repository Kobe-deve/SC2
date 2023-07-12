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
	//updateStatus(FIRST_FLOOR_TEXT,s);

	// start dungeon crawling music 
	//switchTrack(DUNGEON_MUSIC,s);
}

// reset after encounter or moving to a new floor
void resetDungeon(struct gameState * s)
{
	int counterx,countery;
	int i;
	
	// clear screen
	system("cls");	

	// allocate sprites back if they're gone 
	if(s->graphicsMode == 1 && s->images == NULL)
	{
		s->images = malloc(sizeof(struct image));
		s->numImages = 1;
			
		addImage(s,DUNGEON_SPRITE);
		s->images[0].x = SPRITE_SQUARE_SIZE;
		s->images[0].y = SPRITE_SQUARE_SIZE;
		s->images[0].scale = 2;
	}
	
	// if variables used in battle aren't null, free them
	//if(s->currentBattle.enemies != NULL)
	//	free(s->currentBattle.enemies);
	//s->currentBattle.enemies = NULL;
	
	// display visible spaces and dungeon border 
	for(countery = -1;countery < s->dungeonSize+1;countery++)
	{
		for(counterx = -1;counterx < s->dungeonSize+1;counterx++)
		{
			setCursor(dungeonPrintCoordX+counterx,dungeonPrintCoordY+countery);
			if((counterx == -1 || counterx == s->dungeonSize || countery == -1 || countery == s->dungeonSize))
			{	
				setColor(BLUE);
				printf("%c",219);
			}				
			else if(s->visible[s->floor][countery][counterx] == 1)
				printf("%c",quickConvert(s->d[s->floor][countery][counterx]));
			setColor(WHITE);
		}
	}
	
	// check main display range around the player 
	displayRange(s);
	
	// display visible enemies 
	if(s->graphicsMode == 0)
	{
		for(i = 0;i<s->numEnemies;i++)
		{
			if(s->visible[s->floor][s->activeEnemies[i].y][s->activeEnemies[i].x] == 1 && s->activeEnemies[i].active == 1)
			{
				setCursor(dungeonPrintCoordX+s->activeEnemies[i].x,dungeonPrintCoordX+s->activeEnemies[i].y);
				printf("+");
			}
		}
	}
	
	// reset passBy variable for npcs 
	//for(i=0;i<s->numNPCs;i++)
	//	s->activeNPCs[i].passBy = 0;
	
	// switch back to dungeon track 
	//switchTrack(DUNGEON_MUSIC,s);
	
	// get dungeon status back up 
	//if(s->graphicsMode == 0)
	//	displayStatus(s);
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
	
	// move floors and update display if on stairs 
	if(state->d[state->floor][state->playerY][state->playerX] == 2 || state->d[state->floor][state->playerY][state->playerX] == 3)
	{	
		system("cls");
		switch(state->d[state->floor][state->playerY][state->playerX])
		{
			case 2:
			//updateStatus(WALK_UP,s);
			state->floor = state->floor+1;
			break;
			case 3:
			//updateStatus(WALK_DOWN,s);
			state->floor = state->floor-1;
			break;
		}	
		//generateEnemies(s);
		resetDungeon(state);
	}
}

#endif