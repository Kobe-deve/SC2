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

void clearDisplay(struct gameState * s)
{	
	if(s->input != 0)
	{
		setCursor(x,y);
		printf("  ");
	}
}

void logic(struct gameState * s)
{
	switch(s->input)
	{
		case UP:
		direction = 0;
		y--;
		break;
		case DOWN:
		direction = 2;
		y++;
		break;
		case LEFT:
		direction = 3;
		x--;
		break;
		case RIGHT:
		direction = 1;
		x++;
		break;
		case ENTER:
		if(fired == 0)
		{
			fired = 1;
			bX = x;
			bY = y;
			bD = direction;
		}
		break;
	}
	
	if(fired == 1 && ((int)(SDL_GetTicks() - startTicks)) % 100 == 0)
	{
		setCursor(bX,bY);
		printf("X");
		
		switch(bD)
		{
			case 0:
			bY--;
			break;
			case 1:
			bX++;
			break;
			case 2:
			bY++;
			break;
			case 3:
			bX--;
			break;
		}
		startTicks = 1;
		
		if(bY < 0 || bX < 0 || bX > 50 || bY > 50)
			fired = 0;
	}
}

int pause(int he) // pauses for he number of seconds
{
    time_t then;
    
    time(&then);
    while(difftime(time(NULL),then) < he);
    fflush(stdin); // clears any extra enter presses
}

void display()
{
	setCursor(x,y);
	printf("%c",1);	
	
	if(fired == 1)
	{
		setCursor(bX,bY);
		printf("+");
	
		setCursor(100,30);
		printf("X:%d Y:%d",bX,bY);
	//	printf("%d",((int)(SDL_GetTicks() - startTicks)))% 1000;
	
	}
}

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