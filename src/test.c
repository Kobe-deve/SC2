// general testing program for Stone Crawler 2.0 to ensure proper data usage without leaks 
// gcc test.c -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o test 
#include "include/base/sc2.h"
#include "include/gameloop.h"

int numInputs;
int * inputs = NULL;

#define NUM_TEST_RUNS 100 // number of times the game is run

// test of title screen functionality 
void titleTest()
{
	int i;
	
	struct gameState state;
	
	state.graphicsMode = 0; // set graphics mode 
	
	printf("\nTITLE TEST\n");
		
	for(i=0;i<NUM_TEST_RUNS;i++)
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
	numInputs = 27;
	inputs = malloc(numInputs * sizeof(int));
	inputs[0] = 0;
	inputs[1] = ENTER;
	inputs[2] = DOWN;
	inputs[3] = DOWN;
	inputs[4] = DOWN;
	inputs[5] = DOWN;
	inputs[6] = DOWN;
	inputs[7] = DOWN;
	inputs[8] = ENTER;
	inputs[9] = RIGHT;
	inputs[10] = RIGHT;
	inputs[11] = RIGHT;
	inputs[12] = UP;
	inputs[13] = UP;
	inputs[14] = ENTER;
	inputs[15] = RIGHT;
	inputs[16] = ENTER;
	inputs[17] = RIGHT;
	inputs[18] = ENTER;
	inputs[19] = DOWN;
	inputs[20] = ENTER;
	inputs[21] = DOWN;
	inputs[22] = ENTER;
	inputs[23] = DOWN;
	inputs[24] = ENTER;
	inputs[25] = DOWN;
	inputs[26] = ENTER;
	
	state.graphicsMode = 0; // set graphics mode 
	
	printf("\nDUNGEON TEST\n");
		
	for(i=0;i<NUM_TEST_RUNS;i++)
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
}

void battleTest()
{
		int i,j;
	
	struct gameState state;
	
	// set up inputs used in the test
	numInputs = 27;
	inputs = malloc(numInputs * sizeof(int));
	inputs[0] = 0;
	inputs[1] = ENTER;
	inputs[2] = DOWN;
	inputs[3] = DOWN;
	inputs[4] = DOWN;
	inputs[5] = DOWN;
	inputs[6] = DOWN;
	inputs[7] = DOWN;
	inputs[8] = RIGHT;
	inputs[9] = BATTLE_SCREEN;
	inputs[10] = ENTER;
	inputs[11] = ENTER;
	inputs[12] = ENTER;
	inputs[13] = ENTER;
	inputs[14] = ENTER;
	inputs[15] = ENTER;
	inputs[16] = ENTER;
	inputs[17] = RIGHT;
	inputs[18] = ENTER;
	inputs[19] = ENTER;
	inputs[20] = ENTER;
	inputs[21] = ENTER;
	inputs[22] = ENTER;
	inputs[23] = ENTER;
	inputs[24] = ENTER;
	inputs[25] = ENTER;
	inputs[26] = ENTER;
	
	state.graphicsMode = 0; // set graphics mode 
	
	printf("\nBATTLE TEST\n");
		
	for(i=0;i<NUM_TEST_RUNS;i++)
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
}

void menuTest()
{
		int i,j;
	
	struct gameState state;
	
	// set up inputs used in the test
	numInputs = 27;
	inputs = malloc(numInputs * sizeof(int));
	inputs[0] = 0;
	inputs[1] = ENTER;
	inputs[2] = DOWN;
	inputs[3] = DOWN;
	inputs[4] = DOWN;
	inputs[5] = DOWN;
	inputs[6] = DOWN;
	inputs[7] = DOWN;
	inputs[8] = RIGHT;
	inputs[9] = MENU;
	inputs[10] = ENTER;
	inputs[11] = ENTER;
	inputs[12] = ENTER;
	inputs[13] = MENU;
	inputs[14] = ENTER;
	inputs[15] = ENTER;
	inputs[16] = ENTER;
	inputs[17] = RIGHT;
	inputs[18] = MENU;
	inputs[19] = ENTER;
	inputs[20] = ENTER;
	inputs[21] = ENTER;
	inputs[22] = ENTER;
	inputs[23] = MENU;
	inputs[24] = ENTER;
	inputs[25] = ENTER;
	inputs[26] = ENTER;
	
	state.graphicsMode = 0; // set graphics mode 
	
	printf("\nMENU TEST\n");
		
	for(i=0;i<NUM_TEST_RUNS;i++)
	{
		printf("%d",i+1);
		initializeGame(&state);
		
		for(j=0;j<numInputs;j++)
		{
			state.input = inputs[j];
			
			// test adding quest
			if(j==10)
				addQuest(1,&state);
			
			// game logic handling 
			logicHandler(&state);
			
			// game display handling 
			displayHandler(&state);
		}
		
		deallocateGame(&state);	
	}
}

void loadedGameTest()
{
		int i,j;
	
	struct gameState state;
	
	// set up inputs used in the test
	numInputs = 27;
	inputs = malloc(numInputs * sizeof(int));
	inputs[0] = 0;
	inputs[1] = DOWN;
	inputs[2] = DOWN;
	inputs[3] = ENTER;
	inputs[4] = DOWN;
	inputs[5] = DOWN;
	inputs[6] = DOWN;
	inputs[7] = DOWN;
	inputs[8] = RIGHT;
	inputs[9] = MENU;
	inputs[10] = ENTER;
	inputs[11] = ENTER;
	inputs[12] = ENTER;
	inputs[13] = MENU;
	inputs[14] = ENTER;
	inputs[15] = ENTER;
	inputs[16] = ENTER;
	inputs[17] = RIGHT;
	inputs[18] = MENU;
	inputs[19] = ENTER;
	inputs[20] = ENTER;
	inputs[21] = ENTER;
	inputs[22] = ENTER;
	inputs[23] = MENU;
	inputs[24] = ENTER;
	inputs[25] = ENTER;
	inputs[26] = ENTER;
	
	state.graphicsMode = 0; // set graphics mode 
	
	printf("\nLOADED GAME TEST\n");
		
	for(i=0;i<NUM_TEST_RUNS;i++)
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
}

// main function 
void main()
{
	
	srand((unsigned)time(NULL));
	
	titleTest();
	dungeonTest();
	battleTest();
	menuTest();
	loadedGameTest();
	system("cls");
	printf("\nDONE");
	free(inputs);
}