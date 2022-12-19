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
	
	int input = 0;
	
	int x = 0;
	int y = 0;
	
	while(input != 27)
	{
		input = 0;
		inputHandler(&input);
		
		if(input != 0)
		{
			setCursor(x,y);
			printf("  ");
		}
		
		switch(input)
		{
			case 72:
			y--;
			break;
			case 80:
			y++;
			break;
			case 75:
			x--;
			break;
			case 77:
			x++;
			break;
		}
		
		setCursor(x,y);
		printf("%c",1);
	}
	
	return 0;
}