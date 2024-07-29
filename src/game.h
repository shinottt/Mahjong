#ifndef _MAHJONG_GAME_HEADER_
#define _MAHJONG_GAME_HEADER_


/*
游戏主流程
*/


#include<memory>

#include"state.h"



void game_main(){


    std::shared_ptr<Rule> rule = std::make_shared<Rule>();
    std::shared_ptr<State> state = std::make_shared<State>();




    while(1){
        
    }
}










#endif      // _MAHJONG_GAME_HEADER_