#include <stdio.h>
#include "state.h"
#include "music.h"
#include "event_handler.h"
#include "graphics.h"

void inputHandler(void *data)
{
	if ( _kbhit() )
		*(int *)data = getch();
	
//	if(*(int *)data != 0 && *(int *)data != 224)
//		printf("\nSOMETHING HAPPENED");
}

void test2(void *data)
{
	printf("\nTEST2");
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
	
	registerEvent(INPUT_HANDLER,test1);
	registerEvent(INPUT_HANDLER,test2);
	
	struct EventHandler *handlers = listeners[INPUT_HANDLER];
	
	int input = 0;
	
	while(input != 27)
	{
		handlers = listeners[INPUT_HANDLER];
		input = 0;
		
		handlers->mainFunction(&input);
	}
	
	destroyListeners(listeners,MAX_EVENTS);
	
	return 0;
}