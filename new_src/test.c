// general testing program for Stone Crawler 2.0 to ensure proper data usage without leaks 
// gcc test.c -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o test 
#include "include/base/sc2.h"
#include "include/gameloop.h"

int numInputs;
int * inputs = NULL;

// test of title screen functionality 
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

// test of dungeon functionality 
void dungeonTest()
{
	int i,j;
	
	struct gameState state;
	
	// set up inputs used in the test
	numInputs = 10;
	inputs = malloc(10 * sizeof(int));
	inputs[0] = 0;
	inputs[1] = ENTER;
	inputs[2] = DOWN;
	inputs[3] = DOWN;
	inputs[4] = DOWN;
	inputs[5] = DOWN;
	inputs[6] = DOWN;
	inputs[7] = 0;
	inputs[8] = 0;
	inputs[9] = 0;
	
	state.graphicsMode = 0; // set graphics mode 
	
	printf("\nDUNGEON TEST\n");
		
	for(i=0;i<100;i++)
	{
		printf("%d",i+1);
		initializeGame(&state);
		
		for(j=0;j<numInputs;j++)
		{
			state.input = inputs[j];
			
			// game logic handling 
			logicHandler(&state);
			
			// game display handling 
			displayHandler(&state);
		}
		
		deallocateGame(&state);	
	}
	
	printf("\nDONE");
}

// main function 
void main()
{
	
	srand((unsigned)time(NULL));
	
	titleTest();
	dungeonTest();
	free(inputs);
}