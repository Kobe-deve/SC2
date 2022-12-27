/*
	Stone Crawler 2.0: Idle Bodies
	Demi
	12/17/2022 - 
	
	Original Stone Crawler
	1/16/2019
*/
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "state.h"
#include "music.h"
#include "event_handler.h"
#include "input.h"
#include "graphics.h"
#include "battle.h"
#include "dungeon.h"
#include "title.h"

// gcc main.c -lSDL2 -lSDL2_mixer -o main

int main()
{
	srand((unsigned)time(NULL));
	
	struct gameState state;
	
	// initialize the window 
	init();
	// initialize music handling
	initMusic(&state);
	
	// initialize listeners 
	initListeners(state.listeners,MAX_EVENTS);
	
	// register events 
	registerEvent(DISPLAY,titleScreenDisplay,state.listeners);
	registerEvent(MENU_SELECTION,menuSelection,state.listeners);
	registerEvent(LOGIC_HANDLER,titleScreenLogic,state.listeners);
	
	struct EventHandler *handlers = state.listeners[DISPLAY];
	
	int i = 0;
	
	while(state.input != 27)
	{
		// input handling
		if ( _kbhit() )
			state.input = getch();
		else
			state.input = 0;
		
		// go through events 
		for(i=0;i<MAX_EVENTS;i++)
		{
			handlers = state.listeners[i];
			if(handlers != NULL)
				handlers->mainFunction(&state);
		}
	}
	
	destroyListeners(state.listeners,MAX_EVENTS);
	
	return 0;
}