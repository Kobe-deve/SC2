#ifndef EVENT_HANDLING
#define EVENT_HANDLING
#include <conio.h>
#include <stdlib.h>
#include <windows.h>

// https://www.gamedev.net/forums/topic/646358-event-handlers-in-c/

// event handling 
typedef void (*callback) (void *);

typedef enum Events{
	DISPLAY,
	MENU_SELECTION,
	LOGIC_HANDLER,
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
	else if(callBackFunction == (*handlers)->mainFunction)
		return;
	else
	{		
		free(handlers);
	
		*handlers = (struct EventHandler*) malloc(sizeof (struct EventHandler));
		(*handlers)->mainFunction = callBackFunction;
	}
}

// destroys specific listener
void destroyListener(gameEvents eventType, struct EventHandler *listeners[])
{
	struct EventHandler **handlers = &listeners[eventType];
	
	if(handlers != NULL)
		free(handlers);
	*handlers = NULL;
	
	listeners[eventType] = NULL;
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
	struct EventHandler *deleteMe;
	
	for(i =0;i<MAX_EVENTS;i++){
		
		deleteMe = handlers[i];
		if(deleteMe != NULL)
			free(deleteMe);
		handlers[i] = NULL;
	}
}

// displaying error 
void throwError(char * errorText)
{		
	HWND consoleWindow = GetConsoleWindow();
	
	// make sure command prompt is shown 
	ShowWindow(consoleWindow, SW_NORMAL);
	
	printf("%s",errorText);
	exit(0);
}

#endif