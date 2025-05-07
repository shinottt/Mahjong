#include"player.h"


void player::hand_to_block(){
    if(!ten_res_.empty()){
        ten_res_.clear();
    }

    // 统计手中的牌数
    int hand_tile_num[34]{0};
    int mt[34]{0};      //hand_tile_num的副本
    for(int i = 0; i < hand_tiles_.size(); i++){
        int t = static_cast<int>(hand_tiles_[i].type_);
        hand_tile_num[t]++;
    }

    // 统计已经成块的副露
    block_num total_block_num;      // 用于判断是否听牌，[111]+[2]+[3] = 5 时听牌
    block_num tbn;  //total_block_num的副本

    if(!fixed_blocks_.empty()){
        for(int i = 0; i < fixed_blocks_.size(); ++i){
            switch(static_cast<int>(fixed_blocks_[i].type_)){
                case 1:
                    total_block_num.num_111++;
                    break;
                case 2:
                    total_block_num.num_3++;
                    break;
                default:
                    break;
            }
        }
    }
    std::vector<block> blocks_taken;    //存放每次取出的块

/**
 * 
 * 缺[2]时的取块顺序：
 * 1. 先取[111] (1.LtoR   2.RtoL  3.LandR 4. RandL)，再取[3]
 * 2. 先取所有[3]，再取[111](1.LtoR   2.RtoL  3.LandR 4. RandL)
 * 
 * 缺[111]或[3]时的取块顺序：
 * 1. 先取[2], 再取[111] (1.LtoR   2.RtoL  3.LandR 4. RandL)，再取[3]
 * 2. 先取[2], 再取[3]，再取[111](1.LtoR   2.RtoL  3.LandR 4. RandL)
 * 
*/

/**
 * 缺[2]
*/

    // 1. 先 [111](LtoR), 再 [3]
    memcpy(mt, hand_tile_num, 34*sizeof(int));
    tbn = total_block_num;
    search_111_LtoR(blocks_taken, tbn, mt);
    search_3_all(blocks_taken, tbn, mt);
    ten_type_check_2(blocks_taken, tbn, mt);
    if(!blocks_taken.empty()){blocks_taken.clear();}

    // 2. 先 [111](RtoL), 再 [3]
    memcpy(mt, hand_tile_num, 34*sizeof(int));
    tbn = total_block_num;
    search_111_RtoL(blocks_taken, tbn, mt);
    search_3_all(blocks_taken, tbn, mt);
    ten_type_check_2(blocks_taken, tbn, mt);
    if(!blocks_taken.empty()){blocks_taken.clear();}

    // 3. 先 [111](LandR), 再 [3]
    memcpy(mt, hand_tile_num, 34*sizeof(int));
    tbn = total_block_num;
    search_111_LandR(blocks_taken, tbn, mt);
    search_3_all(blocks_taken, tbn, mt);
    ten_type_check_2(blocks_taken, tbn, mt);
    if(!blocks_taken.empty()){blocks_taken.clear();}

    // 4. 先 [111](RandL), 再 [3]
    memcpy(mt, hand_tile_num, 34*sizeof(int));
    tbn = total_block_num;
    search_111_RandL(blocks_taken, tbn, mt);
    search_3_all(blocks_taken, tbn, mt);
    ten_type_check_2(blocks_taken, tbn, mt);
    if(!blocks_taken.empty()){blocks_taken.clear();}

    // 5. 先 [3], 再 [111](LtoR)
    memcpy(mt, hand_tile_num, 34*sizeof(int));
    tbn = total_block_num;
    search_3_all(blocks_taken, tbn, mt);
    search_111_LtoR(blocks_taken, tbn, mt);
    ten_type_check_2(blocks_taken, tbn, mt);
    if(!blocks_taken.empty()){blocks_taken.clear();}

    // 6. 先 [3], 再 [111](RtoL)
    memcpy(mt, hand_tile_num, 34*sizeof(int));
    tbn = total_block_num;
    search_3_all(blocks_taken, tbn, mt);
    search_111_RtoL(blocks_taken, tbn, mt);
    ten_type_check_2(blocks_taken, tbn, mt);
    if(!blocks_taken.empty()){blocks_taken.clear();}

    // 7. 先 [3], 再 [111](LandR)
    memcpy(mt, hand_tile_num, 34*sizeof(int));
    tbn = total_block_num;
    search_3_all(blocks_taken, tbn, mt);
    search_111_LandR(blocks_taken, tbn, mt);
    ten_type_check_2(blocks_taken, tbn, mt);
    if(!blocks_taken.empty()){blocks_taken.clear();}

    // 8. 先 [3], 再 [111](RandL)
    memcpy(mt, hand_tile_num, 34*sizeof(int));
    tbn = total_block_num;
    search_3_all(blocks_taken, tbn, mt);
    search_111_RandL(blocks_taken, tbn, mt);
    ten_type_check_2(blocks_taken, tbn, mt);
    if(!blocks_taken.empty()){blocks_taken.clear();}

/**
 * 缺[111]或[3]
*/
    // 先找出所有[2]的位置
    std::vector<int> pos_2;
    for(int i = 0; i < 34; ++i){
        if(hand_tile_num[i] >= 2){
            pos_2.emplace_back(i);
        }
    }
    // 遍历每个[2]的位置
    for(int i = 0; i < pos_2.size(); ++i){
        // 1. 先 [2], 再 [111](LtoR), 再 [3]
        memcpy(mt, hand_tile_num, 34*sizeof(int));
        tbn = total_block_num;
        search_2_aim(blocks_taken, tbn, mt, pos_2[i]);
        search_111_LtoR(blocks_taken, tbn, mt);
        search_3_all(blocks_taken, tbn, mt);
        ten_type_check_3(blocks_taken, tbn, mt);
        if(!blocks_taken.empty()){blocks_taken.clear();}

        // 2. 先 [2], 再 [111](RtoL), 再 [3]
        memcpy(mt, hand_tile_num, 34*sizeof(int));
        tbn = total_block_num;
        search_2_aim(blocks_taken, tbn, mt, pos_2[i]);
        search_111_RtoL(blocks_taken, tbn, mt);
        search_3_all(blocks_taken, tbn, mt);
        ten_type_check_3(blocks_taken, tbn, mt);
        if(!blocks_taken.empty()){blocks_taken.clear();}

        // 3. 先 [2], 再 [111](LandR), 再 [3]
        memcpy(mt, hand_tile_num, 34*sizeof(int));
        tbn = total_block_num;
        search_2_aim(blocks_taken, tbn, mt, pos_2[i]);
        search_111_LandR(blocks_taken, tbn, mt);
        search_3_all(blocks_taken, tbn, mt);
        ten_type_check_3(blocks_taken, tbn, mt);
        if(!blocks_taken.empty()){blocks_taken.clear();}

        // 4. 先 [2], 再 [111](RandL), 再 [3]
        memcpy(mt, hand_tile_num, 34*sizeof(int));
        tbn = total_block_num;
        search_2_aim(blocks_taken, tbn, mt, pos_2[i]);
        search_111_RandL(blocks_taken, tbn, mt);
        search_3_all(blocks_taken, tbn, mt);
        ten_type_check_3(blocks_taken, tbn, mt);
        if(!blocks_taken.empty()){blocks_taken.clear();}

        // 5. 先 [2], 再 [3], 再 [111](LtoR)
        memcpy(mt, hand_tile_num, 34*sizeof(int));
        tbn = total_block_num;
        search_2_aim(blocks_taken, tbn, mt, pos_2[i]);
        search_3_all(blocks_taken, tbn, mt);
        search_111_LtoR(blocks_taken, tbn, mt);
        ten_type_check_3(blocks_taken, tbn, mt);
        if(!blocks_taken.empty()){blocks_taken.clear();}

        // 6. 先 [2], 再 [3], 再 [111](RtoL)
        memcpy(mt, hand_tile_num, 34*sizeof(int));
        tbn = total_block_num;
        search_2_aim(blocks_taken, tbn, mt, pos_2[i]);
        search_3_all(blocks_taken, tbn, mt);
        search_111_RtoL(blocks_taken, tbn, mt);
        ten_type_check_3(blocks_taken, tbn, mt);
        if(!blocks_taken.empty()){blocks_taken.clear();}

        // 7. 先 [2], 再 [3], 再 [111](LandR)
        memcpy(mt, hand_tile_num, 34*sizeof(int));
        tbn = total_block_num;
        search_2_aim(blocks_taken, tbn, mt, pos_2[i]);
        search_3_all(blocks_taken, tbn, mt);
        search_111_LandR(blocks_taken, tbn, mt);
        ten_type_check_3(blocks_taken, tbn, mt);
        if(!blocks_taken.empty()){blocks_taken.clear();}

        // 8. 先 [2], 再 [3], 再 [111](RandL)
        memcpy(mt, hand_tile_num, 34*sizeof(int));
        tbn = total_block_num;
        search_2_aim(blocks_taken, tbn, mt, pos_2[i]);
        search_3_all(blocks_taken, tbn, mt);
        search_111_RandL(blocks_taken, tbn, mt);
        ten_type_check_3(blocks_taken, tbn, mt);
        if(!blocks_taken.empty()){blocks_taken.clear();}

    }

    std::cout<<"判断完毕"<<std::endl;

    // 检查 ten_res_ 里是否有重复的听牌结果
    for(auto& res_temp : ten_res_){
        std::sort(res_temp.blocks_.begin(), res_temp.blocks_.end());
    }
    std::cout<<"排序完毕"<<std::endl;
    std::sort(ten_res_.begin(), ten_res_.end());
    for(int i = 0; i < ten_res_.size()-1; ++i){
        for(int j = i+1; j < ten_res_.size(); ++j){
            if(ten_res_[i] == ten_res_[j]){
                ten_res_.erase(ten_res_.begin()+j);
                --j;
            }
        }
    }
    std::cout<<"去重完毕"<<std::endl;

    // 控制台打印，测试
    for(auto& res : ten_res_){
        res.console_print();
        std::cout<<std::endl;
    }


    /*  测试 search_111_LtoR 和 RtoL ，没问题
    std::vector<block> blocks_taken_1;
    std::vector<block> blocks_taken_2;
    search_111_LtoR(blocks_taken_1, total_block_num, hand_tile_num);
    search_111_RtoL(blocks_taken_2, total_block_num, hand_tile_num);

    std::cout<<"111 LtoR: "<<std::endl;
    if(!blocks_taken_1.empty()){
        for(auto& b1 : blocks_taken_1){
            b1.console_print();
        }
    }

    std::cout<<"111 RtoL: "<<std::endl;
    if(!blocks_taken_2.empty()){
        for(auto& b2 : blocks_taken_2){
            b2.console_print();
        }
    }
    */

    /*  测试 search_111_LandR 和 RandL ，没问题
    std::vector<block> blocks_taken_1;
    std::vector<block> blocks_taken_2;
    search_111_LandR(blocks_taken_1, total_block_num, hand_tile_num);
    search_111_RandL(blocks_taken_2, total_block_num, hand_tile_num);

    std::cout<<"111 LandR: "<<std::endl;
    if(!blocks_taken_1.empty()){
        for(auto& b1 : blocks_taken_1){
            b1.console_print();
        }
    }

    std::cout<<"111 RandL: "<<std::endl;
    if(!blocks_taken_2.empty()){
        for(auto& b2 : blocks_taken_2){
            b2.console_print();
        }
    }
    */



}   

