#ifndef _MAHJONG_ENUMS_HEADER_
#define _MAHJONG_ENUMS_HEADER_

enum TileType: int {
    _1m, _2m, _3m, _4m, _5m, _6m, _7m, _8m, _9m,
    _1p, _2p, _3p, _4p, _5p, _6p, _7p, _8p, _9p,
    _1s, _2s, _3s, _4s, _5s, _6s, _7s, _8s, _9s,
    _1z, _2z, _3z, _4z, _5z, _6z, _7z
};

//不完整，顺，刻，杠，对
enum BlockType: int {
    _INCOMPLETE, _SHUNTSU, _KOUTSU, _KANTSU, _TOITSU
};




enum CharacterId: int {
    _mortal, _shino, _hayari
};

enum YakuType: int {
    
    _RIICHI,                    //立直
    _TANYAO,                    //断幺九
    _MENZENTSUMO,               //门前清自摸和
    _YAKUHAIWINDSELF,           //自风
    _YAKUHAIWINDTABLE,          //场风
    _YAKUHAISANGEN,             //三元役牌
    _PINFU,                     //平和
    _IIPEIKOU,                  //一盃口
    _CHANKAN,                   //抢杠
    _RINSHANKAIHOU,             //嶺上開花
    _HAITEIRAOYUE,              //海底摸月
    _HOUTEIRAOYUI,              //河底撈魚
    _IPPATSU,                   //一発
    _DORA,                      //ドラ
    _URADORA,                   //裏ドラ
    _AKADORA,                   //赤ドラ





};



#endif      // _MAHJONG_ENUMS_HEADER_