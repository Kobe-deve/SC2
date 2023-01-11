#ifndef TITLE_HANDLED
#define TITLE_HANDLED

#include "base/state.h"
#include "base/music.h"
#include "base/event_handler.h"
#include "base/input.h"
#include "base/graphics.h"

#ifndef DUNGEON_HANDLED
#include "dungeon.h"
#endif

// title screen functions 
void titleScreenDisplay(void *data)
{
	// display title 
	printPattern(TITLE,45,10,80,10);
	
	setCursor(55,20);
	printf("Version 2.0: Idle on the Grindstone");
	setCursor(55,21);
	printf("Finality Games - Developed by Demi");
	
	struct gameState * s = (struct gameState *)data;
	
	// initialize options 
	if(s->options == NULL)
	{
		char ** array = malloc(2 * sizeof(char*));
		array[0] = "New Game";
		array[1] = "Continue";
		
		initMenu(s,2,array,85,24);
		
		free(array);
	}
	destroyListener(DISPLAY,s->listeners);
}

// logic at the title screen 
void titleScreenLogic(void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	if(s->input == ENTER) // start dungeon 
	{
		switch(s->option)
		{
			case 0:
			freeMenuProcess(s);
			initNewGame(s);
			break;
			case 1:
			
			
			break;
		}
	}
}

#endif