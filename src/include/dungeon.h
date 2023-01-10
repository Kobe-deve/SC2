#ifndef STATE_HANDLED
#include "base/state.h"
#endif

#ifndef MUSIC_HANDLED
#include "base/music.h"
#endif

#ifndef EVENT_HANDLING
#include "base/event_handler.h"
#endif

#ifndef MENU_HANDLED
#include "base/input.h"
#endif
 
#ifndef GRAPHICS
#include "base/graphics.h"
#endif

#ifndef DUNGEON_HANDLED
#define DUNGEON_HANDLED

#ifndef BATTLE_HANDLED
#include "battle.h"
#endif

int direction = 0;

Uint32 startTicks = 1;
Uint32 capTicks = 0;
int frames = 0;

// coord to print dungeon at 
int dungeonPrintCoordX = 1;
int dungeonPrintCoordY = 1;

// struct for movable enemies
struct enemies 
{
	int x; // coordinates
	int y;
	int active; // is the enemy alive 
	int type; // the type of enemy for the encounter 
	int inCombat; // is the enemy fighting an npc 
	int speed; // what interval does the enemy move 
	int startTicks;
};

struct npc
{
	int x; // coordinates 
	int y;
	int floor; // what floor the npc is on 
	int active; // is the npc alive 
	int type; // type of npc for dialogue/stats  
	int inCombat; // is the npc fighting an npc 
	int talking; // is the npc talking to the player 
	int speed; // interval the npc moves at 
	int startTicks;
};

// array of visible tiles in an area
int *** visible = NULL;

// width/height of the current dungeon
int dungeonSize = 0;

// enemy movement information for dungeon 
int numEnemies = 0;
struct enemies * activeEnemies = NULL;

// npc information for dungeon 
int numNPCs = 0;
struct npc * activeNPCs = NULL;

// status text lines 
int maxStatus = 10; // max number of status lines visible 
char ** statusText = NULL;
int numStatusLines = 0;

int *** d = NULL;

void initDungeonFloor(void *data);
void displayRange(struct gameState * s);
void generateEnemies(struct gameState * s);

// display status in dungeon 
void displayStatus()
{
	int i;
	
	setCursor(1,30);
	printf("STATUS:");
	// reset displayed order 
	for(i=0;i<numStatusLines;i++)
	{
		setCursor(1,31+i);
		printf("\33[2K");
		setCursor(1,31+i);
		printf("%s",statusText[i]);
	}
}

// update status text and display it 
void updateStatus(char * text)
{
	int i;
	
	// set order of text
	if(numStatusLines < maxStatus)
	{
		statusText[numStatusLines] = text;
		++numStatusLines;
	}
	else // move recent to bottom if max is filled 
	{
		if(strlen(statusText[0]) > strlen(text))
		{
			setColor(BLACK);
			for(i=0;i<strlen(statusText[0])+1;i++)
			{
				setCursor(1+i,21);
				printf("%c",219);
			}
			setColor(WHITE);
		}
		
		free(statusText[0]);
		for(i=0;i<numStatusLines-1;i++)
		{
			statusText[i] = statusText[i+1];
		}
		statusText[numStatusLines-1] = text;
	}
	
	displayStatus();
}

// briefly using this function to convert old dungeon 
int quickConvert(int x)
{
	switch(x)
	{
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
		case A:
		case B:
		case C:
		case D:
		case F:
		case G:
		case 7:
		return 1;
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
		
		default:
		return 178;
		break;
	}
}

// reset after encounter or moving to a new floor
void resetDungeon(void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	int counterx,countery;
	int i;
	
	// if variables used in battle aren't null, free them
	if(s->currentBattle.enemies != NULL)
		free(s->currentBattle.enemies);
	s->currentBattle.enemies = NULL;
	
	// display visible spaces and dungeon border 
	for(countery = -1;countery < dungeonSize+1;countery++)
	{
		for(counterx = -1;counterx < dungeonSize+1;counterx++)
		{
			setCursor(dungeonPrintCoordX+counterx,dungeonPrintCoordY+countery);
			if((counterx == -1 || counterx == dungeonSize || countery == -1 || countery == dungeonSize))
			{	
				setColor(BLUE);
				printf("%c",219);
			}				
			else if(visible[s->floor][countery][counterx] == 1)
				printf("%c",quickConvert(d[s->floor][countery][counterx]));
			setColor(WHITE);
		}
	}
	
	displayRange(s);
	
	// display visible enemies 
	for(i = 0;i<numEnemies;i++)
	{
		if(visible[s->floor][activeEnemies[i].y][activeEnemies[i].x] == 1 && activeEnemies[i].active == 1)
		{
			setCursor(dungeonPrintCoordX+activeEnemies[i].x,dungeonPrintCoordX+activeEnemies[i].y);
			printf("+");
		}
	}
	
	destroyListener(DISPLAY,s->listeners);
	registerEvent(DISPLAY,walkAround,s->listeners);
	
	displayStatus();
}

