/*
	Stone Crawler 2.0 testing program
	Demi
	12/17/2022 - 
	
	Original Stone Crawler
	1/16/2019 - 6/9/2019
*/
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "include/base/font.h"
#include "include/base/state.h"
#include "include/base/music.h"
#include "include/base/event_handler.h"
#include "include/base/input.h"
#include "include/base/graphics.h"
#include "include/battle.h"
#include "include/dungeon.h"
#include "include/title.h"
#include "include/file_selection.h"

// initialize before running a test 
void initTest(struct gameState * state)
{
	// initialize the window 
	init(state);

	// initialize music handling
	initMusic(state);
	
	// initialize listeners 
	initListeners(state->listeners,MAX_EVENTS);
	
	// register events used at the title screen 
	registerEvent(DISPLAY,titleScreenDisplay,state->listeners);
	registerEvent(MENU_SELECTION,menuSelection,state->listeners);
	registerEvent(LOGIC_HANDLER,titleScreenLogic,state->listeners);
}

// close all procedures
void endTest(struct gameState * state)
{
	// clear data being used 
	clearState(state);
	
	// free data
	freeMenuProcess(state);
		
	freeDungeonData(state);
	freeBattleData(state);
	
	// destroy all listeners when game is done
	destroyListeners(state->listeners,MAX_EVENTS);
}

// main event loop in testing 
void mainLoopTest(struct gameState * state)
{
}

// check if init and end tests work 
void init_test(struct gameState * state)
{
	setColor(15);
	printf("\nINIT TEST ");
	
	initTest(state);
	endTest(state);
	
	setColor(10);
	printf("%c",251);
}

// test loading to title screen 
void title_test(struct gameState * state)
{
	setColor(15);
	printf("\nTITLE SCREEN TEST ");
	
	initTest(state);
	endTest(state);
	
	setColor(10);
	printf("%c",251);
}

int main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	
	struct gameState state;
	
	init_test(&state);
	title_test(&state);
	
	
	setColor(15);
}