#ifndef _MAHJONG_TILE_HEADER_
#define _MAHJONG_TILE_HEADER_

#include<vector>

#include"enums.h"


class Tile{
public:
    TileType type_;
    bool is_red_;
    TileState tile_state_;    

    bool is_wind();            //是否风牌
    bool is_sangen();          //是否三元牌
    bool is_19();              //是否老头牌：19万，筒，条
    bool is_tsu();              //是否字牌：风牌+三元牌
    bool is_yaojiu();           //是否幺九牌：老头牌+字牌
    bool is_green();            //是否绿牌：判断绿一色

    Tile()=default;
    Tile(const Tile& aim):type_(aim.type_), is_red_(aim.is_red_), tile_state_(aim.tile_state_){}
    Tile(TileType type, bool is_red, TileState tile_state = TileState::_TSUMO)
    :type_(type), is_red_(is_red), tile_state_(tile_state){}

    //不判断红宝
    bool operator==(const Tile& aim) const {
        return type_ == aim.type_;
    }

    bool operator!=(const Tile& aim){
        return !(*this == aim);
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

inline bool Tile::is_wind(){
    if(type_ >= TileType::_1z && type_ <= TileType::_4z){
        return true;
    }
    return false;
}

inline bool Tile::is_sangen(){
    if(type_ >= TileType::_5z && type_ <= TileType::_7z){
        return true;
    }
    return false;
}

inline bool Tile::is_19(){
    if(type_ == TileType::_1m || type_ == TileType::_1p || type_ == TileType::_1s || type_ == TileType::_9m || type_ == TileType::_9p || type_ == TileType::_9s){
        return true;
    }
    return false;
}

inline bool Tile::is_tsu(){
    if(is_wind() || is_sangen()){
        return true;
    }
    return false;
}

inline bool Tile::is_yaojiu(){
    if(is_19() || is_tsu()){
        return true;
    }
    return false;
}

inline bool Tile::is_green(){
    if(type_ == TileType::_6z || type_ == TileType::_2s || type_ == TileType::_3s || type_ == TileType::_4s || type_ == TileType::_6s || type_ == TileType::_8s){
        return true;
    }
    return false;
}




/*
手牌转化为各个block，听牌的block组合中，听牌所在block的type_为_INCOMPLETED
*/
class Block{
public:

    BlockType type_;    //block的类型：不完整，顺，刻，杠，对
    BlockColor color_;   //block的颜色： 万，条，筒，风，三元
    int len_;           //block中牌的数量
    bool is_hand_;      //是否为手中的block
    TenType ten_ = TenType::_NOTING;

    std::vector<Tile> tiles_;       //block中牌的集合

    Block()=default;
    Block(const Block& aim):type_(aim.type_), color_(aim.color_), len_(aim.len_), is_hand_(aim.is_hand_), ten_(aim.ten_), tiles_(aim.tiles_){}
    Block(BlockType type, BlockColor color, int len, bool is_hand, TenType ten = TenType::_NOTING)
    :type_(type), color_(color), len_(len), is_hand_(is_hand), ten_(ten){}


    //完整的block才用下面的判断
    bool is_wind();            //是否风牌block
    bool is_sangen();          //是否三元牌block
    bool is_19();              //是否老头牌block: [123], [789], [111], [999]


    bool operator=(const Block& aim){
        type_ = aim.type_;
        color_ = aim.color_;
        len_ = aim.len_;
        is_hand_ = aim.is_hand_;
        ten_ = aim.ten_;
        tiles_ = aim.tiles_;
        return true;
    }


    bool operator==(const Block& aim) const {
        //合在一起写成一行会报错
        bool a = (type_ == aim.type_);
        bool b = (color_ == aim.color_);
        bool c = (is_hand_ == aim.is_hand_);
        bool d = (tiles_[0].type_ == aim.tiles_[0].type_);
        bool e = (ten_ == aim.ten_);
        return (a && b && c && d);
    }

    bool operator!=(const Block& aim) const {
        return !(*this == aim);
    }

    //按一定规则比大小
    bool operator<(const Block& aim) const {
        if(is_hand_){
            if(!aim.is_hand_){return true;}
            else{
                if(type_ < aim.type_){return true;}
                else if(type_ > aim.type_){return false;}
                else{
                    if(color_ < aim.color_){return true;}
                    else if(color_ > aim.color_){return false;}
                    else{
                        if(tiles_[0].type_ < aim.tiles_[0].type_){return true;}
                        else{return false;}
                    }
                }
            }
        } else{
            if(aim.is_hand_) {return false;}
            else{
                if(type_ < aim.type_){return true;}
                else if(type_ > aim.type_){return false;}
                else{
                    if(color_ < aim.color_){return true;}
                    else if(color_ > aim.color_){return false;}
                    else{
                        if(tiles_[0].type_ < aim.tiles_[0].type_){return true;}
                        else{return false;}
                    }
                }
            }

        }
    }

    bool operator>(const Block& aim) const {
        bool b = (!(*this < aim) && !(*this == aim));
        return b;
    }


};

inline bool Block::is_wind(){
    if(color_ == BlockColor::_WIND){return true;}
    return false;
}

inline bool Block::is_sangen(){
    if(color_ == BlockColor::_SANGEN){return true;}
    return false;
}

inline bool Block::is_19(){
    for(auto& temp_tile_ : tiles_){
        if(temp_tile_.is_19()){return true;}
    }
    return false;
}


#endif  // _MAHJONG_TILE_HEADER_