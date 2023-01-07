/*
	Stone Crawler 2.0: Idle Bodies
	Demi
	12/17/2022 - 
	
	Original Stone Crawler
	1/16/2019 - 6/9/2019
*/
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "include/base/state.h"
#include "include/base/music.h"
#include "include/base/event_handler.h"
#include "include/base/input.h"
#include "include/base/graphics.h"
#include "include/battle.h"
#include "include/dungeon.h"
#include "include/title.h"

// gcc main.c -lSDL2 -lSDL2_mixer -o main

// initialize new game 
void initNewGame(struct gameState * s)
{
	destroyListener(MENU_SELECTION,s->listeners);
	system("cls");
	
	destroyListener(LOGIC_HANDLER,s->listeners);
	registerEvent(DISPLAY,initDungeonFloor,s->listeners);
	
	switchTrack(DUNGEON_MUSIC,s);
	
	s->playerX = 0;
	s->playerY = 0;
	s->floor = 0;
	s->building = 0;
};

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
	
	// main loop
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
	
	// free variables based on what is being used
	if(state.listeners[DISPLAY] != NULL && (state.listeners[DISPLAY]->mainFunction == walkAround))
		freeDungeonData(&state);
	
	// destroy all listeners when game is done
	destroyListeners(state.listeners,MAX_EVENTS);
	
	return 0;
}