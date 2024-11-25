#include "include/base/sc2.h"
#include "include/gameloop.h"

// windres main.rs -o main.o 
// gcc main.c main.o -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o "Stone Crawler 2"

int main(int argc, char *argv[])
{
	int i;
	
	srand((unsigned)time(NULL));

	struct gameState state;
    
	// initialize game 	
	initializeGame(&state);

    int start_system = BATTLE_SCREEN;

    if (filePresent(MAIN_SAVE_FILE) == 1)
    {
        state.newGame = 0;
        loadData(&state);
        state.switchSystem = 1;
        state.enemyType = 1;
			
        state.switchTo = start_system;
    }
    else
    {
        printf("CANNOT FIND FILE");
        return 1;
    }

	// load settings if they're available
	if(filePresent(SETTINGS_FILE))
		loadSettings(&state);

	// main loop
	while(state.input != 27)
	{
		// clear screen
		SDL_SetRenderDrawColor(state.renderer, state.colors[0], state.colors[1], state.colors[2], state.colors[3]);
		SDL_RenderClear(state.renderer);
		state.frameRateTracker = SDL_GetTicks();
		
		// render background 
		state.backgroundAsset.angle++;
		state.backgroundAsset.angle%=360;
		state.backgroundAsset.x = state.screenW-300;
		for(i=0;i<3;i++)
		{
			state.backgroundAsset.y = (i*200)-50;
			renderImage(&state.backgroundAsset,state.renderer,NULL);
		}
		
		// input handling	
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
		
		// if switching systems, ignore input besides exiting 
		if(state.switchSystem == 1 && state.input != 27)
			state.input = 0;
				
		// game display handling 
		displayHandler(&state);
		
		// game logic handling 
		logicHandler(&state);
		
        if(state.gameSystem != start_system && state.gameSystem != TITLE_SCREEN)
        {
            state.input = 27;
        }


		// render screen and keep the frame rate 
		SDL_RenderPresent(state.renderer);
	
		int framet = SDL_GetTicks() - state.frameRateTracker; // for capping frame rate
		if( framet < SCREEN_TICK_PER_FRAME)
			SDL_Delay( SCREEN_TICK_PER_FRAME - framet );
	}
	
	// deallocate game before closing 
	deallocateGame(&state);	
	
	return 0;
}