// start encounter 
void startEncounter(int type, void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	// set up battle structure based on the type of enemy 
	s->currentBattle.turns = 0;
	s->currentBattle.numEnemies = 0;
	s->currentBattle.enemies = malloc(s->currentBattle.numEnemies * sizeof(struct character));
	switch(type)
	{
		case 0:
		break;
		case 1:
		break;
		case 2:
		break;
		case 3:
		break;
	}
	
	// set events to start battle processing
	destroyListener(MENU_SELECTION,s->listeners);
	destroyListener(DISPLAY,s->listeners);
	destroyListener(LOGIC_HANDLER,s->listeners);
	registerEvent(LOGIC_HANDLER,initBattle,s->listeners);
	
	updateStatus("You encountered a curse!");
}

// display range when moving  
void displayRange(struct gameState * s)
{
	int i;
	int enemyHere = 0;
	int x,y;
	for(y=s->playerY-2;y<s->playerY+3;y++)
	{
		for(x=s->playerX-2;x<s->playerX+3;x++)
		{	
			if(!(x == s->playerX && y == s->playerY))
			{
				if((y >= 0 && x >= 0 && y < dungeonSize && x < dungeonSize))
				{
					if((visible[s->floor][y][x] == 0))
					{
						setCursor(dungeonPrintCoordX+x,dungeonPrintCoordY+y);
			
						visible[s->floor][y][x] = 1;
						
						printf("%c",quickConvert(d[s->floor][y][x]));
					}
				}
				
				setColor(WHITE);
			}
			
			// checks if any enemies are visible with the updated range
			if((y >= 0 && x >= 0 && y < dungeonSize && x < dungeonSize))
			{
				for(i = 0;i<numEnemies;i++)
				{
					if(activeEnemies[i].active == 1 && activeEnemies[i].x == x && activeEnemies[i].y == y)
					{
						setCursor(dungeonPrintCoordX+activeEnemies[i].x,dungeonPrintCoordX+activeEnemies[i].y);
						printf("+");
					}
				}
			}
		}
	}
}

// generate text status descriptions based on where the player is 
void description(struct gameState * s)
{
	// update status text based on where the player is at 
	switch(d[s->floor][s->playerY][s->playerX])
	{
		case 5:
		updateStatus("You stand near an opened empty steel chest.");
		break;
		case 4:
		updateStatus("You stand near a closed steel chest, press enter to open it.");
		break;
		case 9:
		updateStatus("You stand near a hole in the wall, press enter to go through it.");
		break;
	}
}

// clear display when player moves  
void clearDisplay(struct gameState * s)
{	
	if(s->input != 0)
	{
		setCursor(dungeonPrintCoordX+s->playerX,dungeonPrintCoordY+s->playerY);
		printf("%c",quickConvert(d[s->floor][s->playerY][s->playerX]));	
	}
}

