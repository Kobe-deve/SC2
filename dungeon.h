#include "state.h"
#include "music.h"
#include "event_handler.h"
#include "input.h"
#include "graphics.h"

int x = 0;
int y = 0;

int bX = 10;
int bY = 10;
int fired = 0;

int direction = 0;
int bD = 0;

Uint32 startTicks = 1;
Uint32 capTicks = 0;
int frames = 0;

// coord to print dungeon at 
int dungeonPrintCoordX = 10;
int dungeonPrintCoordY = 10;

int d[20][20] = {0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,
			   0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,
			   0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,
			   0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,
			   0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,
			   0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
			   0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
			   0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			   0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
			   0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
			   0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,
			   0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,
			   0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,
			   0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,
			   0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,
			   0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,
			   0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,
			   0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,
			   0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,
			   0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0};

int dungeonSize = 20;

void clearDisplay(struct gameState * s)
{	
	if(s->input != 0)
	{
		setCursor(dungeonPrintCoordX+x,dungeonPrintCoordY+y);
		switch(d[y][x])
		{
			case 1:	
			printf("%c",219);
			break;
			case 0:
			printf("%c",178);	
			break;
		}
	}
}

void logic(struct gameState * s)
{
	switch(s->input)
	{
		case UP:
		direction = 0;
		if(y > 0)
			y--;
		break;
		case DOWN:
		direction = 2;
		if(y < dungeonSize-1)
			y++;
		break;
		case LEFT:
		direction = 3;
		if(x > 0)
			x--;
		break;
		case RIGHT:
		direction = 1;
		if(x < dungeonSize-1)
			x++;
		break;
		case ENTER:
		break;
	}
	
	if(((int)(SDL_GetTicks() - startTicks)) % 1000 == 0)
	{
		setCursor(dungeonPrintCoordX+bX,dungeonPrintCoordX+bY);
		switch(d[bY][bX])
		{
			case 1:	
			printf("%c",219);
			break;
			case 0:
			printf("%c",178);	
			break;
		}
		bD = rand()%4;
		
		switch(bD)
		{
			case 0:
			if(bY > 0 && d[bY-1][bX] != 1)
				bY--;
			break;
			case 1:
			if(bX < dungeonSize-1 && d[bY][bX+1] != 1)
				bX++;
			break;
			case 2:
			if(bY < dungeonSize-1 && d[bY+1][bX] != 1)
				bY++;
			break;
			case 3:
			if(bX > 0 && d[bY][bX-1] != 1)
				bX--;
			break;
		}
		startTicks = 1;
	}
}

void display()
{
	// display player
	setCursor(dungeonPrintCoordX+x,dungeonPrintCoordY+y);
	printf("%c",1);	
	
	setCursor(dungeonPrintCoordX+bX,dungeonPrintCoordX+bY);
	printf("+");
}

// main dungeon crawling function 
void walkAround(void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	capTicks = 0;
	
	clearDisplay(s);
	logic(s);
	display();
	
	++frames;
	int framet = SDL_GetTicks() - capTicks;
	if( framet < SCREEN_TICK_PER_FRAME)
		pause( SCREEN_TICK_PER_FRAME - framet );
}

// display the dungeon floor initially
void displayDungeon(void *data)
{		
	struct gameState * s = (struct gameState *)data;
	
	int counterx,countery;
	for(countery = 0;countery < dungeonSize;countery++)
	{
		for(counterx = 0;counterx < dungeonSize;counterx++)
		{
			setCursor(dungeonPrintCoordX+counterx,dungeonPrintCoordY+countery);
			if(d[countery][counterx] == 1)
				printf("%c",219);
			else
				printf("%c",178);	
		}
	}
	
	// set to main loop of dungeon 
	destroyListener(DISPLAY,s->listeners);
	registerEvent(DISPLAY,walkAround,s->listeners);
}