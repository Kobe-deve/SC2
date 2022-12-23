#include "state.h"
#include "music.h"
#include "event_handler.h"
#include "input.h"
#include "graphics.h"

#ifndef DUNGEON_HANDLED
#define DUNGEON_HANDLED

int bX = 8;
int bY = 8;
int fired = 0;

int direction = 0;
int bD = 0;

Uint32 startTicks = 1;
Uint32 capTicks = 0;
int frames = 0;

// coord to print dungeon at 
int dungeonPrintCoordX = 10;
int dungeonPrintCoordY = 10;

struct enemies 
{
	int visible;
	int x;
	int y;
};

int d[10][10][10] = {  {0,0,0,1,0,0,0,0,4,0,
						0,0,0,1,0,1,1,1,1,1,
						0,0,0,28,0,0,0,0,0,0,
						0,0,0,0,0,1,0,0,0,0,
						A,0,0,9,1,0,0,0,0,0,
						1,1,1,1,1,0,0,0,2,0,
						0,4,0,0,1,0,0,0,0,0,
						0,4,4,0,0,0,0,0,0,0,
						1,1,0,1,0,0,0,0,0,0,
						4,4,0,0,0,C,0,0,D,0},
						
						{4,0,0,0,0,0,0,0,0,0,
						 1,1,1,1,0,0,0,0,2,0,
						 4,4,1,0,0,0,0,0,0,0,
						 4,9,1,0,1,1,1,1,1,1,
						 0,0,1,0,1,0,0,0,0,B,
						 1,0,1,0,1,0,1,0,0,0,
						 0,0,1,0,1,0,1,0,3,0,
					 	 0,0,1,0,1,0,1,0,0,0,
						 0,0,1,0,1,0,0,1,1,1,
						 0,0,0,0,0,0,0,0,0,4},
						
						{0,0,0,0,0,0,0,0,0,B,
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
						 B,0,1,0,1,0,0,0,0,0,
						 0,0,0,0,1,0,0,0,A,0},
						
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
						 0,0,0,1,1,1,1,1,A,0,
						 0,1,1,1,0,0,0,1,4,4,
						 0,0,0,1,0,0,0,1,1,1,
						 1,1,0,1,0,1,3,1,4,4,
						 0,0,0,1,0,1,1,1,4,4,
						 0,1,1,1,0,1,0,0,0,0,
						 0,0,0,0,0,0,0,0,0,0},
						
						{ 0,0,0,0,0,0,0,0,0,3,
						  0,0,1,1,1,1,1,1,0,0,
						  0,1,2,0,0,0,0,0,1,1,
						  0,1,0,0,A,1,1,0,0,0,
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
						  0,1,A,0,0,0,1,0,0,0,
						  0,0,1,1,1,1,1,0,0,0,
						  0,0,0,0,0,0,0,0,0,0},
						 
						 {0,0,0,1,0,0,0,0,2,0,
						  0,2,0,1,0,0,1,0,0,0,
						  0,0,A,1,0,0,1,0,0,0,
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

int dungeonSize = 10;

int ** visible = NULL;

void displayDungeon(void *data);

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
		
		default:
		return 178;
		break;
	}
}

// display cone 
void displayRange(struct gameState * s)
{
	int x,y;
	for(y=s->playerY-2;y<s->playerY+3;y++)
	{
		for(x=s->playerX-2;x<s->playerX+3;x++)
		{
			if(!(x == s->playerX && y == s->playerY))
			{
				if(y >= 0 && x >= 0 && y < dungeonSize && x < dungeonSize)
				{
					setCursor(dungeonPrintCoordX+x,dungeonPrintCoordY+y);
					if(visible[y][x] == 0)
					{
						visible[y][x] = 1;
						printf("%c",quickConvert(d[s->floor][y][x]));
					}
				}
				else
				{
					setCursor(dungeonPrintCoordX+x,dungeonPrintCoordY+y);
					setColor(BLUE);
					printf("%c",219);
				}
			}
			setColor(WHITE);
		}
	}
}

