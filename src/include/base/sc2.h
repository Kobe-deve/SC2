// general functions used in the game code 
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

// displaying errors 
void throwError(char * errorText)
{		
	HWND consoleWindow = GetConsoleWindow();
	
	// make sure command prompt is shown 
	ShowWindow(consoleWindow, SW_NORMAL);
	
	printf("%s",errorText);
	exit(0);
}

// checks if a file is present 
int filePresent(char * fileName)
{
	FILE *file;
	char * fileReader = malloc(128 * sizeof(char)); 
	file = fopen(fileName,"r");
	
	if(file != NULL)
	{
		fclose(file);
		return 1;
	}
	return 0;
}
