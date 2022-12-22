#include <stdio.h>
#include <time.h>
#include <string.h>
#include "state.h"
#include "music.h"
#include "event_handler.h"
#include "input.h"
#include "graphics.h"
#include "dungeon.h"

// gcc main.c -lSDL2 -lSDL2_mixer -o main

// title screen functions 
void titleScreenDisplay(void *data)
{
	printPattern(TITLE,50,10,80,10);
	
	struct gameState * s = (struct gameState *)data;
	
	// initialize options 
	if(s->options == NULL)
	{
		char ** array = malloc(2 * sizeof(char*));
		array[0] = "New Game";
		array[1] = "Continue";
		
		initMenu(s,2,array,50,21);
		
		free(array);
	}
	destroyListener(DISPLAY,s->listeners);
	printf("%d",rand()%10+1);
}

// logic at the title screen 
void titleScreenLogic(void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	if(s->input == ENTER)
	{
		destroyListener(MENU_SELECTION,s->listeners);
		system("cls");
		
		destroyListener(LOGIC_HANDLER,s->listeners);
		registerEvent(DISPLAY,displayDungeon,s->listeners);
		
		s->music = Mix_LoadMUS("music/Live.mp3");
		Mix_PlayMusic(s->music, -1);
		
		s->playerX = 0;
		s->playerY = 0;
		s->floor = 0;
	}
}

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