// clear display 
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
	if(((int)(SDL_GetTicks() - startTicks)) % 1000 == 0)
	{
		if(visible[bY][bX] == 1)
		{
			setCursor(dungeonPrintCoordX+bX,dungeonPrintCoordX+bY);
			printf("%c",quickConvert(d[s->floor][bY][bX]));	
			bD = rand()%4;
		}	
		
		switch(bD)
		{
			case 0:
			if(bY > 0 && d[s->floor][bY-1][bX] != 1)
				bY--;
			break;
			case 1:
			if(bX < dungeonSize-1 && d[s->floor][bY][bX+1] != 1)
				bX++;
			break;
			case 2:
			if(bY < dungeonSize-1 && d[s->floor][bY+1][bX] != 1)
				bY++;
			break;
			case 3:
			if(bX > 0 && d[s->floor][bY][bX-1] != 1)
				bX--;
			break;
		}
			
		
		startTicks = 1;
		
		// display enemy 
		if(visible[bY][bX] == 1)
		{
			setCursor(dungeonPrintCoordX+bX,dungeonPrintCoordX+bY);
			printf("+");
		}
	}
}

// logic handling in dungeon section for moving player/npcs/etc
void logic(struct gameState * s)
{
	switch(s->input)
	{
		case UP:
		direction = 0;
		if(s->playerY > 0 && d[s->floor][s->playerY-1][s->playerX] != 1)
		{
			displayRange(s);
			s->playerY--;
			direction = 0;
		}
		break;
		case DOWN:
		direction = 2;
		if(s->playerY < dungeonSize-1 && d[s->floor][s->playerY+1][s->playerX] != 1)
		{
			displayRange(s);
			s->playerY++;
			direction = 2;
		}
		break;
		case LEFT:
		direction = 3;
		if(s->playerX > 0 && d[s->floor][s->playerY][s->playerX-1] != 1)
		{
			displayRange(s);
			s->playerX--;
			direction = 3;
		}
		break;
		case RIGHT:
		direction = 1;
		if(s->playerX < dungeonSize-1 && d[s->floor][s->playerY][s->playerX+1] != 1)
		{
			displayRange(s);
			s->playerX++;
			direction = 1;
		}
		break;
		case ENTER:
		break;
	}
	
	enemyHandler(s);
}

// display movable elements in dungeon 
void display(struct gameState * s)
{
	// display player
	setCursor(dungeonPrintCoordX+s->playerX,dungeonPrintCoordY+s->playerY);
	printf("%c",1);	
	
	setCursor(60,10);
	printf("FLOOR: %d",s->floor);
	setCursor(60,15);
	printf("VALUE: %d  ",d[s->floor][s->playerY][s->playerX]);
}

// main dungeon crawling function 
void walkAround(void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	capTicks = 0;
	
	clearDisplay(s);
	logic(s);
	
	// move floors and update display 
	if(d[s->floor][s->playerY][s->playerX] == 2 || d[s->floor][s->playerY][s->playerX] == 3)
	{	
		switch(d[s->floor][s->playerY][s->playerX])
		{
			case 2:
			s->floor = s->floor+1;
			system("cls");
			destroyListener(DISPLAY,s->listeners);
			registerEvent(DISPLAY,displayDungeon,s->listeners);
			break;
			case 3:
			s->floor = s->floor-1;
			system("cls");
			destroyListener(DISPLAY,s->listeners);
			registerEvent(DISPLAY,displayDungeon,s->listeners);
			break;
		}	
	}
	else
	{
		display(s);
			
		++frames;
		int framet = SDL_GetTicks() - capTicks;
		if( framet < SCREEN_TICK_PER_FRAME)
			pause( SCREEN_TICK_PER_FRAME - framet );
	}
		
}

// display the dungeon floor initially
void displayDungeon(void *data)
{		
	struct gameState * s = (struct gameState *)data;
	
	int counterx,countery;
	/*for(countery = -1;countery < dungeonSize+1;countery++)
	{
		for(counterx = -1;counterx < dungeonSize+1;counterx++)
		{
			setCursor(dungeonPrintCoordX+counterx,dungeonPrintCoordY+countery);
			if(counterx == -1 || counterx == dungeonSize || countery == -1 || countery == dungeonSize)
			{	
				setColor(BLUE);
				printf("%c",219);
			}				
			else
				printf("%c",quickConvert(d[s->floor][countery][counterx]));
			setColor(WHITE);
		}
	}*/
	
	if(visible != NULL)
	{
		free(visible);
		visible = NULL;
	}
	
	visible = malloc(dungeonSize * sizeof(int *));
	
	int ix,iy;
	for(iy=0;iy<dungeonSize;iy++)
	{	
		visible[iy] = malloc(dungeonSize * sizeof(int));
		for(ix=0;ix<dungeonSize;ix++)
			visible[iy][ix] = 0;
	}
	
	displayRange(s);
	
	// set to main loop of dungeon 
	destroyListener(DISPLAY,s->listeners);
	registerEvent(DISPLAY,walkAround,s->listeners);
}

#endif