bool player::ten_type_check_2(std::vector<block>& blocks_taken, block_num& block_num, int *hand_tile){
    if(!(block_num.num_111 + block_num.num_3 == 4)){
        return false;
    }
    int *mt = hand_tile;
    count_res res;
    // 把剩下的一张取出来
    for(int i = 0; i < 34; ++i){
        if(mt[i] == 1){
            mt[i]--;
            block_num.num_2++;
            // 加一张牌组成完整的[2]
            tile ten_tile(static_cast<TileType>(i));
            block temp(BlockType::_TOITSU, TileTypeToBlockColor(static_cast<TileType>(i)), 2);
            take_1(temp, i);
            temp.tiles_.emplace_back(ten_tile);
            std::sort(temp.tiles_.begin(), temp.tiles_.end());
            blocks_taken.emplace_back(temp);
            // 听牌结果放到res里
            res.tile_ = ten_tile;
            res.hand_tiles_ = this->hand_tiles_;
            res.fixed_tiles_ = this->fixed_tiles_;
            res.blocks_ = blocks_taken;
            this->ten_res_.emplace_back(res);
            break;
        }
    }
    return true;
}

bool player::ten_type_check_3(std::vector<block>& blocks_taken, block_num& block_num, int *hand_tile){
    int* mt = hand_tile;
    if(!(block_num.num_111 + block_num.num_3 == 3 && block_num.num_2 == 1)){
        return false;
    }

    //判断剩下的是将[2]，还是[11]，还是嵌张[1_1]
    count_res res;
    res.hand_tiles_ = this->hand_tiles_;
    res.fixed_tiles_ = this->fixed_tiles_;
    // [2]
    bool ting_2 = false;
    for(int i = 0; i < 34 && !ting_2; ++i){
        if(mt[i] == 2){
            ting_2 = true;
            mt[i] -= 2;
            block_num.num_3++;

            // 加一张牌组成完整的[3]
            tile ten_tile(static_cast<TileType>(i));
            block temp(BlockType::_KOUTSU, TileTypeToBlockColor(static_cast<TileType>(i)), 3);
            take_2(temp, i);
            temp.tiles_.emplace_back(ten_tile);
            std::sort(temp.tiles_.begin(), temp.tiles_.end());
            blocks_taken.emplace_back(temp);
            // 听牌结果放到res里
            res.tile_ = ten_tile;
            res.blocks_ = blocks_taken;
            this->ten_res_.emplace_back(res);
            break;
        }
    }
    if(ting_2){return true;}
    // [11]
    bool ting_11 = false;
    for(int i = 0; i < 27 && !ting_11; i += 9){
        for(int j = 9*i; j < 9*i+8 && !ting_11; ++j){
            if(mt[j] == 1 && mt[j+1] == 1 && !ting_11){
                mt[j]--;
                mt[j+1]--;
                block_num.num_111++;
                ting_11 = true;
                if(j == 9*i || j+1 == 9*i+8){
                    tile ten_tile;
                    if(j == 9*i){
                        ten_tile.type_=static_cast<TileType>(j+2);
                    }
                    if(j+1 == 9*i+8){
                        ten_tile.type_=static_cast<TileType>(j-1);
                    }
                    block temp(BlockType::_SHUNTSU, TileTypeToBlockColor(static_cast<TileType>(j)), 3);
                    take_11(temp, j);
                    temp.tiles_.emplace_back(ten_tile);
                    std::sort(temp.tiles_.begin(), temp.tiles_.end());
                    blocks_taken.emplace_back(temp);                  
                    res.tile_ = ten_tile;
                    res.blocks_ = blocks_taken;
                    this->ten_res_.emplace_back(res);
                    break;
                } else {
                    std::vector<block> blocks_taken_1 = blocks_taken;
                    std::vector<block> blocks_taken_2 = blocks_taken;
                    //听牌1
                    tile ten_tile_1(static_cast<TileType>(j+2));
                    block temp_1(BlockType::_SHUNTSU, TileTypeToBlockColor(static_cast<TileType>(j)), 3);
                    take_11(temp_1, j);
                    temp_1.tiles_.emplace_back(ten_tile_1);
                    std::sort(temp_1.tiles_.begin(), temp_1.tiles_.end());
                    blocks_taken_1.emplace_back(temp_1);                  
                    res.tile_ = ten_tile_1;
                    res.blocks_ = blocks_taken_1;
                    this->ten_res_.emplace_back(res);
                    //听牌2
                    tile ten_tile_2(static_cast<TileType>(j-1));
                    block temp_2(BlockType::_SHUNTSU, TileTypeToBlockColor(static_cast<TileType>(j)), 3);
                    take_11(temp_2, j);
                    temp_2.tiles_.emplace_back(ten_tile_2);
                    std::sort(temp_2.tiles_.begin(), temp_2.tiles_.end());
                    blocks_taken_2.emplace_back(temp_2);    
                    res.tile_ = ten_tile_2;
                    res.blocks_ = blocks_taken_2;
                    this->ten_res_.emplace_back(res);
                    break;
                }
            }
        }
    }
    if(ting_11){return true;}
    // [1_1]
    bool ting_1_1 = false;
    for(int i = 0; i < 27 && !ting_1_1; i += 9){
        for(int j = 9*i; j < 9*i+7 && !ting_1_1; ++j){
            if(mt[j] == 1 && mt[j+2] == 1){
                ting_1_1 = true;
                tile ten_tile(static_cast<TileType>(j+1));
                block temp(BlockType::_SHUNTSU, TileTypeToBlockColor(static_cast<TileType>(j)), 3);
                take_1_1(temp, j);
                temp.tiles_.emplace_back(ten_tile);
                std::sort(temp.tiles_.begin(), temp.tiles_.end());
                blocks_taken.emplace_back(temp);   
                res.tile_ = ten_tile;
                res.blocks_ = blocks_taken;
                this->ten_res_.emplace_back(res);
                break;
            }
        }
    }
    if(ting_1_1){return true;}

    return false;
}

