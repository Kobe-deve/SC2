#ifndef STATE_HANDLED
#include "state.h"
#endif

#ifndef MUSIC_HANDLED
#include "music.h"
#endif

#ifndef EVENT_HANDLING
#include "event_handler.h"
#endif

#ifndef MENU_HANDLED
#include "input.h"
#endif

#ifndef GRAPHICS
#include "graphics.h"
#endif

#ifndef DUNGEON_HANDLED
#include "dungeon.h"
#endif

#ifndef BATTLE_HANDLED
#define BATTLE_HANDLED

// logic during battle 
void battleLogic()
{
	
}

// displaying during battle
void battleDisplay()
{
	
}

// main loop for handling battles 
void battleLoop(void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	battleDisplay();
	if(s->input == ENTER)
	{
		system("cls");
		destroyListener(DISPLAY,s->listeners);
		registerEvent(DISPLAY,resetDungeon,s->listeners);
	}
}

// start battle 
void initBattle(void *data)
{
	system("cls");
	struct gameState * s = (struct gameState *)data;
	destroyListener(LOGIC_HANDLER,s->listeners);
	registerEvent(DISPLAY,battleLoop,s->listeners);

	printPattern(M1,0,0,20,20);
}

#endif