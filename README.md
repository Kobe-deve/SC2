# SC2

An RPG developed using C with the SDL2 library

Visual modes:

  ASCII - Uses basic terminal windows.h graphics
  Sprite - Uses SDL2 rendering library

Compiling command:

run setup_sc2.bat with a C/C++ compiler installed with the following libraries:
	
	SDL2
	SDL2_image
	SDL2_ttf
	SDL2_mixer

	windres main.rs -o main.o 
	gcc main.c main.o -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o "Stone Crawler 2"

File Organization:

	src:  
  	 test.c - test program used for implementation  
	 main.c - main program 
	 setup_sc2 - batch file for compiling 
     -> include - header files used
		-battle.h - general battle system operations 
		-character_menu.h - general status menu operations 
		-cutscene.h - general cutscene operations 
		
		-dungeon.h - general dungeon operations 
		-dungeon_logic.h - specific dungeon logic 
		-dungeon_npc.h - npc dungeon logic 
		-dungeon_display.h - display dungeon logic 
		-dungeon_enemies.h - enemy dungeon logic 
		
		-gameloop.h - contains the main game loop 
		-gameover.h - general gameover screen operations
		
		-shops.h - handles shop operations 
		-title_screen.h - handles title screen operations
		
		-> base
			-> graphics
				-font.h - contains all font/text handling from sprite mode 
				-image.h - contains image handling for sprite mode 
				-music.h - contains all music handling 
				-sprite_data.h - contains sprite data for ascii mode from the first Stone Crawler
			
			-> information
				-filenames.h - contains all the names of files used in the game 
				-npc.h - specifies npc data structure
				-stats.h - specifies stat data structure 
				-text.h - contains all text used in the game 
				
			-sc2.h - general functions used in the game code 
			-menu.h - generic menu operations 
			-state.h - contains the game state and operations for initializing/loading/saving/deallocating the data 

     -> maps - contains map files read by the game 
		-> npc_data - contains npc data files to be read for the dungeon 
		
     -> resources - contains asset data 
     	-> font - contains font assets
		-> music - contains music/sound assets
		-> sprites - contains image data for sprite mode
			-> background - contains assets used for the background
			-> curses - enemy sprites during combat

	 -> data - save data files 
	 
	 
Graphic Modes:
	To toggle a specific mode with a build, the state variable graphicsMode is set to a 
	specific value with these being the specific modes (set in main.c)
	0 - ascii
	1 - sprites

Game Systems:

	Each state of the general game application (such as being in a battle, dungeon, title screen)
	involves having a specific display and logic function. Generally the only specification
	for each system is that it should have the below set up for display functions so as
	to fit with the constraints of the Ascii/sprite mode.
	
	Code Example:
	
	void display(void)
	{	
		switch(state->graphicsMode)
		{
			case 0: // ascii mode 
			if(state->megaAlpha == 0) // update screen to add title screen 
			{
				state->megaAlpha = 1;
			}
			break;
			case 1: // sprite mode
			if(state->numImages == 0)
			{
				// initialize images used 
			}
			else
			{
				// display operations 
			}
			
			break;
		}
	}