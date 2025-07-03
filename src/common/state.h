#ifndef _MAHJONG_STATE_HEADER_
#define _MAHJONG_STATE_HEADER_

#include<random>
#include<algorithm>
#include"tile.h"


class state {
public:

    explicit state()=default;

    /**
     * 王牌摆放：   130是宝牌指示牌
     * 122 124 126 128 130 132 134
     * 123 125 127 129 131 133 135
     * 
     * 
     * 竞技麻将：不允许杠第5个
    */
    std::vector<tile> total_tiles_;

    TileType table_wind_;

    void init();

    std::vector<TileType> dora_indicate();

    tile get_current_tile();    // 获取当前摸的牌

    void render();              // 渲染在牌桌上的牌山

private:

    int current_tile_index_;        // 现在摸的是哪张牌
    int current_kan_index_;         // 现在杠的是哪张牌

};



#endif // _MAHJONG_STATE_HEADER_