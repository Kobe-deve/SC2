#include "state.h"
#include "music.h"
#include "event_handler.h"
#include "graphics.h"

#ifndef MENU_HANDLED
#define MENU_HANDLED
// input handling 
enum input
{
	UP = 72,
	DOWN = 80,
	LEFT = 75,
	RIGHT = 77,
	ENTER = 13,
	ESC = 27
};

// free menu selection variables when done 
void freeMenuProcess(struct gameState * s)
{
	for(int i=0;i<s->numOptions;i++)
	{	
		free(s->options[i]);
	}
	free(s->options);
	s->options = NULL;
}

// menu selection handling 
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

#endif