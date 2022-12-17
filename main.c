#include <stdio.h>
#include "music.h"
#include "event_handler.h"
#include "graphics.h"

int main()
{
	// initialize the window 
	init();
	// initialize music handling
	initMusic();
	printPattern(50,10,80,10);
	getchar();
	
	
	return 0;
}