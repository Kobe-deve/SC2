// basic character menu for seeing the status, accessing settings, etc.
#ifndef CHARACTER_MENU_HANDLED
#define CHARACTER_MENU_HANDLED

// initialize data for character menu
void initCharacterMenu(struct gameState * state)
{
	char ** array = malloc(3 * sizeof(char*));
	array[0] = C_SAVE;
	array[1] = C_SET;
	array[2] = C_EXIT;
	
	initMenu(state,3,array,85,24);
	
	free(array);
}

// deallocate data for character menu 
void deallocateCharacterMenu(struct gameState * state)
{
	
}

// logic for character menu
void characterMenuLogic(struct gameState * state)
{
	handleMenu(state);
	
	if(state->input == ENTER)
	{		
		// handle a specific option 
		switch(state->option)
		{
			case 0: // save
			saveData(state);
			break;
			case 1: // settings 
			state->calledSystem= MENU_SCREEN;
			
			state->switchSystem = 1;
			state->switchTo = SETTINGS_SCREEN;
			deallocateMenu(state);
			break;
			case 2: // exit 
			state->switchSystem = 1;
			state->switchTo = DUNGEON_SCREEN;

			deallocateMenu(state);
			break;
		}
	}
}


// display character menu 
void characterMenuDisplay(struct gameState * state)
{
	int i;
	char displayer[400]; // used in sprite mode to display stats 
	
	printText("MENU", 10, 10, state->fontHandler);
	
	// display current quests
	printText("Quests:", 10, 100, state->fontHandler);
	for(i=0;i<state->numQuests;i++)
	{
		// display based on specific quest type 
		switch(state->currentQuests[i].questType)
		{
			case 0:
			sprintf(displayer,QUEST_1,state->keys);	
			break;
			case 1:
			sprintf(displayer,QUEST_2,state->keys);	
			break;
		}
		
		printText(displayer, 10, 100+(i+1)*40, state->fontHandler);
	
	}
	printText(state->stats.name, 10, 440, state->fontHandler);
	
	/*if(state->numImages == 0)
	{
		addImage(state,LOGO_IMAGE);
		state->images[0].x = 20*12;
		state->images[0].y = 10*12;
	}
	else
		renderImage(&state->images[0], state->renderer, NULL);*/
	
}



#endif