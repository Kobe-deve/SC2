Stone Crawler Rebuild

A rebuild of Stone Crawler from the ground up using 
C and the SDL2 (along with SDL2_Mixer) library

Compiling command:
run setup_sc2.bat with a C++ compiler installed with the following libraries:
	SDL2
	SDL2_image
	SDL2_ttf
	SDL2_mixer

File Organization:
src: - main.c
     -> include - header files used
		-battle.h - handles battle system operations 
		-dungeon.h - handles dungeon system operations
		-title.h - handles title screen operations
		-shop.h - handles shop operations 
		-menu.h - handles menu operations (accessed while in dungeon/overworld)
		-> base
			-event_handler.h - event handling operations
			-filenames.h - holds filenames for assets used
			-graphics.h - display sprite data operations and init
			-input.h - input handling (contains menu operations also)
			-music.h - music handling
			-sprite_data.h - contains graphic arrays
			-state.h - game state operations
			-stats.h - character stat operations
			-npc.h - npc operations in dungeon crawling 
			-text.h - all text/dialogue in the game is stored here
			-font.h - handling font in window/sprite mode
			-image.h - handling images in window/sprite mode 
     -> data - folder for holding save data
     -> maps - contains map files read by the game 
     -> resources - contains asset data 
     	 -> font - contains font assets
	 -> music - contains music/sound assets
	 -> sprites - contains sprite data
		-> background - contains assets used for the background
		-> curses - enemy sprites during combat
     -> dlls - contains dlls used for compiling 
	 -> data - save data files 
	 
	 

Graphic Modes:
	With Stone Crawler Rebuild there are two graphical modes: ASCII and sprite 
	
	To toggle a specific mode with a build, a global graphicsMode is set to a 
	specific value with these being the specific modes
	0 - ascii
	1 - sprites

Debug Mode
	Debug mode is a toggled variable in state.h
	
	0 - off
	1 - on

Controls:
	Arrow Keys - Movement/Selecting
	Enter - Confirm
	Esc - Exit the game
	Backspace - exit conversation with npc
	M - (in dungeon) menu 
	H - (in debug mode for sprite version) open command prompt

Event Handling:
	Events are registered through the registerEvent function specifically with 
	a certain enumerated value to represent said function, it's important to
	deallocate this with destroyListener when said function/event does not need
	to be utilized.
	
	For example menu handling in this game is done with MENU_SELECTION
	but it first must be initialized with values before being used