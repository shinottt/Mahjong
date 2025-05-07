#include"state.h"



void state::init(){
    // 放牌
    for(int i = 0; i < 136; ++i){
        tile t{static_cast<TileType>(i/4)};
        total_tiles_.emplace_back(t);
    }
    // 洗牌
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(total_tiles_.begin(), total_tiles_.end(), g);

    current_tile_index_ = 0;
    current_kan_index_ = 135;
}


std::vector<TileType> state::dora_indicate(){
    // 以后可能会加上杠宝牌
    std::vector<TileType> res;
    res.emplace_back(total_tiles_[130].type_);
    return res;
}


tile state::get_current_tile(){
    return total_tiles_[current_tile_index_++];
}