void player::search_111_LtoR(std::vector<block>& blocks_taken, block_num& block_num, int *hand_tile){
    int *mt = hand_tile;
    const struct SearchConfig{
        BlockColor color;
        int init_pos;
        int back_pos;
    } search_config[3] = {{BlockColor::_MAN, 0, 8},{BlockColor::_PIN, 9, 17},{BlockColor::_SUO, 18, 26}};
    
    for(int s_i = 0; s_i < 3; ++s_i){
        for(int i1 = search_config[s_i].init_pos; i1+2 <= search_config[s_i].back_pos; ++i1){
            while(mt[i1] != 0 && mt[i1+1] != 0 && mt[i1+2] != 0){
                mt[i1]--;
                mt[i1+1]--;
                mt[i1+2]--;
                block_num.num_111++;
                block temp(BlockType::_SHUNTSU, search_config[s_i].color, 3);
                take_111(temp, i1);
                blocks_taken.emplace_back(temp);                  
            }
        }
    }   
}

void player::search_111_RtoL(std::vector<block>& blocks_taken, block_num& block_num, int *hand_tile){
    int *mt = hand_tile;
    const struct SearchConfig{
        BlockColor color;
        int init_pos;
        int back_pos;
    } search_config[3] = {{BlockColor::_MAN, 0, 8},{BlockColor::_PIN, 9, 17},{BlockColor::_SUO, 18, 26}};
    
    for(int s_i = 0; s_i < 3; ++s_i){
        for(int i2 = search_config[s_i].back_pos; i2-2 >= search_config[s_i].init_pos; --i2){
            while(mt[i2] != 0 && mt[i2-1] != 0 && mt[i2-2] != 0){
                mt[i2]--;
                mt[i2-1]--;
                mt[i2-2]--;
                block_num.num_111++;
                block temp(BlockType::_SHUNTSU, search_config[s_i].color, 3);
                take_111(temp, i2-2);
                blocks_taken.emplace_back(temp);                  
            }
        }
    }
}

