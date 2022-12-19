#ifndef EVENT_HANDLING
#define EVENT_HANDLING
#include <conio.h>
#include <stdlib.h>

// https://www.gamedev.net/forums/topic/646358-event-handlers-in-c/

typedef enum Events{
	INPUT_HANDLER,
	DISPLAY,
	MAX_EVENTS
}gameEvents;

typedef void (*callback) (void *);

struct EventHandler{
	callback mainFunction;
	struct EventHandler *next;
};

struct EventHandler *listeners[MAX_EVENTS];

// registers event 
void registerEvent(gameEvents eventType, callback callBackFunction)
{
	struct EventHandler **handlers = &listeners[eventType];
	if (*handlers == NULL)
	{
		*handlers = (struct EventHandler*) malloc(sizeof (struct EventHandler));
		(*handlers)->mainFunction = callBackFunction;
		
		(*handlers)->next = NULL;
	}
	else
	{
		while ((*handlers)->next != NULL){
			(*handlers)++;
		}
		(*handlers)->next = (struct EventHandler*) malloc(sizeof (struct EventHandler));
		(*handlers)->next->mainFunction = callBackFunction;
		
		(*handlers)->next->next = NULL;
	}
}

// initialize listener
void initListeners(struct EventHandler *handlers[], int size)
{
	int i;
	for(i =0;i<MAX_EVENTS;i++){
		handlers[i] = NULL;
	}
}

// destroy listeners
void destroyListeners(struct EventHandler *handlers[], int size)
{
	int i;
	struct EventHandler *deleteMe, *next;
	
	for(i =0;i<MAX_EVENTS;i++){
		deleteMe = handlers[i];
		
		while(deleteMe)
		{
			next = deleteMe->next;
			free(deleteMe);
			deleteMe = next;
		}
		
	}
}

// main event loop
void mainLoop()
{
}

#endif