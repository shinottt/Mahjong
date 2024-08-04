#ifndef _MAHJONG_ENUMS_HEADER_
#define _MAHJONG_ENUMS_HEADER_


#include"sn_log.h"


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
    _NOTING, _TANKI, _RYANMEN, _KANCHAN, _PENCHAN, _SHANPON
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



static BlockColor TileTypeToBlockColor(TileType type_);
static TileType IndexToTileType(int index_);


static BlockColor TileTypeToBlockColor(TileType type_){
    int c = static_cast<int>(type_)/9;
    int t = static_cast<int>(type_)%9;
    switch(c){
        case 0:
            return BlockColor::_MAN;break;
        case 1:
            return BlockColor::_PIN;break;
        case 2:
            return BlockColor::_SUO;break;
        case 3:
            if(t >= 0 && t <= 3){return BlockColor::_WIND;break;}
            else if(t >= 4 && t <= 6){return BlockColor::_SANGEN;break;}
        default:
            printf("function TileTypeToBlockColor debug: invalid tile type");
            exit(1);
            break;
    }
}


static TileType IndexToTileType(int index_){
    int c = index_/9;
    int t = index_%9;
    switch(c){
        case 0:
            switch(t){
                case 0: return TileType::_1m;break;
                case 1: return TileType::_2m;break;
                case 2: return TileType::_3m;break;
                case 3: return TileType::_4m;break;
                case 4: return TileType::_5m;break;
                case 5: return TileType::_6m;break;
                case 6: return TileType::_7m;break;
                case 7: return TileType::_8m;break;
                case 8: return TileType::_9m;break;
                default:break;
            }
        case 1:
            switch(t){
                case 0: return TileType::_1p;break;
                case 1: return TileType::_2p;break;
                case 2: return TileType::_3p;break;
                case 3: return TileType::_4p;break;
                case 4: return TileType::_5p;break;
                case 5: return TileType::_6p;break;
                case 6: return TileType::_7p;break;
                case 7: return TileType::_8p;break;
                case 8: return TileType::_9p;break;
                default:break;
            }
        case 2:
            switch(t){
                case 0: return TileType::_1s;break;
                case 1: return TileType::_2s;break;
                case 2: return TileType::_3s;break;
                case 3: return TileType::_4s;break;
                case 4: return TileType::_5s;break;
                case 5: return TileType::_6s;break;
                case 6: return TileType::_7s;break;
                case 7: return TileType::_8s;break;
                case 8: return TileType::_9s;break;
                default:break;
            }
        case 3:
            switch(t){
                case 0: return TileType::_1z;break;
                case 1: return TileType::_2z;break;
                case 2: return TileType::_3z;break;
                case 3: return TileType::_4z;break;
                case 4: return TileType::_5z;break;
                case 5: return TileType::_6z;break;
                case 6: return TileType::_7z;break;
                default:break;
            }
        default:
            sn_consolelog_debug("function IndexToTileType debug: invalid index\n");
            exit(1);
            break;
    }
}




#endif      // _MAHJONG_ENUMS_HEADER_