void player::search_111_LandR(std::vector<block>& blocks_taken, block_num& block_num, int *hand_tile){
    int *mt = hand_tile;
    const struct SearchConfig{
        BlockColor color;
        int init_pos;
        int back_pos;
    } search_config[3] = {{BlockColor::_MAN, 0, 8},{BlockColor::_PIN, 9, 17},{BlockColor::_SUO, 18, 26}};

    for(int s_i = 0; s_i < 3; ++s_i){
        int i1 = search_config[s_i].init_pos;
        int i2 = search_config[s_i].back_pos;
        bool L_run = true;
        bool R_run = true;
        while(L_run || R_run){
            while(i1+2 <= search_config[s_i].back_pos){
                if(mt[i1] != 0 && mt[i1+1] != 0 && mt[i1+2] != 0){
                    mt[i1]--;
                    mt[i1+1]--;
                    mt[i1+2]--;
                    block_num.num_111++;
                    block temp(BlockType::_SHUNTSU, search_config[s_i].color, 3);
                    take_111(temp, i1);
                    blocks_taken.emplace_back(temp);
                    break;                  
                } else {i1++;}
            }
            if(i1+2 > search_config[s_i].back_pos){L_run = false;}
            while(i2-2 >= search_config[s_i].init_pos){
                if(mt[i2] != 0 && mt[i2-1] != 0 && mt[i2-2] != 0){
                    mt[i2]--;
                    mt[i2-1]--;
                    mt[i2-2]--;
                    block_num.num_111++;
                    block temp(BlockType::_SHUNTSU, search_config[s_i].color, 3);
                    take_111(temp, i2-2);
                    blocks_taken.emplace_back(temp);
                    break;                  
                } else {i2--;}
            }
            if(i2-2 < search_config[s_i].init_pos){R_run = false;}
        }
    }   
}

