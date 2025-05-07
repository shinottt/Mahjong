#include"tile.h"

void tile::set_red(bool is_red){
    is_red_ = is_red;
}

bool tile::is_green() const{
    if(type_ == TileType::_6z || type_ == TileType::_2s || type_ == TileType::_3s || type_ == TileType::_4s || type_ == TileType::_6s || type_ == TileType::_8s){
        return true;
    }
    return false;
}

bool tile::is_sangen() const{
    if(type_ >= TileType::_5z && type_ <= TileType::_7z){
        return true;
    }
    return false;
}

bool tile::is_wind() const{
    if(type_ >= TileType::_1z && type_ <= TileType::_4z){
        return true;
    }
    return false;
}

bool tile::is_tsu() const{
    if(is_wind() || is_sangen()){
        return true;
    }
    return false;
}

bool tile::is_19() const{
    if(type_ == TileType::_1m || type_ == TileType::_1p || type_ == TileType::_1s || type_ == TileType::_9m || type_ == TileType::_9p || type_ == TileType::_9s){
        return true;
    }
    return false;
}

bool tile::is_yaojiu() const{
    if(is_19() || is_tsu()){
        return true;
    }
    return false;
}

bool operator==(const tile& t1, const tile& t2){
    return t1.type_ == t2.type_;
}

bool operator!=(const tile& t1, const tile& t2){
    return t1.type_ != t2.type_;
}

bool operator<(const tile& t1, const tile& t2){
    return t1.type_ < t2.type_;
}

bool operator>(const tile& t1, const tile& t2){
    return t1.type_ > t2.type_;
}

bool block::is_wind() const{
    return color_ == BlockColor::_WIND;
}

bool block::is_sangen() const{
    return color_ == BlockColor::_SANGEN;
}

// 判断块是否为老头牌块，例：[111],[789]
bool block::is_19() const{
    bool res = false;
    int j = static_cast<int>(tiles_[0].type_)%9;
    int t = static_cast<int>(tiles_[0].type_)/9;
    switch(type_){
        case BlockType::_SHUNTSU:
            if(j == 0 || j == 6){res = true;}
            break;
        case BlockType::_TOITSU:
        case BlockType::_KOUTSU:
        case BlockType::_KANTSU:
            if(j == 0 || j == 8 || t == 3){res = true;}
            break;
        default:
            break;
    }
    return res;
}

bool block::is_tsu() const{
    return is_wind() || is_sangen();
}

bool block::is_yaojiu() const{
    return is_19() || is_tsu();
}