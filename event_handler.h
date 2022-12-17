#ifndef EVENT_HANDLING
#define EVENT_HANDLING

typedef enum Events{
	INPUT_HANDLER,
	DISPLAY,
	TEST3,
	MAX_EVENTS
}gameEvents;

typedef void (*callback) (void *);

struct EventHandler{
	callback mainFunction;
};


// registers event 
void registerEvent(gameEvents eventType, callback callBackFunction)
{
	
}



#endif