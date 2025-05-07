/**
 * @author shinottt
 * @brief 主函数
*/

#include<iostream>
#include"game.h"
#include"player.h"
#include"state.h"
#include<SDL3/SDL.h>
#include<SDL3/SDL_main.h>

#include"sn_log.hpp"
#include"net_server.hpp"

tile m1{TileType::_1m};
tile m2{TileType::_2m};
tile m3{TileType::_3m};
tile m4{TileType::_4m};
tile m5{TileType::_5m};
tile m6{TileType::_6m};
tile m7{TileType::_7m};
tile m8{TileType::_8m};
tile m9{TileType::_9m};
tile p1{TileType::_1p};
tile p2{TileType::_2p};
tile p3{TileType::_3p};
tile p4{TileType::_4p};
tile p5{TileType::_5p};
tile p6{TileType::_6p};
tile p7{TileType::_7p};
tile p8{TileType::_8p};
tile p9{TileType::_9p};
tile s1{TileType::_1s};
tile s2{TileType::_2s};
tile s3{TileType::_3s};
tile s4{TileType::_4s};
tile s5{TileType::_5s};
tile s6{TileType::_6s};
tile s7{TileType::_7s};
tile s8{TileType::_8s};
tile s9{TileType::_9s};
tile z1{TileType::_1z};
tile z2{TileType::_2z};
tile z3{TileType::_3z};
tile z4{TileType::_4z};
tile z5{TileType::_5z};
tile z6{TileType::_6z};
tile z7{TileType::_7z};

// 测试
void test_yaku_cal();

auto log_t = logger::get_instance();

int main(int argc, char* argv[]){

    log_t->log_init_file("log.txt");
    log_t->log_start();
    auto winsock = sock_manager::get_instance();

    SDL_Log("Main function is running...\n");
    
    /*
    SDL_Log("Game is running...\n");
    auto game_ins = game::get_instance();
    game_ins->init("咲慕流年", 1600, 900);
    game_ins->run();
    SDL_Log("Game is over...\n");
    */


    language_manager::get_instance()->init(Language::Chinese);
    /*
    std::unique_ptr<state> test_state = std::make_unique<state>();
    test_state->init();
    */

    test_yaku_cal();


    SDL_Log("Main function is over...\n");
    return 0;
}


