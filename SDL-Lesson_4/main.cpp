// Setting up SDL Extension Libraries to allow more picture loading types

//Include SDL functions and datatypes
#include "SDL.h"
#include <string>
#include "SDL_image.h"

//The attributes of the screen
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The surfaces that will be used
SDL_Surface *image = NULL;
SDL_Surface *screen = NULL;

//The event structure that will be used
SDL_Event event;

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
    }

	//Return the optimized image
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    //Make a temporary rectangle to hold the offsets
    SDL_Rect offset;
    
    //Give the offsets to the rectangle
    offset.x = x;
    offset.y = y;

	//Blit the surface
    SDL_BlitSurface( source, NULL, destination, &offset );
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
    SDL_WM_SetCaption( "Event test", NULL );
    
    //If everything initialized fine
    return true;
}

// Here is the file loading function. It loads the images, and returns false if there were any problems.
bool load_files()
{
    //Load the image
    image = load_image( "x.png" );
    
    //If there was an error in loading the image
    if( image == NULL )
    {
        return false;    
    }
    
    //If everything loaded fine
    return true;    
}

void clean_up()
{
    //Free the image
    SDL_FreeSurface( image );
    
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

	//Apply the surface to the screen
    apply_surface( 0, 0, image, screen );
    
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