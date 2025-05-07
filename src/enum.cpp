#include"enum.h"

std::string yaku_to_string(YakuType yaku){
    auto lang_ins = language_manager::get_instance();
    int it = static_cast<int>(yaku)+ 1000;
    std::string res = lang_ins->get_str(std::to_string(it));
    return res;
}

BlockColor TileTypeToBlockColor(TileType tile_type){
    int c = static_cast<int>(tile_type)/9;
    int t = static_cast<int>(tile_type)%9;
    BlockColor res;
    switch(c){
        case 0:
            res = BlockColor::_MAN;
            break;
        case 1:
            res = BlockColor::_PIN;
            break;
        case 2:
            res = BlockColor::_SUO;
            break;
        case 3:
            if(t >= 0 && t <= 3){res = BlockColor::_WIND;break;}
            else if(t >=4 && t <= 6){res = BlockColor::_SANGEN;break;}
        default:
            throw std::invalid_argument("TileTypeToBlockColor: Invalid Tile Type");
            break;
    }
    return res;
}

