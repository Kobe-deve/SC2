#include "include/base/sc2.h"
// tool used for creating dungeons in the game 
// gcc dungeon_maker.c -o dungeon_maker 

// input variable
int input = 0;

// position of the cursor 
int cursorX = 0;
int cursorY = 0;

// current block to set 
int blockType = 1;

// save the dungeon 
void saveDungeon()
{
}




void main()
{
	/*
	while(input != 27)
	{
		// handle input 
		if ( _kbhit() )
			input = getch();
		else
			input = 0;
	}
	*/
	int i;
	
	for(i=0;i<255;i++)
	{
		printf("\n%d - %c",i,i);
	}
	
}