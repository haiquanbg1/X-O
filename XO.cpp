/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, math, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include "bot.h"

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

const int Square_Width = 199;
const int Square_Height = 199;
const int TotalSquares = 9;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

bool xx[9], oo[9];

enum Status{
    Default,
    Mousemotion,
    X,
    O,
    Total
};

SDL_Texture* texture[ Total ];
SDL_Texture* End;
SDL_Texture* Win;
SDL_Texture* Lose;
SDL_Texture* Draw;

// bao nhiêu ô
const int squares = 3;

// lượt của ai
bool humanround = true;

//đã đi bn nước
int movated = 0;

//The mouse button
class LButton
{
	public:
		//Initializes internal variables
		LButton();

		//Init buttons
		void InitButton();

		//Sets top left position
		void setPosition( int x, int y );

		//Handles mouse event
		void handleEvent( SDL_Event* e , int i );

		//Shows button sprite
		void render();

	private:
		//Top left position
		SDL_Point mPosition;

		//Currently used global sprite
		Status mCurrentSprite;
};

LButton Button[ TotalSquares ];

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = Default;
}

void LButton::InitButton()
{
    mCurrentSprite = Default;
}

void LButton::setPosition( int x, int y )
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::handleEvent( SDL_Event* e , int i )
{
    if( humanround )
    {
        //If mouse event happened
        if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN )
        {
            //Get mouse position
            int x, y;
            SDL_GetMouseState( &x, &y );

            //Check if mouse is in button
            bool inside = true;

            //Mouse is left of the button
            if( x < mPosition.x )
            {
                inside = false;
            }
            //Mouse is right of the button
            else if( x > mPosition.x + Square_Width )
            {
                inside = false;
            }
            //Mouse above the button
            else if( y < mPosition.y )
            {
                inside = false;
            }
            //Mouse below the button
            else if( y > mPosition.y + Square_Height )
            {
                inside = false;
            }

            //Mouse is outside button
            if( !inside )
            {
                if(mCurrentSprite == Mousemotion)
                    mCurrentSprite = Default;
            }
            //Mouse is inside button
            else
            {
                //Set mouse over sprite
                if( mCurrentSprite == Default || mCurrentSprite == Mousemotion )
                    switch( e->type )
                    {
                        case SDL_MOUSEMOTION:
                        mCurrentSprite = Mousemotion;
                        break;

                        case SDL_MOUSEBUTTONDOWN:
                        {
                            mCurrentSprite = X;
                            humanround = false;
                            xx[ i ] = true;
                            movated++;
                        }
                        break;
                    }
            }
        }
    }
    else
    {
        if( mCurrentSprite == Default || mCurrentSprite == Mousemotion )
        {
            mCurrentSprite = O;
            humanround = true;
            oo[ i ] = true;
        }
    }
}

void LButton::render()
{
    SDL_Rect RenderIMG = { mPosition.x, mPosition.y, Square_Width, Square_Height};
    SDL_RenderCopy( gRenderer, texture[ mCurrentSprite ], NULL, &RenderIMG );
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

    texture[ Default ] = loadTexture( "Default.png" );
    texture[ Mousemotion ] = loadTexture( "Mousemotion.png" );
    texture[ X ] = loadTexture( "X.png" );
    texture[ O ] = loadTexture( "O.png" );
    End = loadTexture( "End.png" );
    Win = loadTexture( "Win.png" );
    Lose = loadTexture( "Lose.png" );
    Draw = loadTexture( "Draw.png" );

    Button[0].setPosition( 0, 0);
    Button[1].setPosition( 0, SCREEN_HEIGHT/3 + 1 );
    Button[2].setPosition( 0, SCREEN_HEIGHT/3*2 + 1 );
    Button[3].setPosition( SCREEN_WIDTH/3 + 1, 0);
    Button[4].setPosition( SCREEN_WIDTH/3*2 + 1, 0);
    Button[5].setPosition( SCREEN_WIDTH/3 + 1, SCREEN_HEIGHT/3 + 1);
    Button[6].setPosition( SCREEN_WIDTH/3 + 1, SCREEN_HEIGHT/3*2 + 1);
    Button[7].setPosition( SCREEN_WIDTH/3*2 + 1, SCREEN_HEIGHT/3 + 1);
    Button[8].setPosition( SCREEN_WIDTH/3*2 + 1, SCREEN_HEIGHT/3*2 + 1);

	return success;
}

