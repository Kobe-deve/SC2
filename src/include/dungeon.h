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
	int x;
	int y;
	int active;
	int type;
	Uint32 startTicks;
};

// array of visible tiles in an area
int *** visible = NULL;

// width/height of the current dungeon
int dungeonSize = 10;

int numEnemies = 0;
struct enemies * activeEnemies = NULL;

// status text lines 
int maxStatus = 10; // max number of status lines visible 
char ** statusText = NULL;
int numStatusLines = 0;

int d[10][10][10] = {  {0,0,0,1,0,0,0,0,4,0,
						0,0,0,1,0,1,1,1,1,1,
						0,0,0,28,0,0,0,0,0,0,
						0,0,0,0,0,1,0,0,0,0,
						0,0,0,9,1,0,0,0,0,0,
						1,1,1,1,1,0,0,0,2,0,
						0,4,0,0,1,0,0,0,0,0,
						0,4,4,0,0,0,0,0,0,0,
						1,1,0,1,0,0,0,0,0,0,
						4,4,0,0,0,0,0,0,0,0},
						
						{4,0,0,0,0,0,0,0,0,0,
						 1,1,1,1,0,0,0,0,2,0,
						 4,4,1,0,0,0,0,0,0,0,
						 4,9,1,0,1,1,1,1,1,1,
						 0,0,1,0,1,0,0,0,0,0,
						 1,0,1,0,1,0,1,0,0,0,
						 0,0,1,0,1,0,1,0,3,0,
					 	 0,0,1,0,1,0,1,0,0,0,
						 0,0,1,0,1,0,0,1,1,1,
						 0,0,0,0,0,0,0,0,0,4},
						
						{0,0,0,0,0,0,0,0,0,0,
						 0,0,1,1,1,0,0,0,0,0,
						 0,0,0,0,1,9,0,0,3,0,
						 0,0,0,0,1,1,1,1,1,1,
						 0,0,0,0,0,0,0,0,0,0,
						 1,1,1,1,0,0,0,0,0,0,
						 0,0,4,1,0,0,0,0,0,0,
						 0,0,0,1,0,1,1,1,1,1,
						 0,2,0,0,0,1,0,4,4,0,
						 0,0,0,1,0,0,0,4,4,0},
						
						{1,1,4,4,0,0,0,0,0,0,
						 0,1,4,4,0,0,0,0,0,0,
						 0,1,1,1,1,1,1,1,1,0,
						 0,0,0,0,0,0,0,0,0,0,
						 1,1,1,0,1,1,1,0,1,1,
						 4,0,0,0,1,0,1,0,1,4,
						 1,1,1,0,1,0,1,0,1,0,
						 0,3,1,0,1,0,1,0,2,0,
						 0,0,1,0,1,0,0,0,0,0,
						 0,0,0,0,1,0,0,0,0,0},
						
						{0,1,0,0,0,0,0,0,0,0,
						 0,0,0,0,0,0,0,0,0,0,
						 0,1,0,0,0,0,0,0,0,0,
						 0,1,0,0,0,0,0,0,0,0,
						 0,1,0,1,1,1,1,1,0,0,
						 0,1,0,0,0,0,2,1,20,0,
						 0,1,1,0,1,0,0,1,0,0,
						 0,4,1,0,1,1,1,1,0,0,
						 0,9,1,0,0,0,4,1,3,0,
						 0,4,1,0,0,0,4,1,0,0},
						 		
						{4,0,0,0,0,0,0,0,0,0,
						 1,1,1,0,0,0,0,1,0,2,
						 0,0,0,0,0,0,0,1,0,0,
						 0,0,0,1,1,1,1,1,0,0,
						 0,1,1,1,0,0,0,1,4,4,
						 0,0,0,1,0,0,0,1,1,1,
						 1,1,0,1,0,1,3,1,4,4,
						 0,0,0,1,0,1,1,1,4,4,
						 0,1,1,1,0,1,0,0,0,0,
						 0,0,0,0,0,0,0,0,0,0},
						
						{ 0,0,0,0,0,0,0,0,0,3,
						  0,0,1,1,1,1,1,1,0,0,
						  0,1,2,0,0,0,0,0,1,1,
						  0,1,0,0,0,1,1,0,0,0,
						  0,1,1,1,1,0,0,1,0,0,
						  0,0,0,0,0,2,0,1,0,0,
						  0,1,1,1,0,0,0,1,0,0,
						  0,1,0,0,1,1,1,0,0,0,
						  0,1,4,4,0,0,0,0,0,0,
						  0,1,9,0,0,0,0,0,0,0},
						 
						 {0,0,0,0,0,0,0,0,0,4,
						  0,0,0,0,0,0,0,0,1,1,
						  0,0,0,1,1,0,0,0,0,0,
						  0,0,3,1,0,0,0,0,0,0,
						  0,0,0,1,0,1,1,1,1,1,
						  0,0,1,0,0,0,3,1,0,0,
						  0,0,1,0,2,1,1,0,0,0,
						  0,1,0,0,0,0,1,0,0,0,
						  0,0,1,1,1,1,1,0,0,0,
						  0,0,0,0,0,0,0,0,0,0},
						 
						 {0,0,0,1,0,0,0,0,2,0,
						  0,2,0,1,0,0,1,0,0,0,
						  0,0,0,1,0,0,1,0,0,0,
						  0,0,0,1,0,0,1,1,1,1,
						  0,0,0,1,0,0,0,0,0,0,
						  0,0,0,1,0,0,0,0,0,0,
						  0,0,0,1,0,1,1,1,1,0,
						  0,0,0,1,3,1,4,4,0,0,
						  0,0,0,1,1,1,0,0,0,0,
						  0,0,0,0,0,0,0,0,0,0},
						 
						 {0,3,1,4,4,4,4,1,0,3,
						  0,0,1,4,7,4,4,1,0,0,
						  0,0,1,0,0,0,0,1,0,0,
						  0,9,1,0,0,0,0,1,0,0,
						  1,1,0,0,0,0,0,1,0,0,
						  0,0,0,0,0,0,0,1,0,0,
						  0,0,0,0,1,0,0,1,0,0,
						  0,0,0,0,1,1,1,0,0,0,
						  0,0,0,0,0,0,0,0,0,0,
						  0,0,0,0,0,0,0,0,0,0},
						};

