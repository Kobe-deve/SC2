#ifndef EVENT_HANDLING
#define EVENT_HANDLING
#include <conio.h>
#include <stdlib.h>

// https://www.gamedev.net/forums/topic/646358-event-handlers-in-c/

// input event
void inputHandler(void *data)
{
	if ( _kbhit() )
		*(int *)data = getch();
	
//	if(*(int *)data != 0 && *(int *)data != 224)
//		system("cls");
}


#endif