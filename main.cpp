#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
 
// Screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

// Vector2 class
class Vector2 {
    public:
        Vector2() {
            x = 0;
            y = 0;
        }
        Vector2(float x, float y) {
            this->x = x;
            this->y = y;
        }

        float x;
        float y;

        Vector2 operator+(const Vector2& v) {
            return Vector2(this->x + v.x, this->y + v.y);
        }
        Vector2 operator-(const Vector2& v) {
            return Vector2(this->x - v.x, this->y - v.y);
        }
        Vector2 operator*(const float& f) {
            return Vector2(this->x * f, this->y * f);
        }
        Vector2 operator/(const float& f) {
            return Vector2(this->x / f, this->y / f);
        }
};

bool init() {
    bool success = true;

    // Initialize SDL
    if(SDL_Init( SDL_INIT_VIDEO ) < 0) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }

    else {
        // Create window
        gWindow = SDL_CreateWindow( "SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if(gWindow == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }

        else {
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if(gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
			}

			else {
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            }

            // Get window surface
            // screenSurface = SDL_GetWindowSurface(gWindow);
 
            // Fill the surface white
            // SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
           
            // Update the surface
            // SDL_UpdateWindowSurface(gWindow);
        }
    }

    return success;
}

void close() {
    // Destroy window
    SDL_DestroyWindow(gWindow);
 
    // Quit SDL subsystems
    SDL_Quit();
}

int main( int argc, char* args[] ) {
    // The surface contained by the window
    SDL_Surface* screenSurface = NULL;

    // Create a vector field describing forces, initalized with gravity
    Vector2 field[SCREEN_WIDTH][SCREEN_HEIGHT] = { Vector2(0, -9.8) };

    if(!init()) {
		printf( "Failed to initialize!\n" );
	}

    else {
        bool quit = false;
        
        SDL_Event e;
        
        while (!quit) {

            // Handle events on queue
            while(SDL_PollEvent(&e) != 0) {
                if(e.type == SDL_QUIT) {
                    quit = true;
                }

                // event handling for objects
            }

            // Clear screen
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);

            // Render red filled quad
            SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);		
            SDL_RenderFillRect(gRenderer, &fillRect);
            
            // Draw vector field
            for (int x = 0; x < SCREEN_WIDTH; x += 15) {
                for (int y = 0; y < SCREEN_HEIGHT; y += 15) {
                    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
                    // SDL_RenderDrawLine(gRenderer, x, y, x + field[x][y].x, y + field[x][y].y);
                    SDL_RenderDrawLine(gRenderer, int(x), int(y), int(x+ 5), int(y + 5));
                }
            }
            // SDL_RenderDrawLine(gRenderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

            // Update screen
            SDL_RenderPresent(gRenderer);
        }
    }
    
    close();

    // // Destroy window
    // SDL_DestroyWindow( window );
 
    // // Quit SDL subsystems
    // SDL_Quit();
 
    return 0;
}
