#include <stdio.h>
#include <string.h>
#include "state.h"
#include "music.h"
#include "event_handler.h"
#include "graphics.h"

enum input
{
	UP = 72,
	DOWN = 80,
	LEFT = 75,
	RIGHT = 77,
	ENTER = 13,
	ESC = 27
};

void menuSelection(void *data)
{
	struct gameState * s = (struct gameState *)data;
	if(s->options != NULL)
	{
		for(int i=0;i<s->numOptions;i++)
		{
			setCursor(s->menuX,s->menuY+i);
			printf("%s",s->options[i]);
		}
	
		switch(s->input)
		{
			case UP:
			if(s->option > 0)
			{
				setCursor(s->menuX-1,s->menuY+s->option);
				printf(" ");
			
				s->option--;
			}
			break;
			case DOWN:
			if(s->option < s->numOptions-1)
			{
				setCursor(s->menuX-1,s->menuY+s->option);
				printf(" ");
		
				s->option++;
			}
			break;
			case ENTER:
			for(int i=0;i<s->numOptions;i++)
			{	
				free(s->options[i]);
			}
			free(s->options);
			s->options = NULL;
			break;
		}
		
		if(s->input == UP || s->input == DOWN)
		{
			setCursor(s->menuX-1,s->menuY+s->option);
			printf(">");
		}
	}
}

// initialize menu based on values given 
void initMenu(struct gameState * s, int numOpts, char ** options, int x, int y)
{
	s->options = malloc(numOpts * sizeof(char*));
	
	for(int i=0;i<numOpts;i++)
	{
		s->options[i] = malloc((strlen(options[i]) + 1) * sizeof(char));
		strcpy(s->options[i], options[i]);	
	}
	
	s->numOptions = numOpts;		
	s->menuX = x;
	s->menuY = y;
	
	// set initial display
	setCursor(s->menuX-1,s->menuY+s->option);
	printf(">");
}

void titleScreen(void *data)
{
	printPattern(TITLE,50,10,80,10);
	
	struct gameState * s = (struct gameState *)data;
	
	// initialize options 
	if(s->options == NULL)
	{
		char ** array = malloc(2 * sizeof(char*));
		array[0] = "New Game";
		array[1] = "Continue";
		
		initMenu(s,2,array,20,20);
		
		free(array);
	}
	destroyListener(DISPLAY,s->listeners);
}

int main()
{
	struct gameState state;
	
	// initialize the window 
	init();
	// initialize music handling
	initMusic(&state);
	
	/*
	for(enum sprites i = BACK_ROOM; i <= COMB; i++)
	{
		system("cls");
		if(i >= GARLAND && i <= M17)
			printPattern(i,50,10,20,20);
		if(i >= BACK_ROOM && i <= PUB)
			printPattern(i,50,10,80,10);
		if(i >= SWORD && i <= COMB)
			printPattern(i,50,10,10,10);
		getchar();
	}
	*/
	
	/*
	int x = 0;
	int y = 0;
	
	while(state.input != 27)
	{
		state.input = 0;
		state.input = inputHandler();
		
		if(state.input != 0)
		{
			setCursor(x,y);
			printf("  ");
		}
		
		switch(state.input)
		{
			case UP:
			y--;
			break;
			case DOWN:
			y++;
			break;
			case LEFT:
			x--;
			break;
			case RIGHT:
			x++;
			break;
		}
		
		setCursor(x,y);
		printf("%c",1);
	}
	*/
	
	// initialize listeners 
	initListeners(state.listeners,MAX_EVENTS);
	
	// register events 
	registerEvent(DISPLAY,titleScreen,state.listeners);
	registerEvent(MENU_SELECTION,menuSelection,state.listeners);
	
	struct EventHandler *handlers = state.listeners[DISPLAY];
	
	int i = 0;
	
	while(state.input != 27)
	{
		// input handling
		if ( _kbhit() )
			state.input = getch();
		else
			state.input = 0;
		
		// go through events 
		for(i=0;i<MAX_EVENTS;i++)
		{
			handlers = state.listeners[i];
			if(handlers != NULL)
				handlers->mainFunction(&state);
		}
	}
	
	destroyListeners(state.listeners,MAX_EVENTS);
	
	return 0;
}