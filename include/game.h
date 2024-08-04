#ifndef _MAHJONG_GAME_HEADER_
#define _MAHJONG_GAME_HEADER_


/*
游戏主流程
*/


#include<memory>

#include"state.h"

//单人游戏
void single_game_process();

//多人游戏, 还不会网络API
void network_game_process();





void single_game_process(){


    std::shared_ptr<Rule> rule = std::make_shared<Rule>();
    std::shared_ptr<State> state = std::make_shared<State>();




    while(1){
        
    }
}

void network_game_process(){
    
}


#endif      // _MAHJONG_GAME_HEADER_