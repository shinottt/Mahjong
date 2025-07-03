#ifndef _MAHJONG_PLAYER_HEADER_
#define _MAHJONG_PLAYER_HEADER_


#include<vector>
#include<iostream>
#include<cstring>
#include<algorithm>
#include<memory>

#include"basic.hpp"
#include"tile.h"
#include"state.h"

class block_num{
public:
    int num_3 = 0; // 刻[111]
    int num_111 = 0; // 顺[123]
    int num_2 = 0; // 对[11]

    block_num()=default;
    block_num(const block_num& other):num_3(other.num_3), num_111(other.num_111), num_2(other.num_2){}
    void operator=(const block_num& other){
        num_3 = other.num_3;
        num_111 = other.num_111;
        num_2 = other.num_2;
    }
};



/**
 * @class count_res
 * @brief 储存胡牌结果的结构体，可用于比较最高胡牌点数和胡牌结算
*/
class count_res{
public:
    tile tile_;         // 胡的牌
    std::vector<tile> hand_tiles_;    // 胡牌时的手牌
    std::vector<tile> fixed_tiles_;    // 胡牌时的副露牌
    std::vector<block> blocks_;    // 胡牌的块
    std::vector<YakuType> yaku_; // 役种
    int ban_count_ = 0;           // 番数
    int fu_count_ = 0;           // 符数
    int basic_point_ = 0;            //基本点数： 符×2^(番+2)


    count_res()=default;



    // 几个比较函数有问题，之后补充
    bool operator<(const count_res& other) const{
        if(tile_.type_ < other.tile_.type_){
            return true;
        } else{return false;}
    }

    bool operator>(const count_res& other) const{
        if(tile_.type_ > other.tile_.type_){
            return true;
        } else{return false;}
    }

    bool operator==(const count_res& other) const{
        if(tile_.type_ != other.tile_.type_){return false;}
        if(blocks_ != other.blocks_){return false;}
        return true;
    }

    // 控制台打印信息
    void console_print(){
        std::cout<<"胡牌："<<tile_.type_<<std::endl;
        std::cout<<"手牌：";
        for(auto& tile:hand_tiles_){
            std::cout<<tile.type_<<" ";
        }
        std::cout<<std::endl;
        std::cout<<"块：";
        for(auto& block:blocks_){
            block.console_print();
        }
    }
};


/**
 * @class player
 * @brief 表示玩家类
 * 向中控发送信息时同时发送id，中控根据id区分角色
*/
class player{
public:

    CharacterID id_;

    bool is_riichi_;
    bool is_double_riichi_;

    TileType self_wind_;     // 自风

    tile next_tile_;        //下一张可以进行判定的牌


    std::vector<tile> hand_tiles_;  //手中可以打出的牌
    std::vector<tile> fixed_tiles_; //副露的牌
    std::vector<block> hand_blocks_; // 手牌构成的块，暂时用不到，之后用于判断向听数
    std::vector<block> fixed_blocks_; // 副露的牌构成的块

    std::vector<count_res> ten_res_;    // 听牌时计算的胡牌结果

    player()=default;

    void init(CharacterID id){
        id_ = id;
        is_riichi_ = false;
        is_double_riichi_ = false;

    }

    // 向中控请求下一张牌，并接受中控传来的牌信息：摸牌、杠牌
    void receive_tile(RequestType res_type);
    // 向中控发送现在的信息
    void send_info(RequestType res_type);

    void naki();            // 鸣牌行为：吃、碰、杠、胡，不喊就跳过
    // 将手中的牌转化为块，并存入hand_block_中
    void hand_to_block();


private:
    // 缺[2]的听牌类型检查
    bool ten_type_check_2(std::vector<block>& blocks_taken, block_num& block_num, int *hand_tile);
    // 缺[111]或[3]的听牌类型检查
    bool ten_type_check_3(std::vector<block>& blocks_taken, block_num& block_num, int *hand_tile);

    /**
     * 从左到右搜索[111]，放入blocks_taken中
     * @param blocks_taken 储存取到的块
     * @param block_num 储存刻、顺的个数
     * @param hand_tile 手牌中每种牌的个数数组
    */
    void search_111_LtoR(std::vector<block>& blocks_taken, block_num& block_num, int *hand_tile);
    void search_111_RtoL(std::vector<block>& blocks_taken, block_num& block_num, int *hand_tile);
    void search_111_LandR(std::vector<block>& blocks_taken, block_num& block_num, int *hand_tile);  //先从左取，后从右取，依次取完
    void search_111_RandL(std::vector<block>& blocks_taken, block_num& block_num, int *hand_tile);
    void search_3_all(std::vector<block>& blocks_taken, block_num& block_num, int *hand_tile);
    void search_2_aim(std::vector<block>& blocks_taken, block_num& block_num, int *hand_tile, int pos);

    /**
     * 从手牌中取完整的块
     * @param aim_block 目标块，存放取出的手牌
     * @param t1 目标块中第一张牌的类型
    */
    void take_111(block& aim_block, int t1); //从手牌中取[111]放入block中
    void take_3(block& aim_block, int t1);
    void take_2(block& aim_block, int t1);
    // 从手牌中取不完整的块
    void take_1(block& aim_block, int t1);
    void take_11(block& aim_block, int t1);
    void take_1_1(block& aim_block, int t1);


};



#endif // _MAHJONG_PLAYER_HEADER_