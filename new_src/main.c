#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

// displaying errors 
void throwError(char * errorText)
{		
	HWND consoleWindow = GetConsoleWindow();
	
	// make sure command prompt is shown 
	ShowWindow(consoleWindow, SW_NORMAL);
	
	printf("%s",errorText);
	exit(0);
}

#include "include/gameloop.h"

// windres main.rs -o main.o 
// gcc main.c main.o -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o "Stone Crawler 2"

int main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));

	struct gameState state;
	
	state.graphicsMode = 0; // set graphics mode 
		
	// initialize the game state 
	init(&state);
	
	// set up SDL input handling if graphics mode enabled 
	if(state.graphicsMode == 1)
	{
		// initialize event handler for SDL2 events and renderer color 
		state.e = malloc(sizeof(SDL_Event));
		state.colors[0] = 0;
		state.colors[1] = 0;
		state.colors[2] = 100;
		state.colors[3] = 0;
		
		//state.fontHandler = malloc(sizeof(struct text));
		
		// initialize background assets
		//backgroundAsset = initImage(BACKGROUND_ASSET,state.renderer);
		//backgroundAsset.scale = 4;
		
		// initialize text handler
		//initFont(state.fontHandler, state.renderer);
	}
	
	// main loop
	while(state.input != 27)
	{
		// if sprite mode enabled, clear screen
		if(state.graphicsMode == 1)
		{
			SDL_SetRenderDrawColor(state.renderer, state.colors[0], state.colors[1], state.colors[2], state.colors[3]);
			SDL_RenderClear(state.renderer);
			state.frameRateTracker = SDL_GetTicks();
			
			/*
			// if in sprite mode, render background 
			backgroundAsset.angle++;
			backgroundAsset.angle%=360;
			backgroundAsset.y = 500;
			for(i=0;i<6;i++)
			{
				backgroundAsset.x = i*200;
				renderImage(&backgroundAsset,state.renderer,NULL);
			}
			*/
		}
		
		// input handling based on mode 
		switch(state.graphicsMode)
		{
			case 0:
			if ( _kbhit() )
				state.input = getch();
			else
				state.input = 0;
			break;

			case 1:
			
			state.keyStates = SDL_GetKeyboardState(NULL);
			
			state.input = 0;
			
			while(SDL_PollEvent(state.e)) 
			{
				switch(state.e->type)
				{
					case SDL_WINDOWEVENT_MINIMIZED:
						while (SDL_WaitEvent(state.e))
						{
							if (state.e->window.event == SDL_WINDOWEVENT_RESTORED)
							{
								break;
							}
						}
					break;
					
					case SDL_JOYBUTTONDOWN: // for controller input
					
						switch(state.e->jbutton.button)
						{
							case SDL_CONTROLLER_BUTTON_DPAD_UP:
								state.input=UP;
							break;
							case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
								state.input=DOWN;
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
						switch(state.e->key.keysym.sym)
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
							
							case SDLK_h:
							/*if(debug == 1)
							{
								HWND consoleWindow = GetConsoleWindow();
								ShowWindow(consoleWindow, SW_NORMAL);
							}*/
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
		
		// game logic handling 
		logicHandler(&state);
		
		// game display handling 
		displayHandler(&state);
		
		// if sprite mode enabled, render screen and keep the frame rate 
		if(state.graphicsMode == 1)
		{
			SDL_RenderPresent(state.renderer);
		
			int framet = SDL_GetTicks() - state.frameRateTracker; // for capping frame rate
			if( framet < SCREEN_TICK_PER_FRAME)
				SDL_Delay( SCREEN_TICK_PER_FRAME - framet );
		}
	}
	
	// deallocate based on the current system being used 
	switch(state.gameSystem)
	{
		case DUNGEON_SCREEN:
		deallocateDungeon(&state);
		break;
	}
	
	deallocate(&state);	
	
	return 0;
}