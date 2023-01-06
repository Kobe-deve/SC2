Stone Crawler Rebuild

A rebuild of Stone Crawler from the ground up using 
C and the SDL2 (along with SDL2_Mixer) library

File Organization:

src: - main.c
     -> include
		-battle.h - handles battle system operations 
		-dungeon.h - handles dungeon system operations
		-title.h - handles title screen operations
		-> base
			-event_handler.h - event handling operations
			-filenames.h - holds filenames for assets used
			-graphics.h - display sprite data operations and init
			-input.h - input handling (contains menu operations also)
			-music.h - music handling
			-sprite_data.h - contains graphic arrays
			-state.h - game state operations
			-stats.h - character stat operations


