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
void mainLoopTest(struct gameState * state, int * inputCommands, int maxInput)
{
	// for loop variable 
	int i = 0;
	int j = 0;

	struct EventHandler *handlers = state->listeners[DISPLAY];
	
	// used for window handling 
	SDL_Event * e;
	const Uint8* keyStates;
	int frameRateTracker; // used for frame rate 
	int inputTimer = 0; // used for keeping input timing consistent with ASCII version 
	int colors[4]; // renderer color 
	struct image backgroundAsset;
	
	// set up SDL input handling if graphics mode enabled 
	if(graphicsMode == 1)
	{
		// initialize event handler for SDL2 events and renderer color 
		e = malloc(sizeof(SDL_Event));
		colors[0] = 0;
		colors[1] = 0;
		colors[2] = 100;
		colors[3] = 0;
		
		state->fontHandler = malloc(sizeof(struct text));
		
		// initialize background assets
		backgroundAsset = initImage(BACKGROUND_ASSET,state->renderer);
		backgroundAsset.scale = 4;
		
		// initialize text handler
		initFont(state->fontHandler, state->renderer);
	}
	
	// loop through given commands
	while(state->input != 27)
	{
		state->input = inputCommands[j];
	
		if(graphicsMode == 1) // if sprite mode enabled, clear screen 
		{
			SDL_SetRenderDrawColor(state->renderer, colors[0], colors[1], colors[2], colors[3]);
			SDL_RenderClear(state->renderer);
			frameRateTracker = SDL_GetTicks();
		
			// if in sprite mode, render background 
			backgroundAsset.angle++;
			backgroundAsset.angle%=360;
			backgroundAsset.y = 500;
			for(i=0;i<6;i++)
			{
				backgroundAsset.x = i*200;
				renderImage(&backgroundAsset,state->renderer,NULL);
			}
		}
		
		// go through and execute events 
		for(i=0;i<MAX_EVENTS;i++)
		{
			handlers = state->listeners[i];
			if(handlers != NULL)
				handlers->mainFunction(state);
		}
		
		// if sprite mode enabled, render screen and keep the frame rate 
		if(graphicsMode == 1)
		{
			SDL_RenderPresent(state->renderer);
			++(frames); // adds to frame tally
		
			int framet = SDL_GetTicks() - frameRateTracker; // for capping frame rate
			if( framet < SCREEN_TICK_PER_FRAME)
				SDL_Delay( SCREEN_TICK_PER_FRAME - framet );
		}
		
		j++;
	}
	
	// deallocate data used for sprite mode 
	if(graphicsMode == 1)
	{
		// deallocate images
		clearImages(state);
		// deallocate background image
		deallocateImage(&backgroundAsset);
		
		// deallocate font 
		deallocateFont(state->fontHandler);
		
		// deallocate renderer and window 
		SDL_DestroyRenderer(state->renderer);
		SDL_DestroyWindow(state->window);
	}
	
}

// check if init and end tests work 
void init_test(struct gameState * state)
{	
	initTest(state);
	endTest(state);
	
	setColor(15);
	printf("\nINIT TEST ");	
	setColor(10);
	printf("%c",251);
}

// test loading to title screen 
void title_test(struct gameState * state)
{
	int commands[10] = {0,0,0,0,0,0,0,0,27};
	
	initTest(state);
	mainLoopTest(state,commands,10);
	endTest(state);
	
	setColor(15);
	system("cls");
	printf("\nTITLE SCREEN TEST ");
	setColor(10);
	printf("%c",251);
}

int main(int argc, char *argv[])
{
	// set test mode 
	testMode = 1;
	graphicsMode = 1;

	srand((unsigned)time(NULL));
	
	struct gameState state;
	
	init_test(&state);
	title_test(&state);
	
	setColor(15);
}