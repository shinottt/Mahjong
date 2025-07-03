#ifndef _MAHJONG_SHINOHAYU_CALCULATOR_HEADER_
#define _MAHJONG_SHINOHAYU_CALCULATOR_HEADER_

#include"player.h"
#include"state.h"

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




#endif  //_MAHJONG_SHINOHAYU_CALCULATOR_HEADER_