// moves enemies on the floor 
void enemyHandler(struct gameState * s)
{
	int i = 0;
	int j,enemyHere;
	int cYo;
	int directionX, directionY;
	
	if(debug == 1)
	{
		setCursor(50,19);
		printf("ENEMIES: %d",numEnemies);
	}
	
	for(i = 0;i<numEnemies;i++)
	{
		enemyHere = 0;
		
		directionY = 0;
		directionX = 0;
		if(activeEnemies[i].active == 1 && ((int)(SDL_GetTicks() - activeEnemies[i].startTicks))/1000.f >= activeEnemies[i].speed)
		{
			// erase/update current spot when moving 
			if(visible[s->floor][activeEnemies[i].y][activeEnemies[i].x] == 1)
			{
				// make sure there isn't another enemy on this spot if updating 
				setCursor(dungeonPrintCoordX+activeEnemies[i].x,dungeonPrintCoordX+activeEnemies[i].y);
				printf("%c",quickConvert(d[s->floor][activeEnemies[i].y][activeEnemies[i].x]));	
			}	
			else
				updateStatus("A footstep echoes from the darkness...");	
			
			// set up direction variables 
			if(s->playerX > activeEnemies[i].x)
				directionX = 1;
			else if(s->playerX < activeEnemies[i].x)
				directionX = 3;
				
			if(s->playerY > activeEnemies[i].y)
				directionY = 1;
			else if(s->playerY < activeEnemies[i].y)
				directionY = 2;
			
			int cY = activeEnemies[i].y;
			int cX = activeEnemies[i].x; 
			cYo = 0;
			
			// move in y direction 
			switch(directionY)
			{
				case 1:
				if(cY < dungeonSize-1 && d[s->floor][cY+1][cX] != 1)
					activeEnemies[i].y++;
				break;
				case 2:
				if(cY > 0 && d[s->floor][cY-1][cX] != 1)
					activeEnemies[i].y--;
				break;
			}
			
			// move in x direction 
			cYo = cY;
			cY = activeEnemies[i].y;
			
			switch(directionX)
			{
				case 1:
				if(cX < dungeonSize-1 && d[s->floor][cY][cX+1] != 1)
					activeEnemies[i].x++;
				break;
				case 3:
				if(cX > 0 && d[s->floor][cY][cX-1] != 1)
					activeEnemies[i].x--;
				break;
			}
			
			// make sure enemies don't overlap when moving, if so then keep them at their spot 
			for(j=0;j<numEnemies;j++)
			{
				if(j !=i && activeEnemies[j].active == 1 && ((activeEnemies[i].y == activeEnemies[j].y) && (activeEnemies[i].x == activeEnemies[j].x)))
				{
					activeEnemies[i].x = cX;
					activeEnemies[i].y = cYo;
					break;
				}
			}
		
			// display enemy 
			if(visible[s->floor][activeEnemies[i].y][activeEnemies[i].x] == 1)
			{
				setCursor(dungeonPrintCoordX+activeEnemies[i].x,dungeonPrintCoordX+activeEnemies[i].y);
				printf("+");
			}
			
			activeEnemies[i].startTicks = SDL_GetTicks();
		}
		
		if(debug == 1)
		{
			setCursor(50,20+i);
			printf("ENEMY #%d: (%d, %d) STATUS:%d MOVEMENT: %d",i,activeEnemies[i].x,activeEnemies[i].y,activeEnemies[i].active,((int)(SDL_GetTicks()))% activeEnemies[i].speed);
		}
	}
}

// handling npcs on the floor 
void npcHandler(struct gameState * s)
{
	int i;
	
	for(i = 0;i<numNPCs;i++)
	{	
		printf("%c",233);
	}
}

// logic handling in dungeon section for moving player/npcs/etc
void dungeonLogic(void *data, struct gameState * s)
{
	int i;
	char test;
	
	switch(s->input)
	{
		case UP:
		direction = 0;
		if(s->playerY > 0 && d[s->floor][s->playerY-1][s->playerX] != 1)
		{
			displayRange(s);
			s->playerY--;
			direction = 0;
			description(s);
		}
		break;
		case DOWN:
		direction = 2;
		if(s->playerY < dungeonSize-1 && d[s->floor][s->playerY+1][s->playerX] != 1)
		{
			displayRange(s);
			s->playerY++;
			direction = 2;
			description(s);
		}
		break;
		case LEFT:
		direction = 3;
		if(s->playerX > 0 && d[s->floor][s->playerY][s->playerX-1] != 1)
		{
			displayRange(s);
			s->playerX--;
			direction = 3;
			description(s);
		}
		break;
		case RIGHT:
		direction = 1;
		if(s->playerX < dungeonSize-1 && d[s->floor][s->playerY][s->playerX+1] != 1)
		{
			displayRange(s);
			s->playerX++;
			direction = 1;
			description(s);
		}
		break;
		case ENTER:
		if(d[s->floor][s->playerY][s->playerX] == 4)
		{
			d[s->floor][s->playerY][s->playerX] = 5;
			updateStatus("You opened the chest and found... nothing wow awesome.");
		}
		else
			updateStatus("You pressed enter");
		break;
		case BACKSPACE:
		debug = !debug;
		break;
		default:
		break;
	}
	
	// move enemies 
	enemyHandler(s);
	
	// move npcs 
	//npcHandler(s);
	
	// check if player has run into enemies 
	for(i=0;i<numEnemies;i++)
	{
		if(activeEnemies[i].active == 1 && activeEnemies[i].y == s->playerY && activeEnemies[i].x == s->playerX)
		{
			activeEnemies[i].active = 0;
			startEncounter(activeEnemies[i].type,data);
			
			setCursor(dungeonPrintCoordX+activeEnemies[i].x,dungeonPrintCoordX+activeEnemies[i].y);
			printf("%c",quickConvert(d[s->floor][activeEnemies[i].y][activeEnemies[i].x]));	
		}
	}
}

