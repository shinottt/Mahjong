#ifndef _MAHJONG_TILE_HEADER_
#define _MAHJONG_TILE_HEADER_

#include<vector>

#include"enums.h"


class Tile{
public:
    TileType type_;

    bool is_red_;




    //不判断红宝
    bool operator==(const Tile& aim) const {
        return type_ == aim.type_;
    }

    bool operator<(const Tile& aim) const {
        if(type_ < aim.type_){return true;}
        return false;
    }

    bool operator>(const Tile& aim) const {
        if(type_ > aim.type_) {return true;}
        return false;
    }

};


class Block{
public:

    BlockType type_;


    int len_;           //block中牌的数量


    std::vector<Tile> tiles_;       //block中牌的集合


};




#endif  // _MAHJONG_TILE_HEADER_