#define TEST_YAKU_FUNC(TILES, YAKU) log_t->log(LogLevel::DEBUG_,LogType::FILE,"{} 测试开始，计算结果如下：", #YAKU); \
test_context.player_->hand_tiles_ = TILES; \
test_context.player_->hand_to_block();  \
for(auto& p : test_context.player_->ten_res_){  \
        test_engine.calculate_total_fan(test_context, p);   \
        log_t->log(LogLevel::DEBUG_, LogType::FILE, "番数: {}", p.ban_count_);    \
        log_t->log(LogLevel::DEBUG_, LogType::FILE,"役种：");  \
        for(auto& q : p.yaku_){         \
            if(!p.yaku_.empty()){log_t->log(LogLevel::DEBUG_,LogType::FILE,yaku_to_string(q));}        \
        }       \
        log_t->log(LogLevel::DEBUG_,LogType::FILE,"{} 测试结束", #YAKU);   \
        log_t->log(LogLevel::DEBUG_,LogType::FILE," ");   \
    }

void test_yaku_cal(){

    // 注册算番规则
    count_engine test_engine;
    test_engine.register_rule(std::make_unique<riichi>());
    test_engine.register_rule(std::make_unique<tanyao>());
    test_engine.register_rule(std::make_unique<menzentsumo>());
    test_engine.register_rule(std::make_unique<yakuhaiwindself>());
    test_engine.register_rule(std::make_unique<yakuhaiwindtable>());
    test_engine.register_rule(std::make_unique<yakuhaisangen>());
    test_engine.register_rule(std::make_unique<pinfu>());
    test_engine.register_rule(std::make_unique<iipeikou>());
    test_engine.register_rule(std::make_unique<chankan>());
    test_engine.register_rule(std::make_unique<rinshankaihou>());

    
    test_engine.register_rule(std::make_unique<dora>());
    
    test_engine.register_rule(std::make_unique<sanshokudoukou>());
    test_engine.register_rule(std::make_unique<chantaiyao>());
    test_engine.register_rule(std::make_unique<ikkitsuukan>());
    test_engine.register_rule(std::make_unique<sanshokudoujun>());
    test_engine.register_rule(std::make_unique<honiisou>());


    std::shared_ptr<player> test_player = std::make_shared<player>();
    win_checker::context_ban test_context;
    std::shared_ptr<state> test_state = std::make_shared<state>();
    test_state->init();
    test_context.set_player_state(test_player, test_state);

    
    std::vector<tile> test_riichi = {m1,m2,m3,m4,m5,m6,m7,m8,m9, s1,s1,s1,s3};
    test_player->is_riichi_ =true;
    test_player->is_double_riichi_ = false;
    TEST_YAKU_FUNC(test_riichi, riichi)
    test_player->is_riichi_ = false;
    test_player->is_double_riichi_ = true;
    TEST_YAKU_FUNC(test_riichi, double_riichi)
    test_player->is_double_riichi_ = false;

    std::vector<tile> test_tanyao = {m2, m3, m4, m6, m7, m8, p2, p2, p2, s3, s3, s3, s5};
    TEST_YAKU_FUNC(test_tanyao, tanyao)

    test_player->next_tile_.state_ = TileState::_TSUMO;
    TEST_YAKU_FUNC(test_tanyao, menzentsumo)
    test_player->next_tile_.state_ = TileState::_DA;

    std::vector<tile> test_wind = {m1,m2,m3,m4,m5,m6,m7,m8,m9, p2,z1,z1,z1};
    test_player->self_wind_ = TileType::_1z;
    TEST_YAKU_FUNC(test_wind, yakuhaiwindself)

    test_state->table_wind_ = TileType::_1z;
    TEST_YAKU_FUNC(test_wind, yakuhaiwindtable)

    std::vector<tile> test_sangen = {m1,m2,m3,m4,m5,m6, p2,z5,z5,z5,z6,z6,z6};
    TEST_YAKU_FUNC(test_sangen,yakuhaisangen)
    
    std::vector<tile> test_iipeikou_1 = {m1 ,m1, m1, m2, m2,m2, m3, m3, m3, p2, p2, p2, p3};
    TEST_YAKU_FUNC(test_iipeikou_1, iipeikou_1)

    
    std::vector<tile> test_iipeikou_2 = {m2,m2,m3,m3,m4,m4, p1,p1,p2,p2,p3,p3, s2};
    TEST_YAKU_FUNC(test_iipeikou_2,iipeikou_2)

    
    TileType dora_temp = test_state->total_tiles_[130].type_;
    test_state->total_tiles_[130].type_ = TileType::_2m;
    std::vector<tile> test_dora = {m2,m3,m3,m3,p2,p3,p4,p2,p3,p4, p6,p6,p6};
    TEST_YAKU_FUNC(test_dora, dora)
    test_state->total_tiles_[130].type_ = dora_temp;


    std::vector<tile> test_sanshokudoukou = {m1,m1,m1,p1,p1,p1,s1,s1,s1,s7,s8,s9, s9};
    TEST_YAKU_FUNC(test_sanshokudoukou, sanshokudoukou)

    std::vector<tile> test_chantaiyao = {m1,m2,m3, m7,m8,m9, p1,p2,p3, s9,s9,s9, z1};
    TEST_YAKU_FUNC(test_chantaiyao, chantaiyao)

    std::vector<tile> test_junchantaiyao = {m1, m2, m3, m7, m8, m9, p1, p2, p3, s1, s1 ,s1, s9};
    TEST_YAKU_FUNC(test_junchantaiyao, junchantaiyao)

    std::vector<tile> test_ikkitsuukan = {m1, m2, m3, m4, m5, m6, m7, m8, m9, s1, s2, s3, s9};
    TEST_YAKU_FUNC(test_ikkitsuukan, ikkitsuukan)

    std::vector<tile> test_sanshokudoujun = {m1,m2,m3,p1,p2,p3,s1,s2,s3, s7,s8,s9, z2};
    TEST_YAKU_FUNC(test_sanshokudoujun, sanshokudoujun)

    std::vector<tile> test_honiisou = {m1,m1,m1,m3,m3,m3,m4,m5,m6,m7,m8,m9, z2};
    TEST_YAKU_FUNC(test_honiisou, honiisou)

    std::vector<tile> test_chiniisou = {m1,m1,m1,m3,m3,m3,m4,m4,m4,m6,m7,m7,m8};
    TEST_YAKU_FUNC(test_chiniisou, chiniisou)

    std::vector<tile> test_tsuuiisou = {z1,z1,z1,z2,z2,z2,z3,z3,z3,z5,z5,z5,z7};
    TEST_YAKU_FUNC(test_tsuuiisou, tsuuiisou)

}

