#ifndef _MAHJONG_PLAYER_HEADER_
#define _MAHJONG_PLAYER_HEADER_



#include"tile.h"


/*

player类中算分

*/

class Player{
public:
    CharacterId id_;

    
    bool isFuriten_ = false;        //振听
    bool is_riichi_ = false;
    bool is_double_riichi_ = false;
    bool is_tenpai_ = false;        //听牌
    bool is_menzen_ = true;


    int score_ = 25000;

    //手中的牌
    std::vector<Tile> hand_;


    //牌河
    std::vector<Tile> river_;


    //听牌的blocks
    std::vector<std::vector<Block>> ting_;



    void hand_to_block();


};






#endif  // _MAHJONG_PLAYER_HEADER_