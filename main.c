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
	
	return 0;
}