#ifndef _MAHJONG_PLAYER_HEADER_
#define _MAHJONG_PLAYER_HEADER_


#include<vector>
#include<iostream>
#include<cstring>
#include<algorithm>
#include<memory>

#include"basicHpp/basic.hpp"
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


class win_checker{
public:
    // 算番所需的所有信息
    typedef struct CONTEXT_BAN{
        std::shared_ptr<player> player_;
        std::shared_ptr<state> state_;
        
        void set_player_state(std::shared_ptr<player> player, std::shared_ptr<state> state){
            player_ = player;
            state_ = state;
        }

    } context_ban;

    // 算符所需的所有信息
    typedef struct CONTEXT_FU{

    } context_fu;

    // 每个规则算番结果结构体
    typedef struct RESULT{
        YakuType yaku_type;
        int ban_value;

        RESULT(){
            yaku_type = YakuType::_NONETYPE;
            ban_value = 0;
        }
    } result;
};


class basic_ban_calculator{
public:
    basic_ban_calculator()=default;
    virtual win_checker::result calculate(win_checker::context_ban& ctx_ban, count_res& ctx_res) = 0;
    virtual ~basic_ban_calculator() = default;
};

template<typename Derived>
class ban_calculator : public basic_ban_calculator{
public:
    ban_calculator()=default;
    win_checker::result calculate(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        return static_cast<Derived*>(this)->calculate_override(ctx_ban, ctx_res);
    }

};

class count_engine{
public:
    std::vector<std::unique_ptr<basic_ban_calculator>> rules_;

    // 注册算番规则
    void register_rule(std::unique_ptr<basic_ban_calculator> rule){
        rules_.emplace_back(std::move(rule));
    }
    // 计算总番数
    void calculate_total_fan(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        std::vector<win_checker::result> ban_result;
        for(auto& rule: rules_){
            win_checker::result tem = rule->calculate(ctx_ban, ctx_res);
            if(tem.yaku_type != YakuType::_NONETYPE){
                ban_result.emplace_back(tem);
            }
        }
        // Todo: 把ban_result的结果放到count_res中
        for(auto& res: ban_result){
            ctx_res.ban_count_ += res.ban_value;
            ctx_res.yaku_.emplace_back(res.yaku_type);
        }
    }
};

/**
 * 
 * 
 * 
 * 
 * 以下为各个规则的实现
 * 
 * 
 * 
 * 
*/

// 立直和双立直复合
class riichi : public ban_calculator<riichi>{
public:
    win_checker::result calculate_override(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        win_checker::result res;
        if(ctx_ban.player_->is_riichi_ == true){
            res.ban_value = 1;
            res.yaku_type = YakuType::_RIICHI;
            return res;
        }
        if(ctx_ban.player_->is_double_riichi_ == true){
            res.ban_value = 2;
            res.yaku_type = YakuType::_DOUBLERIICHI;
            return res;
        }
        return res;
    }
};

// 断幺
class tanyao : public ban_calculator<tanyao>{
public:
    win_checker::result calculate_override(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        win_checker::result res;
        for(auto& b : ctx_res.blocks_){
            if(b.is_yaojiu()){return res;}
        }
        res.ban_value = 1;
        res.yaku_type = YakuType::_TANYAO;
        return res;
    }
};

// 门前清自摸
class menzentsumo : public ban_calculator<menzentsumo>{
public:
    win_checker::result calculate_override(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        win_checker::result res;
        if(ctx_ban.player_->fixed_tiles_.empty() && ctx_ban.player_->next_tile_.state_ == TileState::_TSUMO){
            res.ban_value = 1;
            res.yaku_type = YakuType::_MENZENTSUMO;
            return res;
        }
        return res;
    }
};


// 自风
class yakuhaiwindself : public ban_calculator<yakuhaiwindself>{
public:
    win_checker::result calculate_override(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        win_checker::result res;
        for(auto b : ctx_res.blocks_){
            if(b.type_ == BlockType::_KOUTSU || b.type_ == BlockType::_KANTSU){
                if(b.tiles_.begin()->type_ == ctx_ban.player_->self_wind_){
                    res.ban_value = 1;
                    res.yaku_type = YakuType::_YAKUHAIWINDSELF;
                    return res;
                }
            }
        }
        return res;
    }
};

// 场风
class yakuhaiwindtable : public ban_calculator<yakuhaiwindtable>{
public:
    win_checker::result calculate_override(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        win_checker::result res;
        for(auto b : ctx_res.blocks_){
            if(b.type_ == BlockType::_KOUTSU || b.type_ == BlockType::_KANTSU){
                if(b.tiles_.begin()->type_ == ctx_ban.state_->table_wind_){
                    res.ban_value = 1;
                    res.yaku_type = YakuType::_YAKUHAIWINDTABLE;
                    return res;
                }
            }
        }
        return res;
    }
};

