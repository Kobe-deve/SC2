// image handling 

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

#ifndef IMAGE_HANDLED
#define IMAGE_HANDLED

struct image
{
	SDL_Texture* texture;
	
	int width;
	int height;
	
	int x;
	int y;
	
	int scale;
	SDL_Rect renderQuad;	
};

// initialize an image 
struct image initImage(char * filename, SDL_Renderer*  renderer)
{
	struct image returnedImage;
	
	// initialize surface 
	SDL_Surface* loadedSurface = IMG_Load(filename);	
	
	if( loadedSurface == NULL )
		throwError("UNABLE TO LOAD IMAGE");
	
	// set up texture 
	returnedImage.texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	
	// set initial variables 
	returnedImage.width = loadedSurface->w;
	returnedImage.height = loadedSurface->h;
	returnedImage.scale = 1;
	returnedImage.renderQuad = (SDL_Rect){ 0, 0, returnedImage.width*returnedImage.scale, returnedImage.height*returnedImage.scale };
	
	// free surface 
	SDL_FreeSurface( loadedSurface );	
	
	return returnedImage;
};

void renderImage(struct image * usedImage, SDL_Renderer*  renderer)
{
	usedImage->renderQuad = (SDL_Rect){ usedImage->x, usedImage->y, usedImage->width*usedImage->scale, usedImage->height*usedImage->scale };
	
	SDL_RenderCopyEx(renderer, usedImage->texture, NULL, &usedImage->renderQuad, 0, NULL, SDL_FLIP_NONE);		
}

void deallocateImage(struct image * usedImage)
{
	SDL_DestroyTexture(usedImage->texture);
	usedImage->texture = NULL;
}

#endif