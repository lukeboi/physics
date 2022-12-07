#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

// Particle texture
SDL_Texture* particleTexture = NULL;

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
            // printf("x: %f, y: %f, v.x: %f, v.y: %f\n", this->x, this->y, v.x, v.y);
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

class Particle {
    public:
        Particle() {
            this->position = Vector2();
            this->velocity = Vector2();
            this->acceleration = Vector2();
        }
        
        Particle(Vector2 position) {
            this->position = position;
            this->velocity = Vector2();
            this->acceleration = Vector2();
        }

        Particle(Vector2 position, Vector2 velocity, Vector2 acceleration) {
            this->position = position;
            this->velocity = velocity;
            this->acceleration = acceleration;
        }

        Vector2 position;
        Vector2 velocity;
        Vector2 acceleration;

        void bounce_off_walls() {
            if (this->position.x < 0) {
                this->position.x = 0;
                this->velocity.x = -this->velocity.x;
            }
            if (this->position.x > SCREEN_WIDTH) {
                this->position.x = SCREEN_WIDTH;
                this->velocity.x = -this->velocity.x;
            }
            if (this->position.y < 0) {
                this->position.y = 0;
                this->velocity.y = -this->velocity.y;
            }
            if (this->position.y > SCREEN_HEIGHT) {
                this->position.y = SCREEN_HEIGHT;
                this->velocity.y = -this->velocity.y;
            }
        }

        void update(float dt) {
            this->bounce_off_walls();
            printf("position: %f, %f\n", this->position.x, this->position.y);
            printf("velocity: %f, %f\n", this->velocity.x, this->velocity.y);
            printf("acceleration: %f, %f\n", this->acceleration.x, this->acceleration.y);
            this->velocity = this->velocity + this->acceleration * dt;
            this->position = this->position + this->velocity;
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
			// Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_SOFTWARE  );
			if(gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
			}

			else {
                particleTexture = IMG_LoadTexture(gRenderer, "particle.png");
                if(particleTexture == NULL) {
                    printf("Unable to load particle texture! SDL Error: %s\n", SDL_GetError());
                    success = false;
                }

				// Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            }
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
    Vector2 field[SCREEN_WIDTH][SCREEN_HEIGHT];
    for(int x = 0; x < SCREEN_WIDTH; x++) {
        for(int y = 0; y < SCREEN_HEIGHT; y++) {
            // float k = 0.8;
            // float l = 50;
            // field[x][y] = Vector2((1 - k) * sin(float(x) / l) + k * sin(float(y) / l), (1 - k) * sin(float(x) / l) - k * cos(float(y) / l)) * 10;
            field[x][y] = Vector2(0, 9.8);
        }
    }

    Particle particles[10];
    for(int i = 0; i < 10; i++) {
        particles[i] = Particle(Vector2(SCREEN_WIDTH / 15 * i + 50, SCREEN_HEIGHT / 2));
    }

    if(!init()) {
		printf( "Failed to initialize!\n" );
	}

    else {
        bool quit = false;
        
        SDL_Event e;
        
        while (!quit) {
            uint32_t frame_start = SDL_GetTicks();

            // Handle events on queue
            while(SDL_PollEvent(&e) != 0) {
                if(e.type == SDL_QUIT) {
                    quit = true;
                }

                // event handling for objects
            }

            // update particles
            for(int i = 0; i < 10; i++) {
                particles[i].bounce_off_walls();
                particles[i].acceleration = field[int(particles[i].position.x)][int(particles[i].position.y)];
                particles[i].update(1.0 / 60);
            }

            // Clear screen
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);

            // Render red filled quad
            // SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
            // SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);		
            // SDL_RenderFillRect(gRenderer, &fillRect);
            
            // Draw vector field
            for (int x = 0; x < SCREEN_WIDTH; x += 15) {
                for (int y = 0; y < SCREEN_HEIGHT; y += 15) {
                    // Draw crosshair at origin of vector field line
                    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
                    SDL_RenderDrawLine(gRenderer, x - 5, y, x + 5, y);
                    SDL_RenderDrawLine(gRenderer, x, y + 5, x, y - 5);
                    
                    // Draw line representing vector field at this point
                    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
                    SDL_RenderDrawLine(gRenderer, x, y, int(x + field[x][y].x), int(y + field[x][y].y));
                }
            }

            // Draw Particles
            for (int i = 0; i < 10; i++) {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
                float x = int(particles[i].position.x);
                float y = int(particles[i].position.y);
                SDL_RenderDrawLine(gRenderer, x - 10, y, x + 10, y);
                SDL_RenderDrawLine(gRenderer, x, y + 10, x, y - 10);
                SDL_Rect rect = { int(x - 15), int(y - 15), 30, 30 };
                // SDL_RenderDrawRect(gRenderer, &rect);
                SDL_RenderCopy(gRenderer, particleTexture, NULL, &rect); 
                // SDL_RenderDrawPoint(gRenderer, ), int(particles[i].position.y));
            }

            // Update screen
            SDL_RenderPresent(gRenderer);

            // Cap frame rate to 60 FPS
            while(SDL_GetTicks() - frame_start < 1000 / 60) {
                SDL_Delay(1);
            }
        }
    }
    
    close();

    // // Destroy window
    // SDL_DestroyWindow( window );
 
    // // Quit SDL subsystems
    // SDL_Quit();
 
    return 0;
}
