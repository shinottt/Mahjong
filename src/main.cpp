/**
 * @author shinottt
 * @brief 主函数
*/

#include<iostream>
#include"game.h"
#include"player.h"
#include"state.h"
#include<SDL3/SDL.h>
#include<SDL3/SDL_main.h>

#include"sn_log.hpp"

tile m1{TileType::_1m};
tile m2{TileType::_2m};
tile m3{TileType::_3m};
tile m4{TileType::_4m};
tile m5{TileType::_5m};
tile m6{TileType::_6m};
tile m7{TileType::_7m};
tile m8{TileType::_8m};
tile m9{TileType::_9m};
tile p1{TileType::_1p};
tile p2{TileType::_2p};
tile p3{TileType::_3p};
tile p4{TileType::_4p};
tile p5{TileType::_5p};
tile p6{TileType::_6p};
tile p7{TileType::_7p};
tile p8{TileType::_8p};
tile p9{TileType::_9p};
tile s1{TileType::_1s};
tile s2{TileType::_2s};
tile s3{TileType::_3s};
tile s4{TileType::_4s};
tile s5{TileType::_5s};
tile s6{TileType::_6s};
tile s7{TileType::_7s};
tile s8{TileType::_8s};
tile s9{TileType::_9s};
tile z1{TileType::_1z};
tile z2{TileType::_2z};
tile z3{TileType::_3z};
tile z4{TileType::_4z};
tile z5{TileType::_5z};
tile z6{TileType::_6z};
tile z7{TileType::_7z};

// 测试
void test_yaku_cal();

auto log_t = logger::get_instance();

int main(int argc, char* argv[]){

    log_t->log_init_file("log.txt");
    log_t->log_start();

    SDL_Log("Main function is running...\n");
    
    /*
    SDL_Log("Game is running...\n");
    auto game_ins = game::get_instance();
    game_ins->init("咲慕流年", 1600, 900);
    game_ins->run();
    SDL_Log("Game is over...\n");
    */


    language_manager::get_instance()->init(Language::Chinese);
    /*
    std::unique_ptr<state> test_state = std::make_unique<state>();
    test_state->init();
    */



    SDL_Log("Main function is over...\n");
    return 0;
}

