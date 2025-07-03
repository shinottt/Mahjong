

#include"game.h"
#include<SDL3/SDL.h>
#include<SDL3/SDL_main.h>


int main(int argc, char* argv[]){
    auto game_ptr = game::get_instance();
    game_ptr->init("Mahjong Game", 1280, 720);
    game_ptr->run();

    return 0;
}

