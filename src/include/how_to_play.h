
#ifndef HOW_TO_PLAY_HANDLED
#define HOW_TO_PLAY_HANDLED

// display function for title screen 
void howtoplayScreenDisplay(struct gameState * state)
{	
	char displayer[400];

	sprintf(displayer,"FPS: %d",SCREEN_FPS);	
	printText(displayer, 10, 288, state->fontHandler);
	
	sprintf(displayer,"Resolution: %d x %d",state->screenW,state->screenH);	
	printText(displayer, 10, 328, state->fontHandler);
}

void howtoplayHandler(struct gameState * state)
{
    if(state->input == ENTER)
	{
        state->switchSystem = 1;
        state->switchTo = state->calledSystem;
        
        deallocateMenu(state);

    }
}

#endif