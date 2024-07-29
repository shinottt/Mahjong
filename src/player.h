#ifndef _MAHJONG_PLAYER_HEADER_
#define _MAHJONG_PLAYER_HEADER_


#include<memory>

#include"state.h"
#include"tile.h"


/*

player类中算分

*/

class Hu{
public:
    TileType type_;         //胡的牌
    TenType ten_;           //听牌类型，
    std::vector<YakuType> yaku_;   //胡牌役种
    std::vector<Block> blocks_;   //听牌的blocks
    ManType man_;           //类型
    int fu_;
    int fan_;
    int basic_point_;            //基本点数： 符×2^(番+2)

    Hu()=default;
    Hu(TileType& type, TenType& ten, std::vector<YakuType>& yaku, std::vector<Block>& blocks, ManType& man, int& fu, int& fan, int& basic_point)
    :type_(type), ten_(ten),yaku_(yaku), blocks_(blocks), man_(man), fu_(fu), fan_(fan), basic_point_(basic_point){}

    void set(TileType type, TenType ten, std::vector<YakuType> yaku, std::vector<Block> blocks, ManType man, int fu, int fan, int basic_point){
        type_ = type;
        ten_ = ten;
        yaku_ = yaku;
        blocks_ = blocks;
        man_ = man;
        fu_ = fu;
        fan_ = fan;
        basic_point_ = basic_point;
    }

};



class Player{
public:
    CharacterId id_;

    
    bool isFuriten_ = false;        //振听
    bool is_riichi_ = false;
    bool is_double_riichi_ = false;
    bool is_tenpai_ = false;        //听牌
    bool is_menzen_ = true;

    int score_ = 25000;

    std::shared_ptr<State> state_;      //获取场况，用于算胡

    Tile next_tile_;    //下一张牌：别人打出的，自摸的

    //手中的牌
    std::vector<Tile> hand_;
    //吃碰杠后放入的牌
    std::vector<Tile> fixed_tiles_;

    //吃，碰，杠的blocks
    std::vector<Block> fixed_blocks_;
    //牌河
    std::vector<Tile> river_;

    //听牌时取到的blocks组合
    std::vector<std::vector<Block>> ten_;
    //胡牌的结果
    std::vector<Hu> hu_;
    //没听牌时用于判定向听数的blocks
    std::vector<std::vector<Block>> shanten_;




    Player()=default;
    Player(CharacterId id):id_(id){}



    void hand_to_block();



    int cal_yaku(Hu&);    //使用 Player(class) 和 Hu(class) 中的数据计算番数和役：一般胡牌




    void take_111(Block& block);        //从手牌中取[111]顺子放入block中
    void take_3(Block& block);          //取[3]刻子
    void take_2(Block& block);          //取[2]对子

    void take_1(Block& block);          //取[1]单张牌
    void take_11(Block& block);         //取[11]挨着的顺子2张
    void take_1_1(Block& block);        //取[1 1]不挨着的顺子2张


    //返回番数
    //1番
    int is_riichi();    //双立直(2)和立直(1)             
    int is_tanyao();    //断幺
    int is_menzentsumo();    //门前清自摸，与暗杠岭上开花不复合
    int is_yakuhai_selfwind();  //自风
    int is_yakuhai_tablewind(); //场风
    int is_yakuhai_sangen();    //三元牌
    int is_pinfu(); //平胡
    int is_iipeikou();      //一盃口(1), 二盃口(3)
    int is_chankan();   //抢杠
    int is_rinshankaihou(); //岭上开花         
    int is_haiteiraoyue();  //海底捞月
    int is_houteiraoyui();  //河底捞鱼
    int is_ippatsu();       //一发
    int is_dora();          //宝牌
    int is_uradora();       //里宝
    int is_akadora();       //红宝
    //2番
    int is_sanshokudoukou();   //三色同刻，排除四暗刻后再判断
    int is_sankantsu();           //三杠子，排除四杠子后再判断
    int is_toitoi();            //对对胡
    int is_sanankou();         //三暗刻(2), 四暗刻(13), 四暗刻单骑(26)
    int is_shousangen();       //小三元(2), 大三元(13)
    int is_honroutou();        //混老头
    int is_chitoitsu();        //七对子，特殊牌型，取block前判断

