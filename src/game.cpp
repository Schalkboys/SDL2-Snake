#include "game.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

namespace game {
    namespace tiles { 
        Tile head;
        Tile food;

        std::vector<Tile> body;
    };

    MoveDirection dir = LEFT;

    bool gameOver = false;
};

void spawnFood();
bool collidesWithSnakeBody(int x, int y);

//lifecycle functions
void game::init() {
    //set random seed
    srand(time(0));

    game::tiles::head = {rand() % game::TILES_X, rand() % game::TILES_Y};

    spawnFood();
}

//executed in the interval of MILLIS_PER_UPDATE
void game::update() {
    if(gameOver)
        return;
    
    //necessary for moving the snakes body later on
    Tile oldHead = game::tiles::head;
    
    //move the snakes head
    switch(dir) {
        case LEFT:  game::tiles::head.x --; break;
        case RIGHT: game::tiles::head.x ++; break;
        case UP:    game::tiles::head.y --; break;
        case DOWN:  game::tiles::head.y ++; break;
    }

    //keep the snakes head inside the play area
    if(game::tiles::head.x < 0)
        game::tiles::head.x = game::TILES_X - 1;
    else if(game::tiles::head.x > game::TILES_X - 1)
        game::tiles::head.x = 0;
    
    if(game::tiles::head.y < 0)
        game::tiles::head.y = game::TILES_Y - 1;
    else if(game::tiles::head.y > game::TILES_Y - 1)
        game::tiles::head.y = 0;

    //check if the food was eaten
    if(game::tiles::head.collides(game::tiles::food)) {
        //add body section
        game::tiles::body.push_back(game::Tile {0, 0});
        //spawn new food item
        spawnFood();
    }
    
    if(game::tiles::body.size() == 0)
        return;
    
    //reposition body tiles
    for(int i = game::tiles::body.size(); i > 0; i--) {
        game::tiles::body[i] = game::tiles::body[i - 1];
    }

    game::tiles::body[0] = oldHead;

    //check if the snake ran into itself
    for(auto &bodyTile : game::tiles::body) {
        if(bodyTile.collides(game::tiles::head)) {
                std::cout << "#######################" << std::endl;
                std::cout << "#=====================#" << std::endl;
                std::cout << "#======GAME OVER======#" << std::endl;
                std::cout << "#=====================#" << std::endl;
                std::cout << "#######################" << std::endl;
                
                gameOver = true;
        }
    }
}

//executed once per frame
void game::render(SDL_Renderer *renderer) {
    //render the snakes head
    SDL_SetRenderDrawColor(renderer, 0x00, 0xAA, 0x44, 0xFF);
    SDL_Rect rectHead = {game::tiles::head.x * game::TILE_WIDTH, game::tiles::head.y * game::TILE_HEIGHT, game::TILE_WIDTH, game::TILE_HEIGHT};
    SDL_RenderFillRect(renderer, &rectHead);
    //render body
    for(auto &bodyPiece : game::tiles::body) {
        SDL_Rect rectBody = {bodyPiece.x * game::TILE_WIDTH, bodyPiece.y * game::TILE_HEIGHT, game::TILE_WIDTH, game::TILE_HEIGHT};
        SDL_RenderFillRect(renderer, &rectBody);
    }
    //render food
    SDL_SetRenderDrawColor(renderer, 0xAA, 0x44, 0x44, 0xFF);
    SDL_Rect rectFood = {game::tiles::food.x * game::TILE_WIDTH, game::tiles::food.y * game::TILE_HEIGHT, game::TILE_WIDTH, game::TILE_HEIGHT};
    SDL_RenderFillRect(renderer, &rectFood);
}

//utility functions
void spawnFood() {
    int x, y;

    do {
        x = rand() % game::TILES_X;
        y = rand() % game::TILES_Y;
    } while(x == game::tiles::head.x 
            && y == game::tiles::head.y 
            //check if the food spawns on the snakes body
            || collidesWithSnakeBody(x, y));

    game::tiles::food = {x, y};
}

void game::changeDirection(game::MoveDirection newDir) {
    //dont turn around 180 degrees at a time
    if((dir == UP && newDir == DOWN)    ||
       (dir == DOWN && newDir == UP)    ||
       (dir == LEFT && newDir == RIGHT) ||
       (dir == RIGHT && newDir == LEFT))
        return;

    dir = newDir;
}

bool collidesWithSnakeBody(int x, int y) {
    for(auto &bodyPiece : game::tiles::body)
        if(bodyPiece.x == x &&
           bodyPiece.y == y)
            return true;
            
    return false;
}

