#include <iostream>
#include <SDL2/SDL.h>

#include "game.hpp"

void shutdown(SDL_Window *window, SDL_Renderer* renderer, bool error);

int main() {
    //init graphics
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Unable to initialize SDL!" << std::endl;
        shutdown(NULL, NULL, true);
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow(game::WINDOW_TITLE, 
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          game::WINDOW_WIDTH,
                                          game::WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if(window == nullptr) {
        std::cerr << "Unable to create window!" << std::endl;
        shutdown(window, NULL, true);
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr) {
        std::cerr << "Unable to create renderer!" << std::endl;
        shutdown(window, renderer, true);
        return -1;
    }
    //init game
    game::init();
    //init timer
    Uint32 millis = 0;
    Uint32 last = SDL_GetTicks();
    //main loop
    bool running = true;
    while(running) {
        //handle events
        SDL_Event e;
        while(SDL_PollEvent(&e))
            //window closed 
            if(e.type == SDL_QUIT)
                running = false;
            //key pressed
            else if(e.type == SDL_KEYDOWN)
                switch(e.key.keysym.sym) {
                    case SDLK_LEFT:  game::changeDirection(game::LEFT);  break;
                    case SDLK_RIGHT: game::changeDirection(game::RIGHT); break;
                    case SDLK_UP:    game::changeDirection(game::UP);    break;
                    case SDLK_DOWN:  game::changeDirection(game::DOWN);  break;
                }
        //check if the game needs to be updated
        if(millis >= game::MILLIS_PER_UPDATE) {
            game::update();
            millis = 0;
        }
        //clear screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);
        //render game
        game::render(renderer);
        SDL_RenderPresent(renderer);
        //update timer
        auto now = SDL_GetTicks();
        millis += now - last;
        last = now;
    }
    //shutdown
    shutdown(window, renderer, false);
}

void shutdown(SDL_Window *window, SDL_Renderer* renderer, bool error) {
    if(renderer != NULL)
       SDL_DestroyRenderer(renderer);
    if(window != NULL)
        SDL_DestroyWindow(window);
    
    if(error) {
        std::cerr << "FINISHED WITH ERROR: " << SDL_GetError() << std::endl;
    } else  {
        std::cout << "FINISHED" << std::endl;
    }

    SDL_Quit();
}