// 三元牌
class yakuhaisangen : public ban_calculator<yakuhaisangen>{
public:
    win_checker::result calculate_override(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        win_checker::result res;
        int sangen_n = 0;
        for(auto b : ctx_res.blocks_){
            if((b.type_ == BlockType::_KOUTSU || b.type_ == BlockType::_KANTSU) && b.color_ == BlockColor::_SANGEN){
                ++sangen_n;
            }
        }
        if(sangen_n != 0){
            res.ban_value = sangen_n;
            res.yaku_type = YakuType::_YAKUHAISANGEN;
        }
        return res;
    }
};

//Todo
class pinfu : public ban_calculator<pinfu>{
public:
    win_checker::result calculate_override(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        win_checker::result res;
        if(!ctx_ban.player_->fixed_blocks_.empty()){
            return res;
        }
        //Todo


        return res;
    }
};

// 一盃口和二盃口复合
class iipeikou : public ban_calculator<iipeikou>{
public:
    win_checker::result calculate_override(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        win_checker::result res;
        if(!ctx_ban.player_->fixed_blocks_.empty()){
            return res;
        }
        
        size_t count = 0;
        for(auto it = ctx_res.blocks_.begin(); it != ctx_res.blocks_.end(); ++it){
            if(it->type_ == BlockType::_SHUNTSU){
                size_t tt_count = 0;
                for(auto tt = it+1; tt != ctx_res.blocks_.end(); ++tt){
                    if(*tt == *it){
                        ++tt_count;
                    }
                }
                if(tt_count != 0){
                    ++count;
                    it += tt_count;
                }
            }
        }
        if(count == 1){
            res.ban_value = 1;
            res.yaku_type = YakuType::_IIPEIKOU;
        }
        if(count == 2){
            res.ban_value = 3;
            res.yaku_type = YakuType::_RYANPEIKOU;
        }

        return res;
    }
};

class chankan : public ban_calculator<chankan>{
public:
    win_checker::result calculate_override(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        win_checker::result res;
        if(ctx_ban.player_->next_tile_.state_ == TileState::_CHAN){
            res.ban_value = 1;
            res.yaku_type = YakuType::_CHANKAN;
        }
        return res;
    }
};

// 岭上开花
class rinshankaihou : public ban_calculator<rinshankaihou>{
public:
    win_checker::result calculate_override(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        win_checker::result res;
        if(ctx_ban.player_->next_tile_.state_ == TileState::_KAN){
            res.ban_value = 1;
            res.yaku_type = YakuType::_RINSHANKAIHOU;
        }
        return res;
    }
};

//Todo
class haiteiraoyue : public ban_calculator<haiteiraoyue>{
public:
    win_checker::result calculate_override(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        win_checker::result res;


        return res;
    }
};

//Todo
class houteiraoyui : public ban_calculator<houteiraoyui>{
public:
    win_checker::result calculate_override(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        win_checker::result res;

        return res;
    }
};

//Todo, 慕小时候没一发
class ippatsu : public ban_calculator<ippatsu>{
public:
    win_checker::result calculate_override(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        win_checker::result res;

        return res;
    }
};

// 宝牌指示牌 130 
class dora : public ban_calculator<dora>{
public:
    win_checker::result calculate_override(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        win_checker::result res;
        size_t dora_count = 0;
        std::vector<TileType> dora_t = ctx_ban.state_->dora_indicate();
        for(auto& d : dora_t){
            size_t dora_type = (static_cast<size_t>(d) + 1) % 9;
            for(auto& b : ctx_res.blocks_){
                for(auto& t : b.tiles_){
                    if(static_cast<size_t>(t.type_) == dora_type){
                        ++dora_count;
                    }
                }
            }
        }
        if(dora_count != 0){
            res.ban_value = dora_count;
            res.yaku_type = YakuType::_DORA;
        }
        return res;
    }
};

//Todo， 无里宝规则
class uradora : public ban_calculator<uradora>{
public:
    win_checker::result calculate_override(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        win_checker::result res;

        return res;
    }
};

//Todo，无红宝规则
class akadora : public ban_calculator<akadora>{
public:
    win_checker::result calculate_override(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        win_checker::result res;

        return res;
    }
};

