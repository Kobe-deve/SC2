#ifndef STATE_HANDLED
#include "base/state.h"
#endif

#ifndef MUSIC_HANDLED
#include "base/music.h"
#endif

#ifndef EVENT_HANDLING
#include "base/event_handler.h"
#endif

#ifndef MENU_HANDLED
#include "base/input.h"
#endif

#ifndef GRAPHICS
#include "base/graphics.h"
#endif

#ifndef DUNGEON_HANDLED
#include "dungeon.h"
#endif

#ifndef SHOP_HANDLED
#define SHOP_HANDLED

// free data used by the shop and return to dungeon 
void freeShop(struct gameState * s)
{
	// free all functions
	destroyListeners(s->listeners,MAX_EVENTS);
	
	// return to dungeon 
	registerEvent(DISPLAY,resetDungeon,s->listeners);
};

// shop logic 
void shopLogic(void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	if(s->input == ENTER)
		freeShop(s);
};

// start shop operations 
void initShop(struct gameState * s)
{
	// destroy all listeners to make way for ones used in the shop  
	destroyListeners(s->listeners,MAX_EVENTS);
	destroyListener(LOGIC_HANDLER,s->listeners);
	destroyListener(DISPLAY,s->listeners);
	destroyListener(MENU_SELECTION,s->listeners);

	// register main loop for shop logic 
	registerEvent(DISPLAY,shopLogic,s->listeners);
	
	// display shop 
	system("cls");
	printPattern(SHOP,0,0,80,20);
	
};

#endif