void player::search_111_RandL(std::vector<block>& blocks_taken, block_num& block_num, int *hand_tile){
    int *mt = hand_tile;
    const struct SearchConfig{
        BlockColor color;
        int init_pos;
        int back_pos;
    } search_config[3] = {{BlockColor::_MAN, 0, 8},{BlockColor::_PIN, 9, 17},{BlockColor::_SUO, 18, 26}};

    for(int s_i = 0; s_i < 3; ++s_i){
        int i1 = search_config[s_i].init_pos;
        int i2 = search_config[s_i].back_pos;
        bool L_run = true;
        bool R_run = true;
        while(L_run || R_run){
            while(i2-2 >= search_config[s_i].init_pos){
                if(mt[i2] != 0 && mt[i2-1] != 0 && mt[i2-2] != 0){
                    mt[i2]--;
                    mt[i2-1]--;
                    mt[i2-2]--;
                    block_num.num_111++;
                    block temp(BlockType::_SHUNTSU, search_config[s_i].color, 3);
                    take_111(temp, i2-2);
                    blocks_taken.emplace_back(temp);
                    break;                  
                } else {i2--;}
            }
            if(i2-2 < search_config[s_i].init_pos){R_run = false;}
            while(i1+2 <= search_config[s_i].back_pos){
                if(mt[i1] != 0 && mt[i1+1] != 0 && mt[i1+2] != 0){
                    mt[i1]--;
                    mt[i1+1]--;
                    mt[i1+2]--;
                    block_num.num_111++;
                    block temp(BlockType::_SHUNTSU, search_config[s_i].color, 3);
                    take_111(temp, i1);
                    blocks_taken.emplace_back(temp);
                    break;                  
                } else {i1++;}
            }
            if(i1+2 > search_config[s_i].back_pos){L_run = false;}
        }
    }   
}

