#include <string.h>

#ifndef FONT_HANDLED
#define FONT_HANDLED

#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED

// main SDL2 handler
#include <SDL2/SDL.h>

// handles pngs with SDL2
#include <SDL2/SDL_image.h>

// handles music with SDL2
#include <SDL2/SDL_mixer.h>

// font handling with SDL2 
#include <SDL2/SDL_ttf.h>

#endif

// font size
#define FONT_SIZE 20

// used for handling font in sprite mode 
struct text
{
	SDL_Renderer * renderer; // the renderer to display to 
	SDL_Texture ** letters; // stored letters for rendering

	SDL_Color textColor; // text color 
	
	int height; // height and width of individual letters 
	int width;
};

// initialize font handling 
void initFont(struct text * t, SDL_Renderer * r)
{
	
	TTF_Font * font = TTF_OpenFont(USED_FONT_FILE,FONT_SIZE);
	
	// set renderer to given renderer		
	t->renderer = r;
	
	// individual letter sizes for text spacing 
	t->width = FONT_SIZE/2+1;
	t->height = FONT_SIZE;
	
	if(font == NULL)
		throwError("Failed to load font");
			
	t->textColor.r = 255;
	t->textColor.g = 255;
	t->textColor.b = 255;
	t->textColor.a = 255;
	
	t->letters = malloc(127 * sizeof(SDL_Texture *));
	int i = 0;
	
	char * letter = malloc(sizeof(char));
	
	
	// load letters 
	for(i=29;i<126;i++)
	{
		letter[0] = (char)i;
		
		// fix error with letter pointer being longer than 1 character 
		if(strlen(letter) > 1)
		{
			while(strlen(letter) > 1)
			{
				free(letter);
				letter = malloc(sizeof(char));
				letter[0] = (char)i;
			}
		}
		
		SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, letter, t->textColor);
		
		if(textSurface == NULL)
			throwError("Unable to render text surface! SDL_ttf");
		else
		{
			//Create texture from surface pixels
			t->letters[i] = SDL_CreateTextureFromSurface(t->renderer, textSurface);
			
			if(t->letters[i] == NULL )
				throwError("Unable to create texture from rendered text!");
			else
			{
				t->width = textSurface->w;
				t->height = textSurface->h;
			}
			
			//Get rid of old surface
			SDL_FreeSurface( textSurface );
		}		
	}
	
	TTF_CloseFont( font );
	
};

// display text to string 
void printText(char * outputString, int x, int y, struct text * t)
{
	int size = 25;
	
	SDL_Rect renderQuad;
	int i;
	
	// char used for getting parts of the t string 
	char chara = ' ';
		
	for(i=0;i<strlen(outputString);i++)
	{
		// render the text, changing the size if it's different from the default 
		renderQuad = (SDL_Rect){x+(i*t->width), y, t->width, t->height};
			
		// set current character to a part of the string 
		chara = outputString[i];
		
		// set text color 
		SDL_SetTextureColorMod(t->letters[(int)chara], t->textColor.r, t->textColor.g, t->textColor.b);
				
		// present alpha and texture piece 
		SDL_SetTextureAlphaMod(t->letters[(int)chara], t->textColor.a);
		SDL_RenderCopyEx(t->renderer, t->letters[(int)chara], NULL, &renderQuad, 0,NULL,SDL_FLIP_NONE);
	
	}	
};

// deallocate font handling 
void deallocateFont(struct text * t)
{
	for(int i = 29;i<126;i++)
		SDL_DestroyTexture(t->letters[i]);
	
	t->renderer = NULL;
};

#endif