#ifndef _MAHJONG_PLAYER_HEADER_
#define _MAHJONG_PLAYER_HEADER_


#include<memory>
#include<algorithm>

#include"state.h"
#include"tile.h"
#include"sn_log.h"


/*

player类中算分

*/




class Blocknum{
public:
    int num_3 = 0;  //刻
    int num_111 = 0;    //顺子
    int num_2 = 0;  //对

    Blocknum()=default;
    Blocknum(const Blocknum& aim):num_3(aim.num_3), num_111(aim.num_111), num_2(aim.num_2){}

    void operator=(const Blocknum& aim){
        num_3 = aim.num_3;
        num_111 = aim.num_111;
        num_2 = aim.num_2;
    }
};


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
    Hu(const Hu& aim):type_(aim.type_), ten_(aim.ten_), yaku_(aim.yaku_), blocks_(aim.blocks_), man_(aim.man_), fu_(aim.fu_), fan_(aim.fan_), basic_point_(aim.basic_point_){}
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

    bool operator<(const Hu& aim){
        if(type_ < aim.type_){
            return true;
        }else if(type_ > aim.type_){
            return false;
        } else{
            if(basic_point_ < aim.basic_point_){
                return true;
            } else {return false;}
        }
    }

    bool operator>(const Hu& aim){
        if(type_ > aim.type_){
            return true;
        } else if(type_ < aim.type_){
            return false;
        } else{
            if(basic_point_ > aim.basic_point_){
                return true;
            } else {return false;}
        }
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
    //13张牌判断是否听牌
    void is_ten(Blocknum& hand_block_num_, int* handnum_);


    int cal_yaku(Hu&);    //使用 Player(class) 和 Hu(class) 中的数据计算番数和役：一般胡牌


    //很乱
    void search_111_LtoR(std::vector<Block>& block_temp_, Blocknum& block_num_, int* m_);   //从左往右取[111]顺子放入block_temp_中
    void search_111_RtoL(std::vector<Block>& block_temp_, Blocknum& block_num_, int* m_);   //从右往左取[111]顺子放入block_temp_中
    void search_111_LandR(std::vector<Block>& block_temp_, Blocknum& block_num_, int* m_);   //从左右两边取[111]顺子放入block_temp_中
    void search_111_RandL(std::vector<Block>& block_temp_, Blocknum& block_num_, int* m_);   //从右左两边取[111]顺子放入block_temp_中
    void search_3_all(std::vector<Block>& block_temp_, Blocknum& block_num_, int* m_);  //取手中所有[3]
    void search_2_aim(std::vector<Block>& block_temp_, Blocknum& block_num_, int* m_, int pos);

    //缺[2]的听牌型判断
    void hu_type_1(std::vector<Block>& block_temp_, Blocknum& block_num_, int* m_);
    //[111]不完整的听牌型判断
    void hu_type_2(std::vector<Block>& block_temp_, Blocknum& block_num_, int* m_);

    void take_111(Block& block, int t1);        //从手牌中取[111]顺子放入block中
    void take_3(Block& block, int t1);          //取[3]刻子
    void take_2(Block& block, int t1);          //取[2]对子

    void take_1(Block& block, int t1);          //取[1]单张牌
    void take_11(Block& block, int t1);         //取[11]挨着的顺子2张
    void take_1_1(Block& block, int t1);        //取[1 1]不挨着的顺子2张


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






    //测试打印函数
    void print_hand_();
    void print_ten_();
    void print_hu_();

};


inline void Player::print_hand_(){
    if(!hand_.empty()){
        std::cout<<"hand tiles: ";
        for(int i = 0; i < hand_.size(); ++i){
            std::cout<<static_cast<int>(hand_[i].type_)<<" ";
        }
        std::cout<<std::endl;

    }else{
        sn_consolelog_info("No hand tiles");
    }

}


inline void Player::print_ten_(){
    if(!ten_.empty()){
        for(int i = 0; i < ten_.size(); ++i){
            std::cout << "ten block tiles: ";
            for(int j = 0; j < ten_[i].size(); ++j){
                for(int k = 0; k < ten_[i][j].tiles_.size(); ++k){
                    std::cout << static_cast<int>(ten_[i][j].tiles_[k].type_) <<" ";
                }
            }
            std::cout << std::endl;
        }
    }else{
        sn_consolelog_info("No ten blocks");
    }
}

inline void Player::print_hu_(){
    if(!hu_.empty()){
        for(int i = 0; i < hu_.size(); ++i){
            std::cout<<"hu tile type: "<<static_cast<int>(hu_[i].type_)<<"  "<<"ten type: "<<static_cast<int>(hu_[i].ten_)<<std::endl;
            std::cout<<"class hu tiles: ";
            for(int j = 0; j < hu_[i].blocks_.size(); ++j){
                for(int k = 0; k < hu_[i].blocks_[j].tiles_.size(); ++k){
                    std::cout<<static_cast<int>(hu_[i].blocks_[j].tiles_[k].type_)<<" ";
                }
            }
            std::cout<<std::endl;
        }
    }else{
        sn_consolelog_info("No hu blocks");
    }
}






#endif  // _MAHJONG_PLAYER_HEADER_