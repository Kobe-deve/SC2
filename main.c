#include <stdio.h>
#include "state.h"
#include "music.h"
#include "event_handler.h"
#include "graphics.h"


void menuSelect(void *data)
{
	
}

void titleScreen(void *data)
{
	printPattern(TITLE,50,10,80,10);
}

int main()
{
	struct gameState state;
	
	// initialize the window 
	init();
	// initialize music handling
	initMusic(&state);
	
	/*
	for(enum sprites i = BACK_ROOM; i <= COMB; i++)
	{
		system("cls");
		if(i >= GARLAND && i <= M17)
			printPattern(i,50,10,20,20);
		if(i >= BACK_ROOM && i <= PUB)
			printPattern(i,50,10,80,10);
		if(i >= SWORD && i <= COMB)
			printPattern(i,50,10,10,10);
		getchar();
	}
	*/
	
	/*
	int x = 0;
	int y = 0;
	
	while(state.input != 27)
	{
		state.input = 0;
		state.input = inputHandler();
		
		if(state.input != 0)
		{
			setCursor(x,y);
			printf("  ");
		}
		
		switch(state.input)
		{
			case UP:
			y--;
			break;
			case DOWN:
			y++;
			break;
			case LEFT:
			x--;
			break;
			case RIGHT:
			x++;
			break;
		}
		
		setCursor(x,y);
		printf("%c",1);
	}
	*/
	
	initListeners(state.listeners,MAX_EVENTS);
	
	registerEvent(INPUT_HANDLER,inputHandler,state.listeners);
	registerEvent(DISPLAY,titleScreen,state.listeners);
	
	struct EventHandler *handlers = state.listeners[INPUT_HANDLER];
	
	int i = 0;
	
	while(state.input != 27)
	{
		state.input = 0;
		
		for(i=0;i<MAX_EVENTS;i++)
		{
			handlers = state.listeners[i];
			if(handlers != NULL)
				handlers->mainFunction(&state.input);
		}
	}
	
	destroyListeners(state.listeners,MAX_EVENTS);
	
	return 0;
}