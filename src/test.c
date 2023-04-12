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
}


// check if init and end tests work 
void init_test(struct gameState * state)
{
	setColor(15);
	printf("INIT TEST ");
	
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
	setColor(15);
}