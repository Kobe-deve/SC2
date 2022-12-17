#include <stdio.h>
#include "state.h"
#include "music.h"
#include "event_handler.h"
#include "graphics.h"

int main()
{
	struct gameState state;
	
	// initialize the window 
	init();
	// initialize music handling
	initMusic(&state);
	
	
	printPattern(50,10,80,10);
	getchar();
	
	
	return 0;
}