void close()
{
    for(int i=0; i<Total; i++)
    {
        SDL_DestroyTexture( texture[i] );
        texture[i] = NULL;
    }
    SDL_DestroyTexture( End );
    End = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

bool rendergameEnd( SDL_Event e )
{
    SDL_RenderCopy( gRenderer, End, NULL, NULL );

    SDL_RenderPresent( gRenderer );

    while (true) {
        while( SDL_PollEvent( &e ))
        if( e.type == SDL_KEYDOWN )
        {
            if(e.key.keysym.sym == SDLK_y)
            {
                return 1;
            }
            else
                if(e.key.keysym.sym == SDLK_n)
            {
                return 0;
            }
        }
        else
        if ( ( e.type == SDL_QUIT) )
            return 0;
        SDL_Delay(100);
    }
}

void Showgame()
{
    SDL_Texture* Currenttexture = NULL;

    if( gameEnd(xx) )
    {
        Currenttexture = Win;
    }
    else
        if( movated == 5 )
        {
            Currenttexture = Draw;
        }
        else
            Currenttexture = Lose;

    SDL_RenderCopy( gRenderer, Currenttexture, NULL, NULL );
    SDL_RenderPresent( gRenderer );
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{

		    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gRenderer );

			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{

			    if( gameEnd(xx) || gameEnd(oo) || movated == 5)
                {
                    Showgame();

                    SDL_Delay(2000);

                    bool ok = rendergameEnd( e );

                    if(ok)
                    {
                        for(int i=0; i<TotalSquares; i++)
                        {
                            xx[i] = false;
                            oo[i] = false;
                        }

                        humanround = true;
                        movated = 0;

                        for(int i=0; i<TotalSquares; i++)
                        {
//                            SDL_DestroyTexture( texture[i] );
                            Button[ i ].InitButton();
                        }
                    }
                    else
                        quit = true;
                }

			    else
				{//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle button events
					for( int i = 0; i < TotalSquares; ++i )
					{
					    int bot = 9;
					    if(!humanround)
                        {
                            controlBot( movated, bot, xx, oo );
                            Button[ bot ].handleEvent( &e, bot );
                        }
                        if(i == bot)
                            continue;
						Button[ i ].handleEvent( &e, i );
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Draw red horizontal line
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );

				for(int i=1; i<squares; i++)
                {
                    SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 3 * i, SCREEN_WIDTH, SCREEN_HEIGHT / 3 * i );
                    SDL_RenderDrawLine( gRenderer, SCREEN_WIDTH / 3 * i, 0, SCREEN_WIDTH/ 3 * i, SCREEN_HEIGHT );
                }

//                SDL_Rect Position;
//                Position.x = SCREEN_WIDTH / 3 + 1 ;
//                Position.y = SCREEN_HEIGHT / 3 + 1 ;
//                Position.w = SCREEN_WIDTH / 3 -1;
//                Position.h = SCREEN_HEIGHT / 3 -1;
//                SDL_RenderCopy( gRenderer, Currenttexture, NULL, &Position);

//                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
//				SDL_RenderClear( gRenderer );

				for(int i=0; i<TotalSquares; i++)
                {
                    Button[i].render();
                }


                //Update screen
				SDL_RenderPresent( gRenderer );
				}
			}
		}
	}


	//Free resources and close SDL
	close();

	return 0;
}