// 三色同刻 (2)
class sanshokudoukou : public ban_calculator<sanshokudoukou>{
public:
    win_checker::result calculate_override(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        win_checker::result res;
        int m[5][9]{0};
        for(auto& t : ctx_res.blocks_){
            if(t.type_ == BlockType::_KOUTSU || t.type_ == BlockType::_KANTSU){
                m[static_cast<int>(t.color_)][static_cast<int>(t.tiles_[0].type_)%9] += 1;
            }
        }
        for(int i = 0; i < 9; ++i){
            if((m[0][i] > 0) && (m[1][i] > 0) && (m[2][i] > 0)){
                res.ban_value = 2;
                res.yaku_type = YakuType::_SANSHOKUDOUKOU;
            }
        }
        return res;
    }
};


// 混全带幺九(2)和纯全带幺九(3)复合，副露-1
class chantaiyao : public ban_calculator<chantaiyao>{
public:
    win_checker::result calculate_override(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        win_checker::result res;
        bool z_tile = false;    // 字牌
        bool f_tile = false;    // 副露
        if(!ctx_ban.player_->fixed_blocks_.empty()){
            f_tile = true;
        }
        int test_count = 0;
        for(auto& b : ctx_res.blocks_){
            if(!b.is_19() && !b.is_tsu()){
                ++test_count;
                return res;
            }
            if(b.is_tsu()){
                z_tile = true;
            }
        }
        if(z_tile){
            res.ban_value = 2;
            res.yaku_type = YakuType::_CHANTAIYAO;
        } else {
            res.ban_value = 3;
            res.yaku_type = YakuType::_JUNCHANTAIYAO;
        }
        if(f_tile && res.ban_value != 0){
            --res.ban_value;
        }
        return res;
    }
};

// 一气通贯 IKKITSUUKAN (2)  副露-1
class ikkitsuukan : public ban_calculator<ikkitsuukan>{
public:
    win_checker::result calculate_override(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        win_checker::result res;
        int m[34]{0};
        bool f_tile = false;
        if(!ctx_ban.player_->fixed_tiles_.empty()){
            f_tile = true;
        }
        for(auto& b : ctx_res.blocks_){
            for(auto& t : b.tiles_){
                m[static_cast<int>(t.type_)] += 1;
            }
        }
        for(int i = 0; i < 3; ++i){
            int index = i*9;
            if(m[index] & m[index+1] & m[index+2] & m[index+3] & m[index+4] & m[index+5] & m[index+6] & m[index+7] & m[index+8]){
                res.ban_value = 2;
                res.yaku_type = YakuType::_IKKITSUUKAN;
            }
        }
        if(res.ban_value != 0 && f_tile){
            res.ban_value--;
        }
        return res;
    }
};

// 三色同顺 (2)  副露-1
class sanshokudoujun : public ban_calculator<sanshokudoujun>{
public:
    win_checker::result calculate_override(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        win_checker::result res;
        int m[3][7]{0};
        for(auto& t : ctx_res.blocks_){
            if(t.type_ == BlockType::_SHUNTSU){
                m[static_cast<int>(t.color_)][static_cast<int>(t.tiles_[0].type_)%9] += 1;
            }
        }
        for(int i = 0; i < 7; ++i){
            if((m[0][i] > 0) && (m[1][i] > 0) & (m[2][i] > 0)){
                res.ban_value = 2;
                res.yaku_type = YakuType::_SANSHOKUDOUJUN;
            }
        }
        if((res.ban_value != 0) && !ctx_ban.player_->fixed_tiles_.empty()){
            res.ban_value--;
        }
        return res;
    }
};


// 混一色 (3) 副露-1 清一色(6) 副露-1 字一色(役满)
class honiisou : public ban_calculator<honiisou>{
public:
    win_checker::result calculate_override(win_checker::context_ban& ctx_ban, count_res& ctx_res){
        win_checker::result res;
        int m[5]{0};
        for(auto& t : ctx_res.blocks_){
            m[static_cast<int>(t.color_)] = 1;
        }
        int a = m[0] + m[1] + m[2];
        int b = m[3] + m[4];
        if(b != 0){
            if(a == 1){
                res.ban_value = 3;
                res.yaku_type = YakuType::_HONIISOU;
            } else if(a == 0){
                res.ban_value = 13;
                res.yaku_type = YakuType::_TSUUIISOU;
                return res;
            }
        } else {
            if(a == 1){
                res.ban_value = 6;
                res.yaku_type = YakuType::_CHINIISOU;
            }
        }
        if((res.ban_value != 0) && !ctx_ban.player_->fixed_tiles_.empty()){
            res.ban_value--;
        }
        return res;
    }
};



#endif // _MAHJONG_PLAYER_HEADER_