#ifndef EVENT_HANDLING
#define EVENT_HANDLING
#include <conio.h>
#include <stdlib.h>

// https://www.gamedev.net/forums/topic/646358-event-handlers-in-c/

enum input
{
	UP = 72,
	DOWN = 80,
	LEFT = 75,
	RIGHT = 77,
	ENTER = 13,
	ESC = 27
};

// keyboard input event handling 
void inputHandler(void *data)
{
	if ( _kbhit() )
		*(int *)data = getch();
}

// event handling 
typedef void (*callback) (void *);

typedef enum Events{
	INPUT_HANDLER,
	DISPLAY,
	TITLE_SCREEN,
	MAX_EVENTS
}gameEvents;

struct EventHandler{
	callback mainFunction;
};

// registers event 
void registerEvent(gameEvents eventType, callback callBackFunction, struct EventHandler *listeners[])
{
	struct EventHandler **handlers = &listeners[eventType];
	if (*handlers == NULL)
	{
		*handlers = (struct EventHandler*) malloc(sizeof (struct EventHandler));
		(*handlers)->mainFunction = callBackFunction;
	}
	else
	{
		free(handlers);
	
		*handlers = (struct EventHandler*) malloc(sizeof (struct EventHandler));
		(*handlers)->mainFunction = callBackFunction;
	}
}

// initialize listener
void initListeners(struct EventHandler *handlers[], int size)
{
	int i;
	for(i =0;i<MAX_EVENTS;i++)
		handlers[i] = NULL;
}

// destroy listeners
void destroyListeners(struct EventHandler *handlers[], int size)
{
	int i;
	struct EventHandler *deleteMe, *next;
	
	for(i =0;i<MAX_EVENTS;i++){
		
		deleteMe = handlers[i];
		if(deleteMe != NULL)
			free(deleteMe);
	}
}

#endif