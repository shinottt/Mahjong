#ifndef _MAHJONG_STATE_HEADER_
#define _MAHJONG_STATE_HEADER_


#include<random>         //std::random_device
#include<algorithm>      //std::shuffle
#include<array>
#include<memory>
#include<string>
#include<fstream>
#include"tile.h"

/*
规则设置
*/

const std::string RULE_DEFAULT_FILE_PATH = "rule_default.dat";
const std::string RULE_FILE_PATH = "rule.dat";




class Rule{
public:

    bool 食断_;          //食断
    bool ippatsu_;      //一发

    bool dora_kan_;     //杠宝牌
    bool dora_ura_;     //里宝牌

    int max_renchan_;   //最大连庄数



    Rule()=default;

    //设置默认规则
    void set_default(){
        std::fstream file_;
        file_.open(RULE_DEFAULT_FILE_PATH.c_str(), std::ios::binary | std::ios::in);
        while(!file_.peek() != EOF){
            file_.read((char*)this, sizeof(*this));
        }
        file_.close();
        printf("default rule load from file: %s\n", RULE_DEFAULT_FILE_PATH.c_str());
    }

    //读取玩家的规则设置
    void read_rule(){
        std::fstream file_;
        file_.open(RULE_FILE_PATH.c_str(), std::ios::binary | std::ios::in);
        while(file_.peek() != EOF){
            file_.read((char*)this, sizeof(*this));
        }
        file_.close();
        printf("rule loaded from file: %s\n", RULE_FILE_PATH.c_str());
    }

    void save_rule(){
        std::fstream file_;
        file_.open(RULE_FILE_PATH.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
        file_.write((const char*)this, sizeof(*this));
        file_.close();
        printf("rule saved to file: %s\n", RULE_FILE_PATH.c_str());
    }


};





/*
场上的情况，包括风，牌库
*/
class State{
public:
    TileType wind_;             //场风

    int tile_index_;            //当前摸到的牌的索引：0-121

    int num_richii_;            //立直数量：四家立直
    int num_kan_;               //杠的数量：四杠散了

    std::shared_ptr<Rule> rule_;    //规则设置


	/*
	0-121: 正常摸的牌。122-131: 宝牌指示牌。132-135: 岭上牌
	宝牌指示牌的对应关系：
	上层        127(第一张)    128    129    130    131
	下层        122(第一张)    123    124    125    126
	岭上牌从后往前摸
	*/
    std::array<Tile, 136> tiles_;   //牌库：136张牌



    State()=default;



    void tiles_init();           //牌库初始化，暂时不放红宝牌   


};




inline void State::tiles_init(){
    for(int i=0; i<136; i++){
        tiles_[i].type_ = static_cast<TileType>(i/4);
        tiles_[i].tile_state_ = TileState::_TSUMO;
    }

    //132-135: 岭上牌，状态为_KAN
    for(int i = 132; i<=135; ++i){
        tiles_[i].tile_state_ = TileState::_KAN;
    }

    //获取硬件噪声产生随机数
    std::random_device rd;
    std::default_random_engine dre(rd());
    //简单的随机洗牌
    std::shuffle(tiles_.begin(), tiles_.end(), rd);

}




#endif   // _MAHJONG_STATE_HEADER_