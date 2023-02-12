/*
	Stone Crawler 2.0: Idle Bodies
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

// windres main.rs -o main.o 
// gcc main.c main.o -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o "Stone Crawler 2"

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

int main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	
	struct gameState state;
	
	// initialize the window 
	init(&state);
	// initialize music handling
	initMusic(&state);
	
	// initialize listeners 
	initListeners(state.listeners,MAX_EVENTS);
	
	// register events used at the title screen 
	registerEvent(DISPLAY,titleScreenDisplay,state.listeners);
	registerEvent(MENU_SELECTION,menuSelection,state.listeners);
	registerEvent(LOGIC_HANDLER,titleScreenLogic,state.listeners);
	
	struct EventHandler *handlers = state.listeners[DISPLAY];
	
	// for loop variable 
	int i = 0;
	
	// used for window handling 
	SDL_Event * e;
	const Uint8* keyStates;
	int frameRateTracker;
	int colors[4];
	
	// set up SDL input handling if graphics mode enabled 
	if(graphicsMode == 1)
	{
		// initialize event handler for SDL2 events and renderer color 
		e = malloc(sizeof(SDL_Event));
		colors[0] = 255;
		colors[1] = 255;
		colors[2] = 255;
		colors[3] = 0;
		
		state.fontHandler = malloc(sizeof(struct text));
		
		// initialize text handler
		initFont(state.fontHandler, state.renderer);
	}
	
	// main loop
	while(state.input != 27)
	{
		// if sprite mode enabled, clear screen 
		if(graphicsMode == 1)
		{
			SDL_SetRenderDrawColor(state.renderer, colors[0], colors[1], colors[2], colors[3]);
			SDL_RenderClear(state.renderer);
			frameRateTracker = SDL_GetTicks();
		}
		
		// input handling based on mode 
		switch(graphicsMode)
		{
			case 0:
			if ( _kbhit() )
				state.input = getch();
			else
				state.input = 0;
			break;

			case 1:
			keyStates = SDL_GetKeyboardState(NULL);
			
			while(SDL_PollEvent(e)) 
			{
				switch(e->type)
				{
					case SDL_WINDOWEVENT_MINIMIZED:
						while (SDL_WaitEvent(e))
						{
							if (e->window.event == SDL_WINDOWEVENT_RESTORED)
							{
								break;
							}
						}
					break;
					
					case SDL_JOYBUTTONDOWN: // for controller input
					
						switch(e->jbutton.button)
						{
							case SDL_CONTROLLER_BUTTON_DPAD_UP:
								state.input=72;
							break;
							case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
								state.input=80;
							break;
							case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
								state.input=75;
							break;
							case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
								state.input=77;
							break;
							case SDL_CONTROLLER_BUTTON_A:
								state.input=13;
							break;
							case SDL_CONTROLLER_BUTTON_B:
								state.input=8;
							break;
						}
					
					break;
					
					case SDL_KEYDOWN: // for keyboard input
						switch(e->key.keysym.sym)
						{
							case SDLK_RETURN:
								state.input = 13;
							break;
							case SDLK_BACKSPACE:
								state.input = 8;
							break;
						
							case SDLK_d:
							case SDLK_RIGHT:
								state.input = 77;
							break;
						
							case SDLK_a:
							case SDLK_LEFT:
								state.input = 75;
							break;
						
							case SDLK_w:
							case SDLK_UP:
								state.input = 72;
							break;
						
							case SDLK_m:
								state.input = 109;
							break;
							
							case SDLK_s:
							case SDLK_DOWN:
								state.input = 80;
							break;
							
							case SDLK_ESCAPE:
								state.input = 27;
							break;
						}
					break;
	
					case SDL_WINDOWEVENT_CLOSE:
					case SDL_QUIT: // clicking the x window button
						state.input = 27;
					break;
					
					default:
					break;
				}
			
			}				
			break;
		}
		
		// go through events 
		for(i=0;i<MAX_EVENTS;i++)
		{
			handlers = state.listeners[i];
			if(handlers != NULL)
				handlers->mainFunction(&state);
		}
		
		// if sprite mode enabled, render screen and keep the frame rate 
		if(graphicsMode == 1)
		{
			printText("STONE CRAWLER 2 - FUCKING FINALLY", 100, 100, state.fontHandler);
			
			SDL_RenderPresent(state.renderer);
			++(frames); // adds to frame tally
		
			int framet = SDL_GetTicks() - frameRateTracker; // for capping frame rate
			if( framet < SCREEN_TICK_PER_FRAME)
				SDL_Delay( SCREEN_TICK_PER_FRAME - framet );
		}
	}
	
	// deallocate data used for sprite mode 
	if(graphicsMode == 1)
	{
		deallocateFont(state.fontHandler);
		
		SDL_DestroyRenderer(state.renderer);
		SDL_DestroyWindow(state.window);
	}
	
	
	// clear data being used 
	clearState(&state);
	
	// free data
	if(state.listeners[MENU_SELECTION] != NULL)
		freeMenuProcess(&state);
		
	freeDungeonData(&state);
	freeBattleData(&state);
	
	// destroy all listeners when game is done
	destroyListeners(state.listeners,MAX_EVENTS);
	
	return 0;
}