void player::search_3_all(std::vector<block>& blocks_taken, block_num& block_num, int *hand_tile){
    int *mt = hand_tile;
    for(int i = 0; i < 34; ++i){
        if(mt[i] >= 3){
            block temp(BlockType::_KOUTSU, TileTypeToBlockColor(static_cast<TileType>(i)), 3);
            take_3(temp, i);
            blocks_taken.emplace_back(temp);
            block_num.num_3++;
            mt[i] -= 3;
        }
    }
}

void player::search_2_aim(std::vector<block>& blocks_taken, block_num& block_num, int *hand_tile, int pos){
    int *mt = hand_tile;
    if(mt[pos] >= 2){
        block temp(BlockType::_TOITSU, TileTypeToBlockColor(static_cast<TileType>(pos)), 2);
        take_2(temp, pos);
        blocks_taken.emplace_back(temp);
        block_num.num_2++;
        mt[pos] -= 2;
    }
}

void player::take_111(block& aim_block, int t1){
    aim_block.tiles_.clear();
    bool found_t1 = false;
    bool found_t2 = false;
    bool found_t3 = false;

    for(int i = 0; i < this->hand_tiles_.size(); ++i){
        int current_type = static_cast<int>(hand_tiles_[i].type_);

        if(!found_t1 && current_type == t1){
            aim_block.tiles_.emplace_back(hand_tiles_[i]);
            found_t1 = true;
            continue;
        }
        if(!found_t2 && current_type == t1+1){
            aim_block.tiles_.emplace_back(hand_tiles_[i]);
            found_t2 = true;
            continue;
        }
        if(!found_t3 && current_type == t1+2){
            aim_block.tiles_.emplace_back(hand_tiles_[i]);
            found_t3 = true;
            continue;
        }
    }
}

