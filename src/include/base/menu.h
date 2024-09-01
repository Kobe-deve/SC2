// menu handler
#ifndef MENU_HANDLED
#define MENU_HANDLED

// deallocate menu assets
void deallocateMenu(struct gameState * state)
{
	if(state->options != NULL)
	{
		for(int i=0;i<state->numOptions;i++)
		{	
			free(state->options[i]);
		}	
		free(state->options);
		state->options = NULL;
	}
}

// initialize a menu 
void initMenu(struct gameState * state, int numOpts, char ** options, int x, int y)
{
	// if there's already an allocated menu, clear it 
	if(state->options != NULL)
	{
		deallocateMenu(state);
		state->options = NULL;
	}
	
	state->options = malloc(numOpts * sizeof(char*));
	
	// set options to options listed in array given 
	for(int i=0;i<numOpts;i++)
	{
		state->options[i] = malloc((strlen(options[i]) + 1) * sizeof(char));
		strcpy(state->options[i], options[i]);	
	}
	
	state->numOptions = numOpts;		
	
	if(state->option >= numOpts)
		state->option = 0;
	
	// set initial display		
	state->menuX = x*12;
	state->menuY = y*12;
}

// handle menu display and input
void handleMenu(struct gameState * state)
{
	if(state->options != NULL)
	{
		for(int i=0;i<state->numOptions;i++)
		{
			printText(state->options[i], state->menuX, state->menuY+i*FONT_SIZE, state->fontHandler);
		}

		switch(state->input)
		{
			case UP:
			if(state->option > 0)
				state->option--;
			break;
			case DOWN:
			if(state->option < state->numOptions-1)
				state->option++;
			break;
		}				
		printText(">", state->menuX-FONT_SIZE,state->menuY+state->option*FONT_SIZE, state->fontHandler);
	}
		
}

#endif
