#ifndef __GAME_HPP_
#define __GAME_HPP_

#include <SDL2/SDL.h>
#include <vector>

namespace game {
    //constants
    constexpr char* WINDOW_TITLE = "Snake";

    constexpr int WINDOW_WIDTH  = 800;
    constexpr int WINDOW_HEIGHT = 600;

    constexpr Uint32 MILLIS_PER_UPDATE = 0.3 * 1000;
    
    constexpr int TILES_X = 20;
    constexpr int TILES_Y = 15;

    constexpr int TILE_WIDTH  = WINDOW_WIDTH / TILES_X;
    constexpr int TILE_HEIGHT = WINDOW_HEIGHT / TILES_Y;


    //data types
    struct Tile {
        int x, y;
        
        bool collides(Tile other) {
            if(other.y == y &&
               other.x == x)
                return true;
            else 
                return false;
        }
    };

    enum MoveDirection {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    
    //lifecycle functions
    void init();
    void update();
    void render(SDL_Renderer *renderer);

    //other functions
    void changeDirection(MoveDirection newDir);
};
#endif
