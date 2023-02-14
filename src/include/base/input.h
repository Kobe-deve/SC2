
#ifndef STATE_HANDLED
#include "state.h"
#endif 

#ifndef MUSIC_HANDLED
#include "music.h"
#endif

#ifndef EVENT_HANDLING
#include "event_handler.h"
#endif

#ifndef GRAPHICS
#include "graphics.h"
#endif

#ifndef MENU_HANDLED
#define MENU_HANDLED

// input delay for sprite version to keep it consistent with ASCII
#define INPUT_DELAY 1

// input handling enumerations 
enum input
{
	UP = 72,
	DOWN = 80,
	LEFT = 75,
	RIGHT = 77,
	ENTER = 13,
	ESC = 27,
	MENU = 109, 
	BACKSPACE = 8
};

// free menu selection variables when done 
void freeMenuProcess(struct gameState * s)
{
	if(s->options != NULL)
	{
		for(int i=0;i<s->numOptions;i++)
		{	
			free(s->options[i]);
		}	
		free(s->options);
		s->options = NULL;
	
		destroyListener(MENU_SELECTION,s->listeners);
	}
}

// clear the menu from the display 
void clearMenu(struct gameState * s) 
{
	int i;
	int j;
	
	setColor(BLACK);
	for(int i=0;i<s->numOptions;i++)
	{
		setCursor(s->menuX,s->menuY+i);
		
		for(j=0;j<strlen(s->options[i]);j++)
		{
			setCursor(s->menuX+j,s->menuY+i);
			printf("%c",219);
		}
	}
	setColor(WHITE);
}

// menu selection handling 
void menuSelection(void *data)
{
	struct gameState * s = (struct gameState *)data;
	if(s->options != NULL)
	{
		for(int i=0;i<s->numOptions;i++)
		{
			switch(graphicsMode)
			{
				case 0:
				setCursor(s->menuX,s->menuY+i);
				printf("%s",s->options[i]);
				break;
				case 1:
				printText(s->options[i], s->menuX, s->menuY+i*FONT_SIZE, s->fontHandler);
				break;
			}
		}
	
		if(graphicsMode == 0)
		{
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
		else
		{
			switch(s->input)
			{
				case UP:
				if(s->option > 0)
					s->option--;
				break;
				case DOWN:
				if(s->option < s->numOptions-1)
					s->option++;
				break;
			}				
			printText(">", s->menuX-FONT_SIZE,s->menuY+s->option*FONT_SIZE, s->fontHandler);
		}		
	}
}

// initialize menu based on values given 
void initMenu(struct gameState * s, int numOpts, char ** options, int x, int y)
{
	s->options = malloc(numOpts * sizeof(char*));
	
	// set options to options listed in array given 
	for(int i=0;i<numOpts;i++)
	{
		s->options[i] = malloc((strlen(options[i]) + 1) * sizeof(char));
		strcpy(s->options[i], options[i]);	
	}
	
	s->numOptions = numOpts;		
	
	if(s->option >= numOpts)
		s->option = 0;
	
	// set initial display
	switch(graphicsMode)
	{
		case 0:
		s->menuX = x;
		s->menuY = y;
	
		setCursor(s->menuX-1,s->menuY+s->option);
		printf(">");
		break;
		case 1:
		s->menuX = x*12;
		s->menuY = y*12;
	
		break;
	}
	
}

#endif