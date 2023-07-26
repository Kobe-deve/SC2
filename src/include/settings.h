// handles settings 

// defines the title screen process used
#ifndef SETTINGS_HANDLED
#define SETTINGS_HANDLED

// display function for title screen 
void settingScreenDisplay(struct gameState * state)
{	
	char displayer[400];

	switch(state->graphicsMode)
	{
		case 0: // ascii mode 
		
		break;
		case 1: // sprite mode
		sprintf(displayer,"FPS: %d",SCREEN_FPS);	
		printText(displayer, 10, 288, state->fontHandler);
		
		sprintf(displayer,"Resolution: %d x %d",state->screenW,state->screenH);	
		printText(displayer, 10, 328, state->fontHandler);
		break;
	}
}

// logic handling for title screen 
void settingScreenHandler(struct gameState * state)
{
	char ** array;
	
	if(state->options == NULL) // initialize menu used
	{
		array = malloc(3 * sizeof(char*));
		array[0] = SETTING_TEXT_F;
		array[1] = SETTING_TEXT_R;
		array[2] = SETTING_TEXT_FPS;
		array[3] = C_EXIT;
		
		initMenu(state,4,array,85,24);
		
		free(array);
	}
	else
		handleMenu(state);
	
	if(state->input == ENTER)
	{		
		// handle a specific option 
		switch(state->option)
		{
			case 0: // full screen 
			state->fullScreen = !state->fullScreen;
			
			// fullscreen toggle
			if(state->fullScreen)
			{
				SDL_SetWindowFullscreen(state->window, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
				
				// get current dimensions for displaying
				SDL_DisplayMode DM;
				SDL_GetCurrentDisplayMode(0, &DM);
				state->screenW = DM.w;
				state->screenH = DM.h;
			}
			else
			{
				state->screenW = WINDOW_WIDTH-100;
				state->screenH = WINDOW_HEIGHT-100;
				SDL_SetWindowFullscreen(state->window, SDL_WINDOW_SHOWN);
			}
			SDL_RenderSetLogicalSize(state->renderer,state->screenW,state->screenH);
			
			break;
			
			case 1: // resolution 
			switch(state->screenW)
			{
				case 1400:
				state->screenW = 1200;
				state->screenH = 600;
				
				break;
				case 1200:
				state->screenW = 1400;
				state->screenH = 700;
				break;
			}
			
			SDL_SetWindowSize(state->window,state->screenW,state->screenH);
			SDL_RenderSetLogicalSize(state->renderer,state->screenW,state->screenH);
			
			break;
			
			case 2: // fps 
			switch(SCREEN_FPS)
			{
				case 20:
				SCREEN_FPS = 30;
				break;
				case 30:
				SCREEN_FPS = 40;
				break;
				case 40:
				SCREEN_FPS = 60;
				break;
				case 60:
				SCREEN_FPS = 20;
				break;
			}
			SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;  
			break;
			
			case 3: // go back 			
			state->switchSystem = 1;
			state->switchTo = state->calledSystem;
			
			deallocateMenu(state);
			break;
		}
	}
}

#endif
