Stone Crawler Rebuild

A rebuild of Stone Crawler from the ground up using 
C and the SDL2 (along with SDL2_Mixer) library

File Organization:
src: - main.c
     -> include
		-battle.h - handles battle system operations 
		-dungeon.h - handles dungeon system operations
		-title.h - handles title screen operations
		-shop.h - handles shop operations 
		-> base
			-event_handler.h - event handling operations
			-filenames.h - holds filenames for assets used
			-graphics.h - display sprite data operations and init
			-input.h - input handling (contains menu operations also)
			-music.h - music handling
			-sprite_data.h - contains graphic arrays
			-state.h - game state operations
			-stats.h - character stat operations
     -> maps - contains map files read by the game 
     -> music - contains music/sound assets
     -> dlls - contains dlls used for compiling 
	 -> data - save data files 
	 
	 
Compiling command:
gcc main.c -lSDL2 -lSDL2_mixer -o main

Controls:
	Arrow Keys - Movement/Selecting
	Enter - Confirm
	Esc - Exit the game
	Backspace - exit conversation with npc
	M - (in dungeon) menu 

Event Handling:
	Events are registered through the registerEvent function specifically with 
	a certain enumerated value to represent said function, it's important to
	deallocate this with destroyListener when said function/event does not need
	to be utilized.
	
	For example menu handling in this game is done with MENU_SELECTION
	but it first must be initialized with values before being used