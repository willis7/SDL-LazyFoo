// Clip Blitting and Sprite Sheets
// http://lazyfoo.net/SDL_tutorials/lesson06/index.php

//Include SDL functions and datatypes
#include "SDL.h"
#include <string>
#include "SDL_image.h"

//The attributes of the screen
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The surfaces that will be used
SDL_Surface *spriteSheet = NULL;
SDL_Surface *screen = NULL;

//The event structure that will be used
SDL_Event event;

//The portions of the sprite map to be blitted
SDL_Rect clip[ 4 ];


SDL_Surface *load_image( std::string filename ) 
{
    //Temporary storage for the image that's loaded
    SDL_Surface* loadedImage = NULL;
    
    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

	//Load the image
    loadedImage = IMG_Load( filename.c_str() );

	//If nothing went wrong in loading the image
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );
        
        //Free the old image
        SDL_FreeSurface( loadedImage );

		//If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface - remove the magenta
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 200, 0, 200 ) );
        }
    }

	//Return the optimized image
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;
    
    //Get offsets
    offset.x = x;
    offset.y = y;
    
    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

//Here is the initialization function. This function starts up SDL, sets up the window, sets the caption and returns false if there are any errors.
bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;    
    }
    
    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    
    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;    
    }
    
    //Set the window caption
    SDL_WM_SetCaption( "Sprite Sheet Loading", NULL );
    
    //If everything initialized fine
    return true;
}

// Here is the file loading function. It loads the images, and returns false if there were any problems.
bool load_files()
{
    //Load the image
    spriteSheet = load_image( "clay_man.png" );
    
    //If there was an error in loading the image
    if( spriteSheet == NULL )
    {
        return false;    
    }
    
    //If everything loaded fine
    return true;    
}

void clean_up()
{
    //Free the image
    SDL_FreeSurface( spriteSheet );
    
    //Quit SDL
    SDL_Quit();    
}

int main( int argc, char* args[] )
{
    //Make sure the program waits for a quit
    bool quit = false;

	//Initialize
    if( init() == false )
    {
        return 1;    
    }
    
    //Load the files
    if( load_files() == false )
    {
        return 1;    
    }

    //Clip range for the top left
    clip[ 0 ].x = 0;
    clip[ 0 ].y = 0;
    clip[ 0 ].w = 75;
    clip[ 0 ].h = 75;

    //Clip range for the top right
    clip[ 1 ].x = 75;
    clip[ 1 ].y = 0;
    clip[ 1 ].w = 75;
    clip[ 1 ].h = 75;

    //Clip range for the bottom left
    clip[ 2 ].x = 0;
    clip[ 2 ].y = 75;
    clip[ 2 ].w = 75;
    clip[ 2 ].h = 75;

    //Clip range for the bottom right
    clip[ 3 ].x = 75;
    clip[ 3 ].y = 75;
    clip[ 3 ].w = 75;
    clip[ 3 ].h = 75;


    //Fill the screen white
    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

    //Apply the sprites to the screen
    apply_surface( 0, 0, spriteSheet, screen, &clip[ 0 ] );
    apply_surface( 540, 0, spriteSheet, screen, &clip[ 1 ] );
    apply_surface( 0, 380, spriteSheet, screen, &clip[ 2 ] );
    apply_surface( 540, 380, spriteSheet, screen, &clip[ 3 ] );
    
    //Update the screen
    if( SDL_Flip( screen ) == -1 )
    {
        return 1;    
    }

	//While the user hasn't quit
    while( quit == false )
    {

		//While there's an event to handle
        while( SDL_PollEvent( &event ) )
        {
			//If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }    
        }
    }

	 //Free the surface and quit SDL
    clean_up();
        
    return 0;    
}