// display movable elements in dungeon 
void display(struct gameState * s)
{
	// display player
	setCursor(dungeonPrintCoordX+s->playerX,dungeonPrintCoordY+s->playerY);
	printf("%c",1);	
	
	if(debug == 1)
	{
		setCursor(60,10);
		printf("FLOOR: %d",s->floor);
		setCursor(60,15);
		printf("VALUE: %d  ",d[s->floor][s->playerY][s->playerX]);
	}
}

// main dungeon crawling function 
void walkAround(void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	capTicks = 0;
	
	clearDisplay(s);
	dungeonLogic(data,s);
	
	// move floors and update display 
	if(d[s->floor][s->playerY][s->playerX] == 2 || d[s->floor][s->playerY][s->playerX] == 3)
	{	
		system("cls");
		destroyListener(DISPLAY,s->listeners);
		switch(d[s->floor][s->playerY][s->playerX])
		{
			case 2:
			updateStatus("You walk upstairs.");
			s->floor = s->floor+1;
			break;
			case 3:
			updateStatus("You walk downstairs.");
			s->floor = s->floor-1;
			break;
		}	
		generateEnemies(s);
		resetDungeon(data);
	}
	else // display 
		display(s);
		
}

// define dungeon based on file
void readDungeonFile(char * fileName)
{
	// the file to be read 
	FILE *readFile;
	char * fileReader = malloc(128 * sizeof(char)); 
	
	readFile = fopen(fileName,"r");
	
	int iy, iz, ix;
	
	// check if file is opened
	if(!readFile)
	{
		printf("ERROR:DUNGEON FILE COULD NOT BE READ");
		getchar();
		exit(0);
	}
	else
	{
		// read size of dungeon 
		fscanf(readFile,"%s",fileReader);
		dungeonSize = atoi(fileReader);
		
		// read number of enemies spawned per floor 
		fscanf(readFile,"%s",fileReader);
		numEnemies = atoi(fileReader);
		
		// skip line
		fscanf(readFile,"%s",fileReader);
		
		// allocate size 
		d = malloc(dungeonSize * sizeof(int **));
		for(iz=0;iz<dungeonSize;iz++)
		{
			d[iz] = malloc(dungeonSize * sizeof(int *));
			for(iy=0;iy<dungeonSize;iy++)
			{	
				d[iz][iy] = malloc(dungeonSize * sizeof(int));
				for(ix=0;ix<dungeonSize;ix++)
					d[iz][iy][ix] = 0;
			}	
		}
		
		// read file data 
		for(iz=0;iz<dungeonSize;iz++)
		{
			for(iy=0;iy<dungeonSize;iy++)
			{	
				for(ix=0;ix<dungeonSize;ix++)
				{	
					fscanf(readFile,"%s",fileReader);
					
					// checks if character is a letter 
					if(fileReader[0] >= 'A' && (strcmp(fileReader," ") != 0))
						d[iz][iy][ix] = 10+(fileReader[0]-65);		
					else if(strcmp(fileReader," ") != 0)
						d[iz][iy][ix] = atoi(fileReader);
				}
			}	
		}
	}
		
	fclose(readFile);
	free(fileReader);
}


// generate enemies on the floor 
void generateEnemies(struct gameState * s)
{
	// number of spawn points specifically on the map
	int numSpawnPoints = 0;
	int ix,iy;
	int i = 0;
	
	// set enemies on floor
	if(activeEnemies != NULL)
	{
		free(activeEnemies);
		activeEnemies = NULL;
	}
	
	int numGenerate = numEnemies;
	
	// allocate memory
	activeEnemies = malloc(numEnemies * sizeof(struct enemies));
	
	// check if the map has any specific enemy spawn points 
	for(iy=0;iy<dungeonSize;iy++)
	{	
		for(ix=0;ix<dungeonSize;ix++)
		{	
			if(d[s->floor][iy][ix] == E && numGenerate > 0)
			{
				activeEnemies[numGenerate-1].speed = rand()%3+1; 
				activeEnemies[numGenerate-1].startTicks = 0;
				activeEnemies[numGenerate-1].x = ix;
				activeEnemies[numGenerate-1].y = iy;
				
				activeEnemies[numGenerate-1].active = 1;
				activeEnemies[numGenerate-1].type = rand()%3+1;
				numGenerate--;
			}
		}
	}
	
	// if there are no specific spawn point coordinates, randomly spawn the remaining number of enemies 
	if(numGenerate > 0)
	{
		// generate enemies to specific coordinates 
		for(i = 0;i<numEnemies;i++)
		{
			activeEnemies[i].speed = rand()%3+1; 
			activeEnemies[i].x = rand()%dungeonSize;
			activeEnemies[i].startTicks = 0;
			activeEnemies[i].y = rand()%dungeonSize;
			while(d[s->floor][activeEnemies[i].y][activeEnemies[i].x] == 1)
			{
				activeEnemies[i].x = rand()%dungeonSize;
				activeEnemies[i].y = rand()%dungeonSize;	
			}
			activeEnemies[i].active = 1;
			activeEnemies[i].type = rand()%3+1;
		}
	}
}

