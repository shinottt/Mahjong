#ifndef _MAHJONG_ENUMS_HEADER_
#define _MAHJONG_ENUMS_HEADER_

enum TileType: int {
    _1m, _2m, _3m, _4m, _5m, _6m, _7m, _8m, _9m,
    _1p, _2p, _3p, _4p, _5p, _6p, _7p, _8p, _9p,
    _1s, _2s, _3s, _4s, _5s, _6s, _7s, _8s, _9s,
    _1z, _2z, _3z, _4z, _5z, _6z, _7z
};

//牌的状态：打出的，自摸的，杠到的
enum TileState: int {
    _DA, _TSUMO, _KAN
};

//不完整，顺，刻，杠，对
enum BlockType: int {
    _INCOMPLETED, _SHUNTSU, _KOUTSU, _KANTSU, _TOITSU
};

enum BlockColor: int {
    _MAN, _PIN, _SUO, _WIND, _SANGEN
};


enum CharacterId: int {
    _mortal, _shino, _hayari
};

//听牌类型：单骑，两面，嵌张，边张，对倒
enum TenType: int {
    _TANKI, _RYANMEN, _KANCHAN, _PENCHAN, _SHANPON
};

//无，满贯，跳满，倍满，三倍满，累计役满，役满，双倍役满
enum ManType: int {
    _NONE, _MANGAN, _HANEMAN, _BAIMAN, _SANBAIMAN, _KAZOYAKUMAN, _YAKUMAN, _DOUBLEYAKUMAN
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


    _DOUBLERIICHI,              //双立直
    _SANSHOKUDOUKOU,            //三色同刻
    _SANKANTSU,                 //三槓子
    _TOITOI,                    //対々和
    _SANANKOU,                  //三暗刻
    _SHOUSANGEN,                //小三元
    _HONROUTOU,                 //混老頭
    _CHITOITSU,                 //七対子    
    _CHANTAIYAO,                //混全帯么九
    _IKKITSUUKAN,               //一気通貫
    _SANSHOKUDOUJUN,            //三色同順

    _RYANPEIKOU,                //二盃口
    _JUNCHANTAIYAO,             //純全帯么九
    _HONIISOU,                  //混一色

    _CHINIISOU,                 //清一色

    _NAGASHIMANGAN,             //流し満貫

    _TENHOU,                    //天和
    _CHIIHOU,                   //地和
    _DAISANGEN,                 //大三元
    _SUUANKOU,                  //四暗刻
    _TSUUIISOU,                 //字一色
    _RYUUIISOU,                 //緑一色
    _CHINROUTOU,                //清老頭
    _KOKUSHIMUSOU,              //国士无双
    _SHOUSUUSHI,                //小四喜
    _SUUKANTSU,                 //四槓子
    _CHUURENPOUTOU,             //九蓮宝燈

    _SUUANKOUTANKI,             //四暗刻単騎
    _KOKUSHIMUSOU13,            //国士无双13面
    _JUNSEICHUURENPOUTOU,       //純正九蓮宝燈
    _DAISUUSHI,                 //大四喜


};


#endif      // _MAHJONG_ENUMS_HEADER_