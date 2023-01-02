#ifndef TITLE_HANDLED
#define TITLE_HANDLED

#include "state.h"
#include "music.h"
#include "event_handler.h"
#include "input.h"
#include "graphics.h"

#ifndef DUNGEON_HANDLED
#include "dungeon.h"
#endif

// title screen functions 
void titleScreenDisplay(void *data)
{
	printPattern(TITLE,10,10,80,10);
	
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
		registerEvent(DISPLAY,initDungeonFloor,s->listeners);
		
		switchTrack(DUNGEON_MUSIC,s);
		
		s->playerX = 0;
		s->playerY = 0;
		s->floor = 0;
	}
}

#endif