void player::take_3(block& aim_block, int t1){
    aim_block.tiles_.clear();
    int found_t1 = 0;
    for(int i = 0; i < this->hand_tiles_.size() && found_t1 < 3; ++i){
        int current_type = static_cast<int>(hand_tiles_[i].type_);
        if(current_type == t1){
            aim_block.tiles_.emplace_back(hand_tiles_[i]);
            found_t1++;
        }
    }
}

void player::take_2(block& aim_block, int t1){
    aim_block.tiles_.clear();
    int found_t1 = 0;
    for(int i = 0; i < this->hand_tiles_.size() && found_t1 < 2; ++i){
        int current_type = static_cast<int>(hand_tiles_[i].type_);
        if(current_type == t1){
            aim_block.tiles_.emplace_back(hand_tiles_[i]);
            found_t1++;
        }
    }
}

void player::take_1(block& aim_block, int t1){
    aim_block.tiles_.clear();
    for(int i = 0; i < this->hand_tiles_.size(); ++i){
        int current_type = static_cast<int>(hand_tiles_[i].type_);
        if(current_type == t1){
            aim_block.tiles_.emplace_back(hand_tiles_[i]);
            break;
        }
    }
}

void player::take_11(block& aim_block, int t1){
    aim_block.tiles_.clear();
    bool found_t1 = false;
    bool found_t2 = false;

    for(int i = 0; i < this->hand_tiles_.size(); ++i){
        int current_type = static_cast<int>(hand_tiles_[i].type_);

        if(!found_t1 && current_type == t1){
            aim_block.tiles_.emplace_back(hand_tiles_[i]);
            found_t1 = true;
            continue;
        }
        if(!found_t2 && current_type == t1+1){
            aim_block.tiles_.emplace_back(hand_tiles_[i]);
            found_t2 = true;
            continue;
        }
    }
}

void player::take_1_1(block& aim_block, int t1){
    aim_block.tiles_.clear();
    bool found_t1 = false;
    bool found_t3 = false;

    for(int i = 0; i < this->hand_tiles_.size(); ++i){
        int current_type = static_cast<int>(hand_tiles_[i].type_);

        if(!found_t1 && current_type == t1){
            aim_block.tiles_.emplace_back(hand_tiles_[i]);
            found_t1 = true;
            continue;
        }
        if(!found_t3 && current_type == t1+2){
            aim_block.tiles_.emplace_back(hand_tiles_[i]);
            found_t3 = true;
            continue;
        }
    }
}

void player::receive_tile(RequestType res_type){
    std::vector<char> send_info;
    send_info.emplace_back(res_type);
    client_.Send(std::span<const char>(send_info.data(), send_info.size()));
    auto response = client_.Receive();
    tile tile_t(static_cast<TileType>(response[0]));
    if(static_cast<int>(response[1]) != 0){
        tile_t.set_red(true);
    }
    if(res_type == RequestType::_MO){
        tile_t.state_ = TileState::_TSUMO;
    }
    if(res_type == RequestType::_KAN){
        tile_t.state_ = TileState::_KAN;
    }
    next_tile_ = tile_t;
}


