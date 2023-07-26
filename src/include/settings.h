// handles settings 

// defines the title screen process used
#ifndef SETTINGS_HANDLED
#define SETTINGS_HANDLED

// display function for title screen 
void settingScreenDisplay(struct gameState * state)
{	
	switch(state->graphicsMode)
	{
		case 0: // ascii mode 
		
		break;
		case 1: // sprite mode
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
		array[2] = C_EXIT;
		
		initMenu(state,3,array,85,24);
		
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
			
			break;
			case 1:
			break;
			case 2: // go back 			
			state->switchSystem = 1;
			state->switchTo = state->calledSystem;
			
			deallocateMenu(state);
			break;
		}
	}
}

#endif