// for freeing data when exiting a dungeon 
void freeDungeonData(void *data)
{
	int iy, iz, ix;

	if(d != NULL && visible != NULL)
	{
		// free dungeon and visibility array 
		for(iz=0;iz<dungeonSize;iz++)
		{
			for(iy=0;iy<dungeonSize;iy++)
			{		
				for(ix=0;ix<dungeonSize;ix++)
				{
					free(&d[iz][iy][ix]);
					free(&visible[iz][iy][ix]);
				}
				free(d[iz][iy]);
				free(visible[iz][iy]);
			}
			free(visible[iz]);
			free(d[iz]);
		}
	}
	
	// free enemies/NPCs
	if(activeEnemies != NULL)
		free(activeEnemies);
	
	if(activeNPCs != NULL)
		free(activeNPCs);

	// free status
	if(statusText != NULL)
	{
		for(iy=0;iy<maxStatus;iy++)
		{
			if(statusText[iy] != NULL)
				free(statusText[iy]);
		}
		free(statusText);
	}
	
	statusText = NULL;
	d = NULL;
	visible = NULL;
	activeNPCs = NULL;
	activeEnemies = NULL;
}

// display the dungeon floor and set up used varaiables initially
void initDungeonFloor(void *data)
{		
	struct gameState * s = (struct gameState *)data;
	
	// reset visibility array if it is empty/deleted
	int iz,ix,iy;
	
	// the dungeon file that needs to be read 
	char * fileName; 

	// set specific conditions based on what dungeon is being initialized 	
	switch(s->building)
	{
		default:
		case 0:
		fileName = ORIGINAL_DUNGEON;
		break;
	}
	
	// read dungeon file 
	readDungeonFile(fileName);
	
	// check if dungeon array is filled with data 
	if(d == NULL)
	{
		printf("ERROR:DUNGEON FILE NOT COMPATIBLE");
		getchar();
		exit(0);
	}
	
	// allocate visible array 
	if(visible == NULL)
	{
		visible = malloc(dungeonSize * sizeof(int **));
		for(iz=0;iz<dungeonSize;iz++)
		{
			visible[iz] = malloc(dungeonSize * sizeof(int *));
			for(iy=0;iy<dungeonSize;iy++)
			{	
				visible[iz][iy] = malloc(dungeonSize * sizeof(int));
				for(ix=0;ix<dungeonSize;ix++)
					visible[iz][iy][ix] = 0;
			}	
		}
	}		
	
	// reset status text
	if(statusText != NULL)
	{
		for(iy=0;iy<maxStatus;iy++)
		{
			free(statusText[iy]);
		}
		free(statusText);
		statusText = NULL;
	}
	
	// generate enemies on the first floor 
	generateEnemies(s);
	
	// initial display range 
	displayRange(s);
	
	// set to main loop of dungeon 
	destroyListener(DISPLAY,s->listeners);
	registerEvent(DISPLAY,walkAround,s->listeners);
	
	// display dungeon walls 
	for(iy = -1;iy < dungeonSize+1;iy++)
	{
		for(ix = -1;ix < dungeonSize+1;ix++)
		{
			setCursor(dungeonPrintCoordX+ix,dungeonPrintCoordY+iy);
			if((ix == -1 || ix == dungeonSize || iy == -1 || iy == dungeonSize))
			{	
				setColor(BLUE);
				printf("%c",219);
			}
			else if(visible[s->floor][iy][ix] == 1)
			{
				setColor(WHITE);
				printf("%c",quickConvert(d[s->floor][iy][ix]));
			}
		}
	}		
	setColor(WHITE);
	
	// generate npc array 
	numNPCs = 0;
	activeNPCs = malloc(maxStatus * sizeof(struct npc));

	// set up status handling array 
	numStatusLines = 0;
	statusText = malloc(maxStatus * sizeof(char *));	
	for(iy=0;iy<maxStatus;iy++)
		statusText[iy] = NULL;
	
	// set initial status 
	updateStatus("You wake up in a strange stone room, dimly lit but still dark.");

	free(fileName);
}

#endif