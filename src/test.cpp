
#include<memory>
#include"career.h"
#include"player.h"
#include"enums.h"
#include"state.h"
#include"tile.h"
#include"game.h"

Tile m1(TileType::_1m, false);
Tile m2(TileType::_2m,false);
Tile m3(TileType::_3m,false);
Tile m4(TileType::_4m,false);
Tile m5(TileType::_5m,false);
Tile m6(TileType::_6m,false);    
Tile m7(TileType::_7m,false);
Tile m8(TileType::_8m,false);
Tile m9(TileType::_9m,false);

Tile p1(TileType::_1p,false);
Tile p2(TileType::_2p,false);
Tile p3(TileType::_3p,false);
Tile p4(TileType::_4p,false);
Tile p5(TileType::_5p,false);
Tile p6(TileType::_6p,false);
Tile p7(TileType::_7p,false);
Tile p8(TileType::_8p,false);
Tile p9(TileType::_9p,false);

Tile s1(TileType::_1s, false);
Tile s2(TileType::_2s, false);
Tile s3(TileType::_3s, false);
Tile s4(TileType::_4s, false);
Tile s5(TileType::_5s, false);
Tile s6(TileType::_6s, false);
Tile s7(TileType::_7s, false);
Tile s8(TileType::_8s, false);
Tile s9(TileType::_9s, false);

Tile z1(TileType::_1z, false);
Tile z2(TileType::_2z, false);
Tile z3(TileType::_3z, false);
Tile z4(TileType::_4z, false);
Tile z5(TileType::_5z, false);
Tile z6(TileType::_6z, false);
Tile z7(TileType::_7z, false);

const int num1 = 5;
const int num2 = 13;

Tile TestLists[num1][num2]{
{m1,m1,m1,m2,m3,m4,m5,m6,m7,m8,m9,m9,m9},       //九莲
{m1,m1,m3,m3,m5,m5,m7,m7,p1,p1,p3,p3,z5},        //七对
{m1,m1,m9,p1,p9,s1,s9,z1,z2,z3,z4,z5,z6},        //国士
{m1,m9,p1,p9,s1,s9,z1,z2,z3,z4,z5,z6,z7},        //国士13面
{s2,s2,s3,s3,s4,s4,s6,s6,s6,s8,s8,s8,z6}        //绿一色
};


int main(){
    printf("[ Main Start ]\n");


    Career career_single;
    career_single.read_from_file_single();
    Career career_net;
    career_net.read_from_file_net();


    std::shared_ptr<State> TestState = std::make_shared<State>();
    TestState->tiles_init();

    Player TestPlayer;
    TestPlayer.state_ = TestState;

    for(int i = 0; i < num1; ++i){
        for(int j = 0; j < num2; ++j){
            TestPlayer.hand_.emplace_back(TestLists[i][j]);
        }
        TestPlayer.hand_to_block();
        TestPlayer.hand_.clear();
    }



    printf("[ Main End ]\n");

    return 0;
}