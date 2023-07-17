// general testing program for Stone Crawler 2.0 to ensure proper data usage without leaks 
// gcc test.c -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o test 
#include "include/base/sc2.h"
#include "include/gameloop.h"

void titleTest()
{
	int i;
	
	struct gameState state;
	
	state.graphicsMode = 0; // set graphics mode 
	
	printf("\nTITLE TEST\n");
		
	for(i=0;i<100;i++)
	{
		printf("%d",i+1);
		initializeGame(&state);
	
		// game logic handling 
		logicHandler(&state);
		
		// game display handling 
		displayHandler(&state);
		
		deallocateGame(&state);	
	}
	
	printf("\nDONE");
}

// main function 
void main()
{
	
	srand((unsigned)time(NULL));
	
	titleTest();

}