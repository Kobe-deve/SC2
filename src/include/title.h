#ifndef TITLE_HANDLED
#define TITLE_HANDLED

#include "base/state.h"
#include "base/music.h"
#include "base/event_handler.h"
#include "base/input.h"
#include "base/graphics.h"

#ifndef DUNGEON_HANDLED
#include "dungeon.h"
#endif

#ifndef TEXT_DEFINED
#include "base/text.h"
#endif 

#ifndef IMAGE_HANDLED
#include "base/image.h"
#endif

// title screen functions 

// display function, called once at the start 
void titleScreenDisplay(void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	// initialize options 
	if(s->options == NULL)
	{
		// initialize menu 
		char ** array = malloc(2 * sizeof(char*));
		array[0] = NG;
		array[1] = CT;
		
		initMenu(s,2,array,85,24);
		
		free(array);
	
		// display 
		switch(graphicsMode)
		{
			case 0:
			// display title 
			printPattern(TITLE,45,10,80,10);
		
			// descriptions 
			setCursor(55,20);
			printf(TITLE_DESC1);
			setCursor(55,21);
			printf(TITLE_DESC2);
	
			destroyListener(DISPLAY,s->listeners);
			break;
			case 1:
		
			// load title image 
			s->images = malloc(sizeof(struct image));
			s->numImages = 1;
			
			addImage(s,LOGO_IMAGE);
			s->images[0].x = 20*12;
			s->images[0].y = 10*12;
			break;
		}
	}
	else if(graphicsMode == 1)
	{
		printText(TITLE_DESC1, 250, 350, s->fontHandler);
		printText(TITLE_DESC2, 250, 375, s->fontHandler);
		renderImage(&s->images[0], s->renderer);
	}
}

// logic at the title screen 
void titleScreenLogic(void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	if(s->input == ENTER) // start dungeon 
	{
		switch(s->option)
		{
			case 0:
			// clear images if used 
			if(graphicsMode == 1)
			{
				destroyListener(DISPLAY,s->listeners);
				clearImages(s);
			}
			
			freeMenuProcess(s);
			
			initNewGame(s);
			break;
			case 1:
			
			
			break;
		}
	}
}

#endif