void initDungeonFloor(void *data);
void displayRange(struct gameState * s);
void generateEnemies(int numGenerate,struct gameState * s);

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
		case E:
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

// reset after encounter 
void resetDungeon(void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	int counterx,countery;
	int i;
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
	
	s->music = Mix_LoadMUS(DUNGEON_MUSIC);
	Mix_FadeInMusic(s->music, -1, 100); // fades into new music 
	
	destroyListener(DISPLAY,s->listeners);
	registerEvent(DISPLAY,walkAround,s->listeners);
	
	displayStatus();
}

// start encounter 
void startEncounter(void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	// set events to start battle processing
	destroyListener(MENU_SELECTION,s->listeners);
	destroyListener(DISPLAY,s->listeners);
	destroyListener(LOGIC_HANDLER,s->listeners);
	registerEvent(LOGIC_HANDLER,initBattle,s->listeners);
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
	int directionX, directionY;
	for(i = 0;i<numEnemies;i++)
	{
		directionY = 0;
		directionX = 0;
		if(activeEnemies[i].active == 1 && ((int)(SDL_GetTicks() - activeEnemies[i].startTicks)) % 1000 == 0)
		{
			if(visible[s->floor][activeEnemies[i].y][activeEnemies[i].x] == 1)
			{
				setCursor(dungeonPrintCoordX+activeEnemies[i].x,dungeonPrintCoordX+activeEnemies[i].y);
				printf("%c",quickConvert(d[s->floor][activeEnemies[i].y][activeEnemies[i].x]));	
			}	
			
			if(s->playerX > activeEnemies[i].x)
				directionX = 1;
			else if(s->playerX < activeEnemies[i].x)
				directionX = 3;
				
			if(s->playerY > activeEnemies[i].y)
				directionY = 1;
			else 
				directionY = 2;
			
			switch(directionY)
			{
				case 1:
				if(activeEnemies[i].y < dungeonSize-1 && d[s->floor][activeEnemies[i].y+1][activeEnemies[i].x] != 1)
					activeEnemies[i].y++;
				break;
				case 2:
				if(activeEnemies[i].y > 0 && d[s->floor][activeEnemies[i].y-1][activeEnemies[i].x] != 1)
					activeEnemies[i].y--;
				break;
			}
			
			switch(directionX)
			{
				case 1:
				if(activeEnemies[i].x < dungeonSize-1 && d[s->floor][activeEnemies[i].y][activeEnemies[i].x+1] != 1)
					activeEnemies[i].x++;
				break;
				case 3:
				if(activeEnemies[i].x > 0 && d[s->floor][activeEnemies[i].y][activeEnemies[i].x-1] != 1)
					activeEnemies[i].x--;
				break;
			}
		
			activeEnemies[i].startTicks = 1;
		
			// display enemy 
			if(visible[s->floor][activeEnemies[i].y][activeEnemies[i].x] == 1)
			{
				setCursor(dungeonPrintCoordX+activeEnemies[i].x,dungeonPrintCoordX+activeEnemies[i].y);
				printf("+");
			}
		}
		
		if(debug == 1)
		{
			setCursor(50,20+i);
			printf("ENEMY #%d: (%d, %d) STATUS:%d MOVEMENT: %d",i,activeEnemies[i].x,activeEnemies[i].y,activeEnemies[i].active,((int)(SDL_GetTicks() - activeEnemies[i].startTicks))% 1000);
		}
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
	
	// check if player has run into enemies 
	for(i=0;i<numEnemies;i++)
	{
		if(activeEnemies[i].active == 1 && activeEnemies[i].y == s->playerY && activeEnemies[i].x == s->playerX)
		{
			activeEnemies[i].active = 0;
			startEncounter(data);
			
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
		registerEvent(DISPLAY,resetDungeon,s->listeners);
		generateEnemies(0,s);
	}
	else
	{
		// display 
		display(s);
			
		// frame capping	
		++frames;
		int framet = SDL_GetTicks() - capTicks;
		if( framet < SCREEN_TICK_PER_FRAME)
			pause( SCREEN_TICK_PER_FRAME - framet );
	}
		
}

// define dungeon based on file
// TODO
void readDungeonFile()
{
}

// generate enemies on the floor 
void generateEnemies(int numGenerate, struct gameState * s)
{
	// set enemies on floor
	if(activeEnemies != NULL)
	{
		free(activeEnemies);
		activeEnemies = NULL;
	}
	
	numEnemies = numGenerate;
	activeEnemies = malloc(numEnemies * sizeof(struct enemies));
	
	// generate enemies to specific coordinates 
	int i;
	for(i = 0;i<numEnemies;i++)
	{
		activeEnemies[i].startTicks = 1;
		activeEnemies[i].x = rand()%dungeonSize;
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

// display the dungeon floor initially
void initDungeonFloor(void *data)
{		
	struct gameState * s = (struct gameState *)data;
	
	// reset visibility array if it is empty/deleted
	int iz,ix,iy;
	
	generateEnemies(0,s);
		
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
		for(iy=0;iy<numStatusLines;iy++)
		{
			free(statusText[iy]);
		}
		free(statusText);
		statusText = NULL;
	}
	
	numStatusLines = 0;
	statusText = malloc(maxStatus * sizeof(char *));	
	
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
	
	updateStatus("WELCOME TO THE DUNGEON");
}

#endif