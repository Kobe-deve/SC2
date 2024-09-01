// handles settings 

// defines the title screen process used
#ifndef SETTINGS_HANDLED
#define SETTINGS_HANDLED

// load settings
void loadSettings(struct gameState * state)
{		
	FILE *file;
	file = fopen(SETTINGS_FILE,"r");
	int data;
	
	if(file != NULL)
	{
		fread(&state->fullScreen, sizeof(int), 1, file);
		fread(&state->screenW, sizeof(int), 1, file);
		fread(&state->screenH, sizeof(int), 1, file);
		fread(&SCREEN_FPS, sizeof(int), 1, file);
		
		// set renderer/window based on loaded data 
		
		if(state->fullScreen)
			SDL_SetWindowFullscreen(state->window, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
			
		SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;  
		
		SDL_SetWindowSize(state->window,state->screenW,state->screenH);
		SDL_RenderSetLogicalSize(state->renderer,state->screenW,state->screenH);
		
		fclose(file);
	}
	else
		throwError("ERROR: COULD NOT READ SETTINGS FILE");
}

// save settings to file 
void saveSettings(struct gameState * state)
{
	FILE *file;
	file = fopen(SETTINGS_FILE,"w+");
	
	if(file != NULL)
	{
		fwrite(&state->fullScreen, sizeof(int), 1, file);
		fwrite(&state->screenW, sizeof(int), 1, file);
		fwrite(&state->screenH, sizeof(int), 1, file);
		fwrite(&SCREEN_FPS, sizeof(int), 1, file);
		
		fclose(file);
		
	}
	else
		throwError("ERROR: COULD NOT SAVE SETTINGS");
}

// display function for title screen 
void settingScreenDisplay(struct gameState * state)
{	
	char displayer[400];

	sprintf(displayer,"FPS: %d",SCREEN_FPS);	
	printText(displayer, 10, 288, state->fontHandler);
	
	sprintf(displayer,"Resolution: %d x %d",state->screenW,state->screenH);	
	printText(displayer, 10, 328, state->fontHandler);
}

// logic handling for title screen 
void settingScreenHandler(struct gameState * state)
{
	char ** array;
	
	if(state->options == NULL) // initialize menu used
	{
		switch(state->menuLevel)
		{
			case 0: // initial settings options
			array = malloc(6 * sizeof(char*));
			array[0] = SETTING_TEXT_F;
			array[1] = SETTING_TEXT_R;
			array[2] = SETTING_TEXT_FPS;
			array[3] = SETTING_TEXT_C;
			array[4] = SETTING_TEXT_D;
			array[5] = C_EXIT;
			
			initMenu(state,6,array,85,24);
			
			free(array);
			break;
			
			case 1: // control settings 
			break;
			
			case 2: // revert to default 
			array = malloc(2 * sizeof(char*));
			array[0] = DEF_Y;
			array[1] = DEF_N;
			
			initMenu(state,2,array,85,24);
			
			free(array);
			break;
		}
	}
	else
		handleMenu(state);
	
	// handle a specific option 
	if(state->input == ENTER)
	{		
		switch(state->menuLevel)
		{
			case 0: // initial settings menu 
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
				// switch(state->screenW)
				// {
				// 	case 1400:
				// 	state->screenW = 1200;
				// 	state->screenH = 600;
					
				// 	break;
				// 	case 1200:
				// 	state->screenW = 1400;
				// 	state->screenH = 700;
				// 	break;
				// }
				
				// SDL_SetWindowSize(state->window,state->screenW,state->screenH);
				// SDL_RenderSetLogicalSize(state->renderer,state->screenW,state->screenH);
				
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
				
				case 3: // set controls 
				break; 
				
				case 4: // revert to default 
				state->menuLevel = 2;
				deallocateMenu(state);
				
				break;
				
				case 5: // go back 	
				
				// save settings
				saveSettings(state);
				
				state->switchSystem = 1;
				state->switchTo = state->calledSystem;
				
				deallocateMenu(state);
				break;
			}
			break; 
			case 1: // controls 
			break;
			case 2: // revert to default 
			switch(state->option)
			{
				case 0:
				state->fullScreen = 0;
				state->screenW = WINDOW_WIDTH-100;
				state->screenH = WINDOW_HEIGHT-100;
				SCREEN_FPS = 40;
				
				SDL_SetWindowFullscreen(state->window, SDL_WINDOW_SHOWN);
				
				SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;  
				
				SDL_SetWindowSize(state->window,state->screenW,state->screenH);
				SDL_RenderSetLogicalSize(state->renderer,state->screenW,state->screenH);
				
				saveSettings(state);
				case 1:
				state->menuLevel = 0;
				deallocateMenu(state);
				break;
			}
			break; 
		}
	}
}

#endif
