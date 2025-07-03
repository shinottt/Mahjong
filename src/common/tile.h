#ifndef _MAHJONG_SHINOHAYU_TILE_HEADER_
#define _MAHJONG_SHINOHAYU_TILE_HEADER_
/**
 * @file tile.h
 * @brief tile类，block类的定义
*/

#include<vector>
#include<iostream>
#include"enum.h"


/**
 * @class tile
 * @brief 牌类，用于表示牌局中的每一张牌
*/
class tile{
public:
    tile(){}
    tile(TileType type):type_(type){}
    tile(const tile& t):type_(t.type_), is_red_(t.is_red_){
        // 可能不需要在创建时确定状态
        state_ = TileState::_UNIMPORTANT;
    }


    /**
     * @brief 设置是否为红宝牌
     * @param is_red 是否为红宝牌
    */
    void set_red(bool is_red);
    /**
     * @brief 判断是否为绿牌，包括发，2索，3索，4索，6索，8索
    */
    bool is_green() const;
    /**
     * @brief 判断是否为三元牌
    */
    bool is_sangen() const;
    /**
     * @brief 判断是否为风牌
    */
    bool is_wind() const;   
    /**
     * @brief 判断是否为字牌，包括风牌和三元牌
    */
    bool is_tsu() const;   
    /**
     * @brief 判断是否老头牌，包括 1,9万，1,9饼，1,9索
    */
    bool is_19() const;
    /**
    * @brief 判断是否为幺九牌，包括老头牌和字牌
    */
    bool is_yaojiu() const;


    /**
     * @brief 仅比较两张牌的种类是否相同
    */
    friend bool operator==(const tile&, const tile&);
    /**
     * @brief 仅比较两张牌的种类是否不同
    */
    friend bool operator!=(const tile&, const tile&);
    /**
     * @brief 仅比较两张牌的种类大小，用于排序，1m < 2m < ... < 9p < 1s < 2s < ... < 9s < 1z < 2z < ... < 7z
    */
    friend bool operator<(const tile&, const tile&);
    /**
     * @brief 仅比较两张牌的种类大小，用于排序，1m > 2m > ... > 9p > 1s > 2s > ... > 9s > 1z > 2z > ... > 7z
    */
    friend bool operator>(const tile&, const tile&);


    TileType type_;     //牌的种类
    bool is_red_;       //规则可能没有红宝

    TileState state_;
};


/**
 * @class block
 * @brief 块类，用于表示一组牌，用于胡牌判定
*/
class block{
public:
    block(){}
    block(BlockType type, BlockColor color, int number):type_(type), color_(color), number_(number){}
    block(const block& b):type_(b.type_), color_(b.color_), number_(b.number_), tiles_(b.tiles_){}


    // 块需要是完整的才能使用以下判断

    bool is_wind() const;

	bool is_sangen() const;
	// 判断块是否为老头牌块，例：[111],[789]
	bool is_19() const;

    bool is_tsu() const;

    bool is_yaojiu() const;

    // 排序用比较大小函数
    bool operator<(const block& other) const{
        if(type_ < other.type_)
            return true;
        else if(type_ > other.type_)
            return false;
        else if(color_ < other.color_)
            return true;
        else if(color_ > other.color_)
            return false;
        else if(tiles_[0].type_ < other.tiles_[0].type_)
            return true;
        else return false;
    }

    bool operator>(const block& other) const{
        if(type_ > other.type_)
            return true;
        else if(type_ < other.type_)
            return false;
        else if(color_ > other.color_)
            return true;
        else if(color_ < other.color_)
            return false;
        else if(tiles_[0].type_ > other.tiles_[0].type_)
            return true;
        else return false;
    }

    bool operator==(const block& other) const{
        if(type_ != other.type_){return false;}
        if(color_ != other.color_){return false;}
        if(tiles_[0].type_ != other.tiles_[0].type_){return false;}
        return true;
    }

    bool operator!=(const block& other) const{
        return !(*this == other);
    }


    //测试打印
    void console_print() const {
        for(auto& t: tiles_)
            std::cout << static_cast<int>(t.type_) << " ";
    }



    BlockType type_;    //块的类型
    BlockColor color_;  //块的种类，例如万、饼、索、风、三元
    int number_;        //块中牌数

    std::vector<tile> tiles_;   //块中的牌
};



/*
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
*/


#endif  // _MAHJONG_SHINOHAYU_TILE_HEADER_