#ifndef FILE_SELECTION_HANDLED
#define FILE_SELECTION_HANDLED

#ifndef MENU_HANDLED
#include "base/input.h"
#endif
 
#ifndef STATE_HANDLED
#include "base/state.h"
#endif

// initialize new game 
void initNewGame(struct gameState * s)
{	
	// destroy title screen logic and menu handlers 
	destroyListener(MENU_SELECTION,s->listeners);
	destroyListener(LOGIC_HANDLER,s->listeners);
	
	// start initializing dungeon process 
	registerEvent(DISPLAY,initDungeonFloor,s->listeners);
	
	// clear screen 
	system("cls");
	
	// set up initial dungeon crawling variables 
	s->protag_stats = generateCharacter(HUMAN);
	s->party = NULL;
	s->partySize = 0;
	s->track = "";
	s->playerX = 0;
	s->playerY = 0;
	s->floor = 0;
	s->building = 0;
};

#endif