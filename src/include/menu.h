#ifndef PMENU_HANDLED
#define PMENU_HANDLED

#ifndef MENU_HANDLED
#include "base/input.h"
#endif
 
#ifndef STATE_HANDLED
#include "base/state.h"
#endif

// free operations used for menu handling and return to dungeon/overworld 
void freeMenuHandling()
{
	
};

// main loop for handling menu operations 
void menuHandlingLoop(void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	if(s->input == ENTER)
	{
		destroyListener(LOGIC_HANDLER,s->listeners);
		registerEvent(DISPLAY,resetDungeon,s->listeners);
	}
}

// initialize the menu process 
void initPlayerMenu(struct gameState * s)
{
	// destroy all listeners to make way for ones used in menu handling 
	destroyListeners(s->listeners,MAX_EVENTS);
	
	// set specific event handlers 
	destroyListener(LOGIC_HANDLER,s->listeners);
	registerEvent(LOGIC_HANDLER,menuHandlingLoop,s->listeners);
	
	system("cls");
	
};

#endif