    int is_chantaiyao();       //混全副露(1), 混全门清(2), 和纯全分开
   
    int is_ikkitsuukan();      //一气贯通, 副露(1), 门清(2)
    int is_sanshokudoujun();   //三色同顺, 副露(1), 门清(2)
    //3番
    int is_honiisou();         //混副露(2)，混门清(3)，纯副露(5)，纯门清(6)，字一色(14)，九莲宝灯(13)，纯正九莲宝灯(26)
    int is_junchantaiyao();     //纯全副露(2), 纯全门清(3)

    //役满
    int is_tenhou();    //天胡
    int is_chiihou();   //地胡
    int is_ryuuiisou(); //绿一色
    int is_chinroutou();    //清老头
    int is_kokushimusou();  //国士无双(13)，国士无双13面(26), 特殊牌型要先判断
    int is_shousuushi();    //小四喜
    int is_suukantsu();     //四杠子     

};




int Player::cal_yaku(Hu& cal_hu_){
    int fan = 0;
    int f = 0;

    f = is_riichi();
    if(f != 0){
        fan += f;
        if(f == 1){cal_hu_.yaku_.push_back(YakuType::_RIICHI);}
        if(f == 2){cal_hu_.yaku_.push_back(YakuType::_DOUBLERIICHI);}
    }
    f = is_tanyao();
    if(f != 0){
        fan += f;
        cal_hu_.yaku_.push_back(YakuType::_TANYAO);
    }
    f = is_menzentsumo();
    if(f != 0){
        fan += f;
        cal_hu_.yaku_.push_back(YakuType::_MENZENTSUMO);
    }


    
    f = is_rinshankaihou();
    if(f != 0){
        fan += f;
        cal_hu_.yaku_.push_back(YakuType::_RINSHANKAIHOU);
    }

    
    
    f = is_chantaiyao();
    if(f != 0){
        fan += f;
        cal_hu_.yaku_.push_back(YakuType::_CHANTAIYAO);
    }
    


    f = is_junchantaiyao();
    if(f != 0){
        fan += f;
        cal_hu_.yaku_.push_back(YakuType::_JUNCHANTAIYAO);
    }

    
    
    
    
    
    
    
    
    
    
    
    return fan;
}









int Player::is_riichi(){
    if(is_double_riichi_){return 2;}
    else if(is_riichi_){return 1;}
    return 0;
}

int Player::is_tanyao(){
    for(auto& temp_tile_ : hand_){
        if(temp_tile_.is_yaojiu()){return 0;}
    }
    for(auto& temp_tile_ : fixed_tiles_){
        if(temp_tile_.is_yaojiu()){return 0;}
    }
    return 1;
}

int Player::is_menzentsumo(){
    if(is_menzen_ && next_tile_.tile_state_ == TileState::_TSUMO){return 1;}
    return 0;
}







int Player::is_rinshankaihou(){
    if(next_tile_.tile_state_ == TileState::_KAN){return 1;}
    return 0;
}




int Player::is_chantaiyao(){
    bool z_tile_ = false;   //字牌，无字牌return 0
    for(auto& temp_tile_ : hand_){
        if(!temp_tile_.is_19() || !temp_tile_.is_tsu()){return 0;}
        if(temp_tile_.is_tsu()){z_tile_ = true;}
    }
    if(is_menzen_){
        if(z_tile_){return 2;}
        else{return 0;}
    }
    else{
        for(auto& temp_tile_ : fixed_tiles_){
            if(!temp_tile_.is_19() || !temp_tile_.is_tsu()){return 0;}
            if(temp_tile_.is_tsu()){z_tile_ = true;}
        }
    }
    if(z_tile_){return 1;}
    return 0;
}




int Player::is_junchantaiyao(){
    for(auto& temp_tile_ : hand_){
        if(!temp_tile_.is_19()){return 0;}
    }
    if(is_menzen_){return 3;}
    else {
        for(auto& temp_tile_ : fixed_tiles_){
            if(!temp_tile_.is_19()){return 0;}
        }
    }
    return 2;
}




#endif  // _MAHJONG_PLAYER_HEADER_