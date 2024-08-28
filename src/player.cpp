#include"player.h"

void Player::hand_to_block(){

    if(!ten_.empty()){ten_.clear();}
    if(!hu_.empty()){hu_.clear();}
    if(!shanten_.empty()){shanten_.clear();}


    //先将手牌转化为相关牌的数量
    int handnum_[34]{0};
    for(int i = 0; i < hand_.size(); ++i){
        int t = static_cast<int>(hand_[i].type_);
        handnum_[t]++;
    }

    Blocknum hand_block_num_;
    //根据吃，碰，杠设定hand_block_num_
    if(!fixed_blocks_.empty()){
        for(int i = 0; i< fixed_blocks_.size(); ++i){
            if(fixed_blocks_[i].is_hand_ == false){
                switch(static_cast<int>(fixed_blocks_[i].type_)){
                    case 1:
                        hand_block_num_.num_111++;break;
                    case 2:case 3:
                        hand_block_num_.num_3++;break;
                    default:
                        break;
                }
            }
        }
    }


    //特殊牌型
    //七对子
    is_chitoitsu(handnum_);
    
    //国士无双
    is_kokushimusou(handnum_);


    //一般胡
    is_ten(hand_block_num_, handnum_);



    //根据hu_算分
    if(!hu_.empty()){
        for(int i = 0; i < hu_.size(); ++i){
            //Todo
        }
    }




    //测试打印
    std::sort(hu_.begin(), hu_.end());

    print_hand_();
    print_ten_();
    print_hu_();

}


void Player::is_ten(Blocknum& hand_block_num_, int* handnum_){
    /*
    * 缺[2]时的听牌型：
    * 1.    先取[111] (1.LtoR   2.RtoL  3.LandR 4. RandL)，再取[3]
    * 2.    先取所有[3]，再取[111](1.LtoR   2.RtoL  3.LandR 4. RandL)
    * 
    */

    std::vector<Block> block_list_;
    Blocknum temp_block_num_;
    int m_[34]{0};

    //手牌数量备份
    temp_block_num_ = hand_block_num_;
    for(int i = 0; i < 34; ++i){
        m_[i] = handnum_[i];
    }
    //1.先取[111] (LtoR)，再取[3]
    search_111_LtoR(block_list_, temp_block_num_, m_);
    search_3_all(block_list_, temp_block_num_, m_);
    hu_type_1(block_list_, temp_block_num_, m_);
    if(!block_list_.empty()){block_list_.clear();}


    //2.先取[111] (RtoL)，再取[3]
    temp_block_num_ = hand_block_num_;
    for(int i = 0; i < 34; ++i){
        m_[i] = handnum_[i];
    }
    search_111_RtoL(block_list_, temp_block_num_, m_);
    search_3_all(block_list_, temp_block_num_, m_);
    hu_type_1(block_list_, temp_block_num_, m_);
    if(!block_list_.empty()){block_list_.clear();}

    //3.先左取[111]再右取[111] (LandR)，最后取[3]
    temp_block_num_ = hand_block_num_;
    for(int i = 0; i < 34; ++i){
        m_[i] = handnum_[i];
    }
    search_111_LandR(block_list_, temp_block_num_, m_);
    search_3_all(block_list_, temp_block_num_, m_);
    hu_type_1(block_list_, temp_block_num_, m_);
    if(!block_list_.empty()){block_list_.clear();}

    //4.先右取[111]再左取[111] (RandL)，最后取[3]
    temp_block_num_ = hand_block_num_;
    for(int i = 0; i < 34; ++i){
        m_[i] = handnum_[i];
    }
    search_111_RandL(block_list_, temp_block_num_, m_);
    search_3_all(block_list_, temp_block_num_, m_);
    hu_type_1(block_list_, temp_block_num_, m_);
    if(!block_list_.empty()){block_list_.clear();}

    //5.先取[3]，再从左往右取[111] (LtoR)
    temp_block_num_ = hand_block_num_;
    for(int i = 0; i < 34; ++i){
        m_[i] = handnum_[i];
    }
    search_3_all(block_list_, temp_block_num_, m_);
    search_111_LtoR(block_list_, temp_block_num_, m_);
    hu_type_1(block_list_, temp_block_num_, m_);
    if(!block_list_.empty()){block_list_.clear();}

    //6.先取[3]，再从右往左取[111] (RtoL)
    temp_block_num_ = hand_block_num_;
    for(int i = 0; i < 34; ++i){
        m_[i] = handnum_[i];
    }
    search_3_all(block_list_, temp_block_num_, m_);
    search_111_RtoL(block_list_, temp_block_num_, m_);
    hu_type_1(block_list_, temp_block_num_, m_);
    if(!block_list_.empty()){block_list_.clear();}

    //7.先取[3]，再先左后右取[111] (LandR)
    temp_block_num_ = hand_block_num_;
    for(int i = 0; i < 34; ++i){
        m_[i] = handnum_[i];
    }
    search_3_all(block_list_, temp_block_num_, m_);
    search_111_LandR(block_list_, temp_block_num_, m_);
    hu_type_1(block_list_, temp_block_num_, m_);
    if(!block_list_.empty()){block_list_.clear();}

    //8.先取[3]，再先右后左取[111] (RandL)
    temp_block_num_ = hand_block_num_;
    for(int i = 0; i < 34; ++i){
        m_[i] = handnum_[i];
    }
    search_3_all(block_list_, temp_block_num_, m_);
    search_111_RandL(block_list_, temp_block_num_, m_);
    hu_type_1(block_list_, temp_block_num_, m_);
    if(!block_list_.empty()){block_list_.clear();}


    /*
    * 有[2], 但[111]不完整时的听牌型
    * 1.    先取[2], 再取[111] (1.LtoR   2.RtoL  3.LandR 4. RandL)，再取[3]
    * 2.    先取[2], 再取[3]，再取[111](1.LtoR   2.RtoL  3.LandR 4. RandL)
    */

    //先取[2]
    std::vector<int> pos_2_;     //[2]的位置下标
    for(int i = 0; i < 34; ++i){
        if(handnum_[i] >= 2){pos_2_.emplace_back(i);}
    }
    //遍历每个[2]的位置
    for( int p = 0; p < pos_2_.size(); ++p){

        //1. [2], [111] (LtoR), [3]
        temp_block_num_ = hand_block_num_;
        for(int i = 0; i < 34; ++i){
            m_[i] = handnum_[i];
        }
        search_2_aim(block_list_, temp_block_num_, m_, pos_2_[p]);
        search_111_LtoR(block_list_, temp_block_num_, m_);
        search_3_all(block_list_, temp_block_num_, m_);
        hu_type_2(block_list_, temp_block_num_, m_);
        if(!block_list_.empty()){block_list_.clear();}

        //2. [2], [111] (RtoL), [3]
        temp_block_num_ = hand_block_num_;
        for(int i = 0; i < 34; ++i){
            m_[i] = handnum_[i];
        }
        search_2_aim(block_list_, temp_block_num_, m_, pos_2_[p]);
        search_111_RtoL(block_list_, temp_block_num_, m_);
        search_3_all(block_list_, temp_block_num_, m_);
        hu_type_2(block_list_, temp_block_num_, m_);
        if(!block_list_.empty()){block_list_.clear();}
   
        //3. [2], [111] (LandR), [3]
        temp_block_num_ = hand_block_num_;
        for(int i = 0; i < 34; ++i){
            m_[i] = handnum_[i];
        }
        search_2_aim(block_list_, temp_block_num_, m_, pos_2_[p]);
        search_111_LandR(block_list_, temp_block_num_, m_);
        search_3_all(block_list_, temp_block_num_, m_);
        hu_type_2(block_list_, temp_block_num_, m_);
        if(!block_list_.empty()){block_list_.clear();}

        //4. [2], [111] (RandL), [3]
        temp_block_num_ = hand_block_num_;
        for(int i = 0; i < 34; ++i){
            m_[i] = handnum_[i];
        }
        search_2_aim(block_list_, temp_block_num_, m_, pos_2_[p]);
        search_111_RandL(block_list_, temp_block_num_, m_);
        search_3_all(block_list_, temp_block_num_, m_);
        hu_type_2(block_list_, temp_block_num_, m_);
        if(!block_list_.empty()){block_list_.clear();}

        //5. [2], [3], [111] (LtoR)
        temp_block_num_ = hand_block_num_;
        for(int i = 0; i < 34; ++i){
            m_[i] = handnum_[i];
        }
        search_2_aim(block_list_, temp_block_num_, m_, pos_2_[p]);
        search_3_all(block_list_, temp_block_num_, m_);
        search_111_LtoR(block_list_, temp_block_num_, m_);
        hu_type_2(block_list_, temp_block_num_, m_);
        if(!block_list_.empty()){block_list_.clear();}

        //6. [2], [3], [111] (RtoL)
        temp_block_num_ = hand_block_num_;
        for(int i = 0; i < 34; ++i){
            m_[i] = handnum_[i];
        }
        search_2_aim(block_list_, temp_block_num_, m_, pos_2_[p]);
        search_3_all(block_list_, temp_block_num_, m_);
        search_111_RtoL(block_list_, temp_block_num_, m_);
        hu_type_2(block_list_, temp_block_num_, m_);
        if(!block_list_.empty()){block_list_.clear();}

        //7. [2], [3], [111] (LandR)
        temp_block_num_ = hand_block_num_;
        for(int i = 0; i < 34; ++i){
            m_[i] = handnum_[i];
        }
        search_2_aim(block_list_, temp_block_num_, m_, pos_2_[p]);
        search_3_all(block_list_, temp_block_num_, m_);
        search_111_LandR(block_list_, temp_block_num_, m_);
        hu_type_2(block_list_, temp_block_num_, m_);
        if(!block_list_.empty()){block_list_.clear();}

        //8. [2], [3], [111] (RandL)
        temp_block_num_ = hand_block_num_;
        for(int i = 0; i < 34; ++i){
            m_[i] = handnum_[i];
        }
        search_2_aim(block_list_, temp_block_num_, m_, pos_2_[p]);
        search_3_all(block_list_, temp_block_num_, m_);
        search_111_RandL(block_list_, temp_block_num_, m_);
        hu_type_2(block_list_, temp_block_num_, m_);
        if(!block_list_.empty()){block_list_.clear();}
    }
}






int Player::cal_yaku(Hu &_cal_hu)
{
    int _fan = 0;
    int _f = 0;

    _f = is_riichi();
    if(_f != 0){
        _fan += _f;
        if(_f == 1){_cal_hu.yaku_.emplace_back(YakuType::_RIICHI);}
        if(_f == 2){_cal_hu.yaku_.emplace_back(YakuType::_DOUBLERIICHI);}
    }
    _f = is_tanyao();
    if(_f != 0){
        _fan += _f;
        _cal_hu.yaku_.emplace_back(YakuType::_TANYAO);
    }
    _f = is_menzentsumo();
    if(_f != 0){
        _fan += _f;
        _cal_hu.yaku_.emplace_back(YakuType::_MENZENTSUMO);
    }



    _f = is_yakuhai_sangen(_cal_hu);
    if(_f != 0){
        _fan += _f;
        _cal_hu.yaku_.emplace_back(YakuType::_YAKUHAISANGEN);
    }


    
    _f = is_rinshankaihou();
    if(_f != 0){
        _fan += _f;
        _cal_hu.yaku_.emplace_back(YakuType::_RINSHANKAIHOU);
    }

    
    
    _f = is_chantaiyao();
    if(_f != 0){
        _fan += _f;
        _cal_hu.yaku_.emplace_back(YakuType::_CHANTAIYAO);
    }
    


    _f = is_junchantaiyao();
    if(_f != 0){
        _fan += _f;
        _cal_hu.yaku_.emplace_back(YakuType::_JUNCHANTAIYAO);
    }

    
    
    
    _f = is_ryuuiisou();
    if(_f == 13){
        _fan += _f;
        _cal_hu.yaku_.emplace_back(YakuType::_RYUUIISOU);
    }
    
    
    
    
    
    
    
    
    _cal_hu.fan_ = _fan;

    return 0;
}

void Player::search_111_LtoR(std::vector<Block> &block_temp_, Blocknum &block_num_, int *m_){
    //1. 取万
    for(int i = 0; i < 7; ++i){
        if(m_[i] != 0){
            if(i == 0){
                if(m_[i+1]>=1 && m_[i+2]>=1){
                    m_[i]--;
                    m_[i+1]--;
                    m_[i+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3,true);
                    take_111(temp, i);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i == 1){
                if(m_[i-1] >=1 && m_[i+1] >=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i-1);
                    block_temp_.emplace_back(temp);
                } else if(m_[i+1] >=1 && m_[i+2] >=1){
                    m_[i]--;
                    m_[i+1]--;
                    m_[i+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i);
                    block_temp_.emplace_back(temp);
                }
            } 
            else if( i > 1){
                if(m_[i-1] >=1 && m_[i+1] >=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i-1);
                    block_temp_.emplace_back(temp);
                } else if(m_[i-1] >=1 && m_[i-2] >=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i-2);
                    block_temp_.emplace_back(temp);
                } else if(m_[i+1] >=1 && m_[i+2] >=1){
                    m_[i]--;
                    m_[i+1]--;
                    m_[i+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i);
                    block_temp_.emplace_back(temp);
                }
                //可能有四个相同的[111], 多加个if取第4个
                if(m_[i] != 0){
                    if(m_[i-1] >=1 && m_[i-2] >=1){
                        m_[i]--;
                        m_[i-1]--;
                        m_[i-2]--;
                        block_num_.num_111++;
                        Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                        take_111(temp, i-2);
                        block_temp_.emplace_back(temp);
                    }
                }
            }
        }
    }
    //2. 取筒
    for(int i = 9; i < 16; ++i){
        if(m_[i] != 0){
            if(i == 9){
                if(m_[i+1]>=1 && m_[i+2]>=1){
                    m_[i]--;
                    m_[i+1]--;
                    m_[i+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3,true);
                    take_111(temp, i);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i == 10){
                if(m_[i-1] >=1 && m_[i+1] >=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i-1);
                    block_temp_.emplace_back(temp);
                } else if(m_[i+1] >=1 && m_[i+2] >=1){
                    m_[i]--;
                    m_[i+1]--;
                    m_[i+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i);
                    block_temp_.emplace_back(temp);
                }
            } 
            else if( i > 10){
                if(m_[i-1] >=1 && m_[i+1] >=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i-1);
                    block_temp_.emplace_back(temp);
                } else if(m_[i-1] >=1 && m_[i-2] >=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i-2);
                    block_temp_.emplace_back(temp);
                } else if(m_[i+1] >=1 && m_[i+2] >=1){
                    m_[i]--;
                    m_[i+1]--;
                    m_[i+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i);
                    block_temp_.emplace_back(temp);
                }
                if(m_[i] != 0){
                    if(m_[i-1] >=1 && m_[i-2] >=1){
                        m_[i]--;
                        m_[i-1]--;
                        m_[i-2]--;
                        block_num_.num_111++;
                        Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                        take_111(temp, i-2);
                        block_temp_.emplace_back(temp);
                    }
                }
            }
        }
    }
    //3. 取条
    for(int i = 18; i < 25; ++i){
        if(m_[i] != 0){
            if(i == 18){
                if(m_[i+1]>=1 && m_[i+2]>=1){
                    m_[i]--;
                    m_[i+1]--;
                    m_[i+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3,true);
                    take_111(temp, i);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i == 19){
                if(m_[i-1] >=1 && m_[i+1] >=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i-1);
                    block_temp_.emplace_back(temp);
                } else if(m_[i+1] >=1 && m_[i+2] >=1){
                    m_[i]--;
                    m_[i+1]--;
                    m_[i+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i);
                    block_temp_.emplace_back(temp);
                }
            } 
            else if( i > 19){
                if(m_[i-1] >=1 && m_[i+1] >=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i-1);
                    block_temp_.emplace_back(temp);
                } else if(m_[i-1] >=1 && m_[i-2] >=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i-2);
                    block_temp_.emplace_back(temp);
                } else if(m_[i+1] >=1 && m_[i+2] >=1){
                    m_[i]--;
                    m_[i+1]--;
                    m_[i+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i);
                    block_temp_.emplace_back(temp);
                }
                if(m_[i] != 0){
                    if(m_[i-1] >=1 && m_[i-2] >=1){
                        m_[i]--;
                        m_[i-1]--;
                        m_[i-2]--;
                        block_num_.num_111++;
                        Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                        take_111(temp, i-2);
                        block_temp_.emplace_back(temp);
                    }
                }
            }
        }
    }
}

void Player::search_111_RtoL(std::vector<Block> &block_temp_, Blocknum &block_num_, int *m_)
{
    //1. 取万
    for(int i = 8; i > 1; --i){
        if(m_[i] != 0){
            if(i == 8){
                if(m_[i-1]>=1 && m_[i-2]>=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3,true);
                    take_111(temp, i-2);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i == 7){
                if(m_[i-1] >=1 && m_[i+1] >=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i-1);
                    block_temp_.emplace_back(temp);
                } else if(m_[i-1] >=1 && m_[i-2] >=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i-2);
                    block_temp_.emplace_back(temp);
                }
            } 
            else if( i < 7){
                if(m_[i-1] >=1 && m_[i+1] >=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i-1);
                    block_temp_.emplace_back(temp);
                } else if(m_[i-1] >=1 && m_[i-2] >=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i-2);
                    block_temp_.emplace_back(temp);
                } else if(m_[i+1] >=1 && m_[i+2] >=1){
                    m_[i]--;
                    m_[i+1]--;
                    m_[i+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i);
                    block_temp_.emplace_back(temp);
                }
                //可能有四个相同的[111], 多加个if取第4个
                if(m_[i] != 0){
                    if(m_[i+1] >=1 && m_[i+2] >=1){
                        m_[i]--;
                        m_[i+1]--;
                        m_[i+2]--;
                        block_num_.num_111++;
                        Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                        take_111(temp, i);
                        block_temp_.emplace_back(temp);
                    }
                }
            }
        }
    }
    //2. 取筒
    for(int i = 17; i > 10; --i){
        if(m_[i] != 0){
            if(i == 17){
                if(m_[i-1]>=1 && m_[i-2]>=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3,true);
                    take_111(temp, i-2);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i == 16){
                if(m_[i-1] >=1 && m_[i+1] >=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i-1);
                    block_temp_.emplace_back(temp);
                } else if(m_[i-1] >=1 && m_[i-2] >=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i-2);
                    block_temp_.emplace_back(temp);
                }
            } 
            else if( i < 16){
                if(m_[i-1] >=1 && m_[i+1] >=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i-1);
                    block_temp_.emplace_back(temp);
                } else if(m_[i-1] >=1 && m_[i-2] >=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i-2);
                    block_temp_.emplace_back(temp);
                } else if(m_[i+1] >=1 && m_[i+2] >=1){
                    m_[i]--;
                    m_[i+1]--;
                    m_[i+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i);
                    block_temp_.emplace_back(temp);
                }
                if(m_[i] != 0){
                    if(m_[i+1] >=1 && m_[i+2] >=1){
                        m_[i]--;
                        m_[i+1]--;
                        m_[i+2]--;
                        block_num_.num_111++;
                        Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                        take_111(temp, i);
                        block_temp_.emplace_back(temp);
                    }
                }
            }
        }
    }
    //3. 取条
    for(int i = 26; i > 19; --i){
        if(m_[i] != 0){
            if(i == 26){
                if(m_[i-1]>=1 && m_[i-2]>=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3,true);
                    take_111(temp, i-2);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i == 25){
                if(m_[i-1] >=1 && m_[i+1] >=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i-1);
                    block_temp_.emplace_back(temp);
                } else if(m_[i-1] >=1 && m_[i-2] >=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i-2);
                    block_temp_.emplace_back(temp);
                }
            } 
            else if( i < 25){
                if(m_[i-1] >=1 && m_[i+1] >=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i-1);
                    block_temp_.emplace_back(temp);
                } else if(m_[i-1] >=1 && m_[i-2] >=1){
                    m_[i]--;
                    m_[i-1]--;
                    m_[i-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i-2);
                    block_temp_.emplace_back(temp);
                } else if(m_[i+1] >=1 && m_[i+2] >=1){
                    m_[i]--;
                    m_[i+1]--;
                    m_[i+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i);
                    block_temp_.emplace_back(temp);
                }
                if(m_[i] != 0){
                    if(m_[i+1] >=1 && m_[i+2] >=1){
                        m_[i]--;
                        m_[i+1]--;
                        m_[i+2]--;
                        block_num_.num_111++;
                        Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                        take_111(temp, i);
                        block_temp_.emplace_back(temp);
                    }
                }
            }
        }
    }
}

void Player::search_111_LandR(std::vector<Block> &block_temp_, Blocknum &block_num_, int *m_){
    //1. 取万
    for(int i1 = 0, i2 = 8; i1 < 7 && i2 > 1; ++i1, --i2){
        //取左
        if(m_[i1] != 0){
            if(i1 == 0){
                if(m_[i1+1] >= 1 && m_[i1+2] >= 1){
                    m_[i1]--;
                    m_[i1+1]--;
                    m_[i1+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3,true);
                    take_111(temp, i1);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i1 == 1){
                if(m_[i1-1] >= 1 && m_[i1+1] >= 1){
                    m_[i1]--;
                    m_[i1-1]--;
                    m_[i1+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i1-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i1+1] >= 1 && m_[i1+2] >= 1){
                    m_[i1]--;
                    m_[i1+1]--;
                    m_[i1+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i1);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i1 > 1){
                if(m_[i1-1] >= 1 && m_[i1+1] >= 1){
                    m_[i1]--;
                    m_[i1-1]--;
                    m_[i1+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i1-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i1+1] >= 1 && m_[i1+2] >= 1){
                    m_[i1]--;
                    m_[i1+1]--;
                    m_[i1+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i1-1] >= 1 && m_[i1-2] >= 1){
                    m_[i1]--;
                    m_[i1-1]--;
                    m_[i1-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i1-2);
                    block_temp_.emplace_back(temp);
                }
                if(m_[i1] != 0){
                    if(m_[i1-1] >= 1 && m_[i1-2] >= 1){
                        m_[i1]--;
                        m_[i1-1]--;
                        m_[i1-2]--;
                        block_num_.num_111++;
                        Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                        take_111(temp, i1-2);
                        block_temp_.emplace_back(temp);
                    }
                }
            }
        }
        //取右
        if(m_[i2] != 0){
            if(i2 == 8){
                if(m_[i2-1] >= 1 && m_[i2-2] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3,true);
                    take_111(temp, i2-2);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i2 == 7){
                if(m_[i2-1] >= 1 && m_[i2+1] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i2-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i2-1] >= 1 && m_[i2-2] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i2-2);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i2 < 7){
                if(m_[i2-1] >= 1 && m_[i2+1] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i2-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i2-1] >= 1 && m_[i2-2] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i2-2);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i2+1] >= 1 && m_[i2+2] >= 1){
                    m_[i2]--;
                    m_[i2+1]--;
                    m_[i2+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i2);
                    block_temp_.emplace_back(temp);
                }
                if(m_[i2] != 0){
                    if(m_[i2+1] >= 1 && m_[i2+2] >= 1){
                        m_[i2]--;
                        m_[i2+1]--;
                        m_[i2+2]--;
                        block_num_.num_111++;
                        Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                        take_111(temp, i2);
                        block_temp_.emplace_back(temp);
                    }
                }
            }
        }
    }

    //取筒
    for(int i1 = 9, i2 = 17; i1 < 16 && i2 > 10; ++i1, --i2){
        //取左
        if(m_[i1] != 0){
            if(i1 == 9){
                if(m_[i1+1] >= 1 && m_[i1+2] >= 1){
                    m_[i1]--;
                    m_[i1+1]--;
                    m_[i1+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3,true);
                    take_111(temp, i1);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i1 == 10){
                if(m_[i1-1] >= 1 && m_[i1+1] >= 1){
                    m_[i1]--;
                    m_[i1-1]--;
                    m_[i1+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i1-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i1+1] >= 1 && m_[i1+2] >= 1){
                    m_[i1]--;
                    m_[i1+1]--;
                    m_[i1+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i1);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i1 > 10){
                if(m_[i1-1] >= 1 && m_[i1+1] >= 1){
                    m_[i1]--;
                    m_[i1-1]--;
                    m_[i1+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i1-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i1+1] >= 1 && m_[i1+2] >= 1){
                    m_[i1]--;
                    m_[i1+1]--;
                    m_[i1+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i1-1] >= 1 && m_[i1-2] >= 1){
                    m_[i1]--;
                    m_[i1-1]--;
                    m_[i1-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i1-2);
                    block_temp_.emplace_back(temp);
                }
                if(m_[i1] != 0){
                    if(m_[i1-1] >= 1 && m_[i1-2] >= 1){
                        m_[i1]--;
                        m_[i1-1]--;
                        m_[i1-2]--;
                        block_num_.num_111++;
                        Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                        take_111(temp, i1-2);
                        block_temp_.emplace_back(temp);
                    }
                }
            }
        }
        //取右
        if(m_[i2] != 0){
            if(i2 == 17){
                if(m_[i2-1] >= 1 && m_[i2-2] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3,true);
                    take_111(temp, i2-2);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i2 == 16){
                if(m_[i2-1] >= 1 && m_[i2+1] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i2-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i2-1] >= 1 && m_[i2-2] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i2-2);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i2 < 16){
                if(m_[i2-1] >= 1 && m_[i2+1] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i2-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i2-1] >= 1 && m_[i2-2] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i2-2);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i2+1] >= 1 && m_[i2+2] >= 1){
                    m_[i2]--;
                    m_[i2+1]--;
                    m_[i2+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);  
                    take_111(temp, i2);
                    block_temp_.emplace_back(temp);
                }
                if(m_[i2] != 0){
                    if(m_[i2+1] >= 1 && m_[i2+2] >= 1){
                        m_[i2]--;
                        m_[i2+1]--;
                        m_[i2+2]--;
                        block_num_.num_111++;
                        Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                        take_111(temp, i2);
                        block_temp_.emplace_back(temp);
                    }
                }
            }
        }
    }

    //取条
    for(int i1 = 18, i2 = 26; i1 < 25 && i2 > 19; ++i1, --i2){
        //取左
        if(m_[i1] != 0){
            if(i1 == 18){
                if(m_[i1+1] >= 1 && m_[i1+2] >= 1){
                    m_[i1]--;
                    m_[i1+1]--;
                    m_[i1+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3,true);
                    take_111(temp, i1);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i1 == 19){
                if(m_[i1-1] >= 1 && m_[i1+1] >= 1){
                    m_[i1]--;
                    m_[i1-1]--;
                    m_[i1+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i1-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i1+1] >= 1 && m_[i1+2] >= 1){
                    m_[i1]--;
                    m_[i1+1]--;
                    m_[i1+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i1);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i1 > 19){
                if(m_[i1-1] >= 1 && m_[i1+1] >= 1){
                    m_[i1]--;
                    m_[i1-1]--;
                    m_[i1+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i1-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i1+1] >= 1 && m_[i1+2] >= 1){
                    m_[i1]--;
                    m_[i1+1]--;
                    m_[i1+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i1-1] >= 1 && m_[i1-2] >= 1){
                    m_[i1]--;
                    m_[i1-1]--;
                    m_[i1-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i1-2);
                    block_temp_.emplace_back(temp);
                }
                if(m_[i1] != 0){
                    if(m_[i1-1] >= 1 && m_[i1-2] >= 1){
                        m_[i1]--;
                        m_[i1-1]--;
                        m_[i1-2]--;
                        block_num_.num_111++;
                        Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                        take_111(temp, i1-2);
                        block_temp_.emplace_back(temp);
                    }
                }
            }
        }
        //取右
        if(m_[i2] != 0){
            if(i2 == 26){
                if(m_[i2-1] >= 1 && m_[i2-2] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3,true);
                    take_111(temp, i2-2);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i2 == 25){
                if(m_[i2-1] >= 1 && m_[i2+1] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i2-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i2-1] >= 1 && m_[i2-2] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i2-2);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i2 < 25){
                if(m_[i2-1] >= 1 && m_[i2+1] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i2-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i2-1] >= 1 && m_[i2-2] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i2-2);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i2+1] >= 1 && m_[i2+2] >= 1){
                    m_[i2]--;
                    m_[i2+1]--;
                    m_[i2+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i2);
                    block_temp_.emplace_back(temp);
                }
                if(m_[i2] != 0){
                    if(m_[i2+1] >= 1 && m_[i2+2] >= 1){
                        m_[i2]--;
                        m_[i2+1]--;
                        m_[i2+2]--;
                        block_num_.num_111++;
                        Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                        take_111(temp, i2);
                        block_temp_.emplace_back(temp);
                    }
                }
            }
        }
    }
}

void Player::search_111_RandL(std::vector<Block> &block_temp_, Blocknum &block_num_, int *m_){
    //取万牌
    for(int i1 = 0, i2 = 8; i1 < 7 && i2 > 1; ++i1, --i2){
        //取右
        if(m_[i2] != 0){
            if(i2 == 8){
                if(m_[i2-1] >= 1 && m_[i2-2] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3,true);
                    take_111(temp, i2-2);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i2 == 7){
                if(m_[i2-1] >= 1 && m_[i2+1] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i2-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i2-1] >= 1 && m_[i2-2] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i2-2);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i2 < 7){
                if(m_[i2-1] >= 1 && m_[i2+1] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i2-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i2-1] >= 1 && m_[i2-2] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i2-2);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i2+1] >= 1 && m_[i2+2] >= 1){
                    m_[i2]--;
                    m_[i2+1]--;
                    m_[i2+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i2);
                    block_temp_.emplace_back(temp);
                }
                if(m_[i2] != 0){
                    if(m_[i2+1] >= 1 && m_[i2+2] >= 1){
                        m_[i2]--;
                        m_[i2+1]--;
                        m_[i2+2]--;
                        block_num_.num_111++;
                        Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                        take_111(temp, i2);
                        block_temp_.emplace_back(temp);
                    }
                }
            }
        }
        //取左
        if(m_[i1] != 0){
            if(i1 == 0){
                if(m_[i1+1] >= 1 && m_[i1+2] >= 1){
                    m_[i1]--;
                    m_[i1+1]--;
                    m_[i1+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3,true);
                    take_111(temp, i1);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i1 == 1){
                if(m_[i1-1] >= 1 && m_[i1+1] >= 1){
                    m_[i1]--;
                    m_[i1-1]--;
                    m_[i1+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i1-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i1+1] >= 1 && m_[i1+2] >= 1){
                    m_[i1]--;
                    m_[i1+1]--;
                    m_[i1+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i1);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i1 > 1){
                if(m_[i1-1] >= 1 && m_[i1+1] >= 1){
                    m_[i1]--;
                    m_[i1-1]--;
                    m_[i1+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i1-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i1+1] >= 1 && m_[i1+2] >= 1){
                    m_[i1]--;
                    m_[i1+1]--;
                    m_[i1+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i1-1] >= 1 && m_[i1-2] >= 1){
                    m_[i1]--;
                    m_[i1-1]--;
                    m_[i1-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                    take_111(temp, i1-2);
                    block_temp_.emplace_back(temp);
                }
                if(m_[i1] != 0){
                    if(m_[i1-1] >= 1 && m_[i1-2] >= 1){
                        m_[i1]--;
                        m_[i1-1]--;
                        m_[i1-2]--;
                        block_num_.num_111++;
                        Block temp(BlockType::_SHUNTSU, BlockColor::_MAN,3, true);
                        take_111(temp, i1-2);
                        block_temp_.emplace_back(temp);
                    }
                }
            }
        }
    }

    //取筒
    for(int i1 = 9, i2 = 17; i1 < 16 && i2 > 10; ++i1, --i2){
        //取右
        if(m_[i2] != 0){
            if(i2 == 17){
                if(m_[i2-1] >= 1 && m_[i2-2] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3,true);
                    take_111(temp, i2-2);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i2 == 16){
                if(m_[i2-1] >= 1 && m_[i2+1] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i2-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i2-1] >= 1 && m_[i2-2] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i2-2);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i2 < 16){
                if(m_[i2-1] >= 1 && m_[i2+1] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i2-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i2-1] >= 1 && m_[i2-2] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i2-2);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i2+1] >= 1 && m_[i2+2] >= 1){
                    m_[i2]--;
                    m_[i2+1]--;
                    m_[i2+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i2);
                    block_temp_.emplace_back(temp);
                }
                if(m_[i2] != 0){
                    if(m_[i2+1] >= 1 && m_[i2+2] >= 1){
                        m_[i2]--;
                        m_[i2+1]--;
                        m_[i2+2]--;
                        block_num_.num_111++;
                        Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                        take_111(temp, i2);
                        block_temp_.emplace_back(temp);
                    }
                }
            }
        }
        //取左
        if(m_[i1] != 0){
            if(i1 == 9){
                if(m_[i1+1] >= 1 && m_[i1+2] >= 1){
                    m_[i1]--;
                    m_[i1+1]--;
                    m_[i1+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3,true);
                    take_111(temp, i1);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i1 == 10){
                if(m_[i1-1] >= 1 && m_[i1+1] >= 1){
                    m_[i1]--;
                    m_[i1-1]--;
                    m_[i1+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i1-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i1+1] >= 1 && m_[i1+2] >= 1){
                    m_[i1]--;
                    m_[i1+1]--;
                    m_[i1+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i1);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i1 > 10){
                if(m_[i1-1] >= 1 && m_[i1+1] >= 1){
                    m_[i1]--;
                    m_[i1-1]--;
                    m_[i1+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i1-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i1+1] >= 1 && m_[i1+2] >= 1){
                    m_[i1]--;
                    m_[i1+1]--;
                    m_[i1+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i1-1] >= 1 && m_[i1-2] >= 1){
                    m_[i1]--;
                    m_[i1-1]--;
                    m_[i1-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                    take_111(temp, i1-2);
                    block_temp_.emplace_back(temp);
                }
                if(m_[i1] != 0){
                    if(m_[i1-1] >= 1 && m_[i1-2] >= 1){
                        m_[i1]--;
                        m_[i1-1]--;
                        m_[i1-2]--;
                        block_num_.num_111++;
                        Block temp(BlockType::_SHUNTSU, BlockColor::_PIN,3, true);
                        take_111(temp, i1-2);
                        block_temp_.emplace_back(temp);
                    }
                }
            }
        }
    }

    //取条
    for(int i1 = 18, i2 = 26; i1 < 25 && i2 > 19; ++i1, --i2){
        //取右
        if(m_[i2] != 0){
            if(i2 == 26){
                if(m_[i2-1] >= 1 && m_[i2-2] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3,true);
                    take_111(temp, i2-2);
                    block_temp_.emplace_back(temp);
                } 
            }
            else if(i2 == 25){
                if(m_[i2-1] >= 1 && m_[i2+1] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i2-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i2-1] >= 1 && m_[i2-2] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i2-2);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i2 < 25){
                if(m_[i2-1] >= 1 && m_[i2+1] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i2-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i2-1] >= 1 && m_[i2-2] >= 1){
                    m_[i2]--;
                    m_[i2-1]--;
                    m_[i2-2]--;
                    block_num_.num_111++;                    
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i2-2);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i2+1] >= 1 && m_[i2+2] >= 1){
                    m_[i2]--;
                    m_[i2+1]--;
                    m_[i2+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i2);
                    block_temp_.emplace_back(temp);
                }
                if(m_[i2] != 0){
                    if(m_[i2+1] >= 1 && m_[i2+2] >= 1){
                        m_[i2]--;
                        m_[i2+1]--;
                        m_[i2+2]--;
                        block_num_.num_111++;
                        Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                        take_111(temp, i2);
                        block_temp_.emplace_back(temp);
                    }
                }
            }
        }
        //取左
        if(m_[i1] != 0){
            if(i1 == 18){
                if(m_[i1+1] >= 1 && m_[i1+2] >= 1){
                    m_[i1]--;
                    m_[i1+1]--;
                    m_[i1+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3,true);
                    take_111(temp, i1);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i1 == 19){
                if(m_[i1-1] >= 1 && m_[i1+1] >= 1){
                    m_[i1]--;
                    m_[i1-1]--;
                    m_[i1+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i1-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i1+1] >= 1 && m_[i1+2] >= 1){
                    m_[i1]--;
                    m_[i1+1]--;
                    m_[i1+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i1);
                    block_temp_.emplace_back(temp);
                }
            }
            else if(i1 > 19){
                if(m_[i1-1] >= 1 && m_[i1+1] >= 1){
                    m_[i1]--;
                    m_[i1-1]--;
                    m_[i1+1]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i1-1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i1+1] >= 1 && m_[i1+2] >= 1){
                    m_[i1]--;
                    m_[i1+1]--;
                    m_[i1+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i1);
                    block_temp_.emplace_back(temp);
                }
                else if(m_[i1-1] >= 1 && m_[i1-2] >= 1){
                    m_[i1]--;
                    m_[i1-1]--;
                    m_[i1-2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                    take_111(temp, i1-2);
                    block_temp_.emplace_back(temp);
                }
                if(m_[i1] != 0){
                    if(m_[i1-1] >= 1 && m_[i1-2] >= 1){
                        m_[i1]--;
                        m_[i1-1]--;
                        m_[i1-2]--;
                        block_num_.num_111++;
                        Block temp(BlockType::_SHUNTSU, BlockColor::_SUO,3, true);
                        take_111(temp, i1-2);
                        block_temp_.emplace_back(temp);
                    }
                }
            }
        }
    }
}

void Player::search_3_all(std::vector<Block>& block_temp_, Blocknum& block_num_, int *m_){
    for(int i =0; i<34; ++i){
        if(m_[i] >= 3){
            m_[i] -= 3;
            block_num_.num_3++;
            Block temp(BlockType::_KOUTSU, TileTypeToBlockColor(static_cast<TileType>(i)), 3, true);
            take_3(temp, i);
            block_temp_.emplace_back(temp);
        }
    }
}

void Player::search_2_aim(std::vector<Block> &block_temp_, Blocknum &block_num_, int *m_, int pos){
    if(m_[pos] >= 2){
        block_num_.num_2++;
        m_[pos] -= 2;
        Block temp(BlockType::_TOITSU, TileTypeToBlockColor(static_cast<TileType>(pos)),2, true);
        take_2(temp, pos);
        block_temp_.emplace_back(temp);
    }
}

void Player::hu_type_1(std::vector<Block>& block_temp_, Blocknum& block_num_, int* m_){
    if(block_num_.num_111 + block_num_.num_3 == 4){
        Hu hu_temp_;
        for(int i = 0; i < 34; ++i){
            if(m_[i] != 0){
                m_[i]--;
                block_num_.num_2++;
                Block temp(BlockType::_TOITSU, TileTypeToBlockColor(static_cast<TileType>(i)), 2, true, TenType::_TANKI);
                take_1(temp, i);
                //把听的那张牌也放进去
                Tile ting_tile_(IndexToTileType(i),false);
                temp.tiles_.emplace_back(ting_tile_);
                std::sort(temp.tiles_.begin(), temp.tiles_.end());
                block_temp_.emplace_back(temp);
                hu_temp_.type_ = IndexToTileType(i);
                hu_temp_.ten_ = TenType::_TANKI;
                hu_temp_.blocks_ = block_temp_;

            }
        }

        std::sort(block_temp_.begin(), block_temp_.end());      //排序
        if(this->ten_.empty()){
            ten_.emplace_back(block_temp_);
            hu_.emplace_back(hu_temp_);
        } else{
            //判断是否有重复的听牌型
            bool is_same = true;
            for(int i = 0; i < ten_.size(); ++i){
                is_same = true;
                for(int j = 0; j < ten_[i].size(); ++j){
                    if(block_temp_[j] != ten_[i][j]){
                        is_same = false;
                        break;
                    }
                }
                if(is_same){break;}
            }
            if(is_same){block_temp_.clear();}
            else{
                ten_.emplace_back(block_temp_);
                hu_.emplace_back(hu_temp_);
            }
        }
    } else {
        //向听数
    }
}

void Player::hu_type_2(std::vector<Block> &block_temp_, Blocknum &block_num_, int *m_){
    if(block_num_.num_111 + block_num_.num_3 + block_num_.num_2 == 4){
        //剩下的是不是[2]
        Hu hu_temp_;
        //两面听牌的情况
        Hu hu_temp2_;
        std::vector<Block> block_temp2_ = block_temp_;
        bool ting_2 = false;
        for(int i = 0; i < 34; ++i){
            if(m_[i] == 2){
                ting_2 = true;
                m_[i] -= 2;
                block_num_.num_3++;
                Block temp(BlockType::_KOUTSU, TileTypeToBlockColor(static_cast<TileType>(i)),3,true, TenType::_SHANPON);
                take_2(temp, i);
                Tile ting_tile_(IndexToTileType(i),false);
                temp.tiles_.emplace_back(ting_tile_);
                std::sort(temp.tiles_.begin(), temp.tiles_.end());
                block_temp_.emplace_back(temp);
                hu_temp_.type_ = IndexToTileType(i);
                hu_temp_.ten_ = TenType::_SHANPON;
                hu_temp_.blocks_ = block_temp_;
                block_temp2_.clear();
            }
        }
        //剩下的是[11]或是[1_1]
        bool ting_11 = false;
        bool ting_1_1 = false;
        for(int i = 0; i < 34; ++i){
            if(m_[i] != 0){
                if(m_[i+1] == 1 && (i/9) == (i+1)/9){
                    if(i%9 == 0){
                        ting_11 = true;
                        m_[i]--;
                        m_[i+1]--;
                        block_num_.num_111++;
                        Block temp(BlockType::_SHUNTSU, TileTypeToBlockColor(static_cast<TileType>(i)),3,true, TenType::_PENCHAN);
                        take_11(temp, i);
                        Tile ting_tile_(IndexToTileType(i+2),false);
                        temp.tiles_.emplace_back(ting_tile_);
                        std::sort(temp.tiles_.begin(), temp.tiles_.end());
                        block_temp_.emplace_back(temp);
                        hu_temp_.type_ = IndexToTileType(i+2);
                        hu_temp_.ten_ = TenType::_PENCHAN;
                        hu_temp_.blocks_ = block_temp_;
                        block_temp2_.clear();
                    } else if(i%9 == 7){
                        ting_11 = true;
                        m_[i]--;
                        m_[i+1]--;
                        block_num_.num_111++;
                        Block temp(BlockType::_SHUNTSU, TileTypeToBlockColor(static_cast<TileType>(i)),3,true, TenType::_PENCHAN);
                        take_11(temp, i);
                        Tile ting_tile_(IndexToTileType(i-1),false);
                        temp.tiles_.emplace_back(ting_tile_);
                        std::sort(temp.tiles_.begin(), temp.tiles_.end());
                        block_temp_.emplace_back(temp);
                        hu_temp_.type_ = IndexToTileType(i-1);
                        hu_temp_.ten_ = TenType::_PENCHAN;
                        hu_temp_.blocks_ = block_temp_;
                        block_temp2_.clear();
                    } else{
                        ting_11 = true;
                        m_[i]--;
                        m_[i+1]--;
                        block_num_.num_111++;
                        Block temp1(BlockType::_SHUNTSU, TileTypeToBlockColor(static_cast<TileType>(i)),3,true, TenType::_RYANMEN);
                        take_11(temp1, i);
                        Block temp2 = temp1;
                        Tile ting_tile1_(IndexToTileType(i-1), false);
                        Tile ting_tile2_(IndexToTileType(i+2),false);
                        temp1.tiles_.emplace_back(ting_tile1_);
                        temp2.tiles_.emplace_back(ting_tile2_);
                        std::sort(temp1.tiles_.begin(), temp1.tiles_.end());
                        std::sort(temp2.tiles_.begin(), temp2.tiles_.end());
                        block_temp_.emplace_back(temp1);
                        block_temp2_.emplace_back(temp2);
                        hu_temp_.type_ = IndexToTileType(i-1);
                        hu_temp_.ten_ = TenType::_RYANMEN;
                        hu_temp_.blocks_ = block_temp_;
                        hu_temp2_.type_ = IndexToTileType(i+2);
                        hu_temp2_.ten_ = TenType::_RYANMEN;
                        hu_temp2_.blocks_ = block_temp2_;
                    }
                    
                }
                else if(m_[i+2] == 1 && (i/9) == (i+2)/9){
                    ting_1_1 = true;
                    m_[i]--;
                    m_[i+2]--;
                    block_num_.num_111++;
                    Block temp(BlockType::_SHUNTSU, TileTypeToBlockColor(static_cast<TileType>(i)),3,true, TenType::_KANCHAN);
                    take_1_1(temp, i);
                    Tile ting_tile_(IndexToTileType(i+1),false);
                    temp.tiles_.emplace_back(ting_tile_);
                    std::sort(temp.tiles_.begin(), temp.tiles_.end());
                    block_temp_.emplace_back(temp);
                    hu_temp_.type_ = IndexToTileType(i+1);
                    hu_temp_.ten_ = TenType::_KANCHAN;
                    hu_temp_.blocks_ = block_temp_;
                }
            }
        }
        bool ting_ = ting_2 || ting_11 || ting_1_1;

        if(ting_){
            std::sort(block_temp_.begin(), block_temp_.end());
            if(ten_.empty()){
                ten_.emplace_back(block_temp_);
                if(!block_temp2_.empty()){
                    ten_.emplace_back(block_temp2_);
                }
            }
            else {
                //判断是否有重复的听牌型
                bool is_same = true;
                for(int i = 0; i < ten_.size(); ++i){
                    is_same = true;
                    for(int j = 0; j < ten_[i].size(); ++j){
                        if(block_temp_[j] != ten_[i][j]){
                            is_same = false;
                            break;
                        }
                    }
                    if(is_same){break;}
                }
                if(is_same){block_temp_.clear();}
                else{
                    ten_.emplace_back(block_temp_);
                    hu_.emplace_back(hu_temp_);
                }
                if(!block_temp2_.empty()){
                    std::sort(block_temp2_.begin(), block_temp2_.end());
                    bool is_same2 = true;
                    for(int i = 0; i < ten_.size(); ++i){
                        is_same2 = true;
                        for(int j = 0; j < ten_[i].size(); ++j){
                            if(block_temp2_[j] != ten_[i][j]){
                                is_same2 = false;
                                break;
                            }
                        }
                        if(is_same2){break;}
                    }
                    if(is_same2){block_temp2_.clear();}
                    else{
                        ten_.emplace_back(block_temp2_);
                        hu_.emplace_back(hu_temp2_);
                    }
                }
            }
        }

    }
    else{
        //向听数

    }


}

void Player::take_111(Block& block, int t1){
    int t1_num = 0;
    int t2_num = 0;
    int t3_num = 0;
    for(int i=0; i<hand_.size(); ++i){
        if(t1_num == 0){
            if(static_cast<int>(hand_[i].type_) == t1){
                block.tiles_.emplace_back(hand_[i]);
                t1_num++;
            }
        }
        if(t2_num == 0){
            if(static_cast<int>(hand_[i].type_) == t1+1){
                block.tiles_.emplace_back(hand_[i]);
                t2_num++;
            }
        }
        if(t3_num == 0){
            if(static_cast<int>(hand_[i].type_) == t1+2){
                block.tiles_.emplace_back(hand_[i]);
                t3_num++;
            }
        }
    }
    std::sort(block.tiles_.begin(), block.tiles_.end());
}

void Player::take_3(Block &block, int t1){
    int t1_num = 0;
    for(int i = 0; i < hand_.size(); ++i){
        if(t1_num < 3){
            if(static_cast<int>(hand_[i].type_) == t1){
                t1_num++;
                block.tiles_.emplace_back(hand_[i]);
            }
        }
    }
}

void Player::take_2(Block &block, int t1){
    int t1_num = 0;
    for(int i = 0; i < hand_.size(); ++i){
        if(t1_num < 2){
            if(static_cast<int>(hand_[i].type_) == t1){
                block.tiles_.emplace_back(hand_[i]);
                t1_num++;
            }
        }
    }
}

void Player::take_1(Block &block, int t1){
    int t1_num = 0;
    for(int i = 0; i < hand_.size(); ++i){
        if(t1_num == 0){
            if(static_cast<int>(hand_[i].type_) == t1){
                block.tiles_.emplace_back(hand_[i]);
                t1_num++;
            }
        }
    }
}

void Player::take_11(Block &block, int t1){
    int t1_num = 0;
    int t2_num = 0;
    for(int i = 0; i < hand_.size(); ++i){
        if(t1_num == 0){
            if(static_cast<int>(hand_[i].type_) == t1){
                block.tiles_.emplace_back(hand_[i]);
                t1_num++;
            }
        }
        if(t2_num == 0){
            if(static_cast<int>(hand_[i].type_) == t1+1){
                block.tiles_.emplace_back(hand_[i]);
                t2_num++;
            }
        }
    }
    std::sort(block.tiles_.begin(), block.tiles_.end());

}

void Player::take_1_1(Block &block, int t1){
    int t1_num = 0;
    int t2_num = 0;
    for(int i = 0; i < hand_.size(); ++i){
        if(t1_num == 0){
            if(static_cast<int>(hand_[i].type_) == t1){
                block.tiles_.emplace_back(hand_[i]);
                t1_num++;
            }
        }
        if(t2_num == 0){
            if(static_cast<int>(hand_[i].type_) == t1+2){
                block.tiles_.emplace_back(hand_[i]);
                t2_num++;
            }
        }
    }
    std::sort(block.tiles_.begin(), block.tiles_.end());
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

int Player::is_yakuhai_sangen(Hu& _hu){
    int _sangen_num = 0;
    for(auto& _temp_block : _hu.blocks_){
        if(_temp_block.is_sangen()){
            _sangen_num++;
        }
    }
    return _sangen_num;
}

int Player::is_rinshankaihou(){
    if(next_tile_.tile_state_ == TileState::_KAN){return 1;}
    return 0;
}

int Player::is_chitoitsu(int* _handnum){
    if(!is_menzen_){return 0;}
    std::vector<int> _pos_2;
    std::vector<int> _pos_1;
    int _m[34]{0};
    for(int i = 0; i < 34; ++i){
        _m[i] = _handnum[i];
    }
    Hu _hu_temp;
    for(int i = 0; i < 34; ++i){
        if(_m[i] == 2){
            _pos_2.emplace_back(i);
        }
        if(_m[i] == 1){
            _pos_1.emplace_back(i);
        }
    }
    if(_pos_2.size() == 6 && _pos_1.size() == 1){
        std::vector<Block> _block_temp;
        for(int i = 0; i < _pos_2.size(); ++i){
            Block _temp(BlockType::_TOITSU, TileTypeToBlockColor(static_cast<TileType>(_pos_2[i])), 2, true);
            take_2(_temp, _pos_2[i]);
            _m[_pos_2[i]] -= 2;
            _block_temp.emplace_back(_temp);
        }
        for(int i = 0; i < _pos_1.size(); ++i){
            _m[_pos_1[i]] -= 1;
            Block _temp(BlockType::_TOITSU, TileTypeToBlockColor(static_cast<TileType>(_pos_1[i])), 1, true);
            take_1(_temp, _pos_1[i]);
            Tile _ting_tile(IndexToTileType(_pos_1[i]), false);
            _temp.tiles_.emplace_back(_ting_tile);
            std::sort(_temp.tiles_.begin(), _temp.tiles_.end());
            _block_temp.emplace_back(_temp);
            _hu_temp.type_ = IndexToTileType(_pos_1[i]);
        }
        _hu_temp.ten_ = TenType::_TANKI;
        _hu_temp.yaku_.emplace_back(YakuType::_CHITOITSU);
        _hu_temp.fu_ = 25;
        _hu_temp.blocks_ = _block_temp;
        std::sort(_block_temp.begin(), _block_temp.end());
        ten_.emplace_back(_block_temp);
        hu_.emplace_back(_hu_temp);
    }
    return 0;
}

int Player::is_chantaiyao()
{
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

int Player::is_ryuuiisou(){
    for(auto& temp : hand_){
        if(!temp.is_green()){return 0;}
    }
    for(auto& temp : fixed_tiles_){
        if(!temp.is_green()){return 0;}
    }
    return 13;
}

int Player::is_kokushimusou(int *_handnum)
{
    if(!is_menzen_ || hand_.size() != 13){return 0;}
    int _m[34]{0};
    for(int i = 0; i < 34; ++i){
        _m[i] = _handnum[i];
    }
    int _t_index[13]{0,8,9,17,18,26,27,28,29,30,31,32,33};
    int _t_num[13]{0};
    for(int i = 0; i < 34; ++i){
        if(_m[i] != 0){
            switch(i){
                case 0: _t_num[0] = 1; break;
                case 8: _t_num[1] = 1; break;
                case 9: _t_num[2] = 1; break;
                case 17: _t_num[3] = 1; break;
                case 18: _t_num[4] = 1; break;
                case 26: _t_num[5] = 1; break;
                case 27: _t_num[6] = 1; break;
                case 28: _t_num[7] = 1; break;
                case 29: _t_num[8] = 1; break;
                case 30: _t_num[9] = 1; break;
                case 31: _t_num[10] = 1; break;
                case 32: _t_num[11] = 1; break;
                case 33: _t_num[12] = 1; break;
                default: return 0; break;
            }
        }
    }
    int _t_sum = 0;
    for(int i = 0; i < 13; ++i){
        _t_sum += _t_num[i];
    }
    if(_t_sum == 12){
        //普通
        Hu _hu_temp;
        _hu_temp.man_ = ManType::_YAKUMAN;
        _hu_temp.fu_ = 25;
        _hu_temp.fan_ = 13;
        _hu_temp.basic_point_ = 8000;
        _hu_temp.ten_ = TenType::_TANKI;
        std::vector<Block> _block_temp;
        _hu_temp.yaku_.emplace_back(YakuType::_KOKUSHIMUSOU);
        for(int i = 0; i < hand_.size(); ++i){
            Block _temp(BlockType::_INCOMPLETED, TileTypeToBlockColor(static_cast<TileType>(_t_index[i])), 1, true);
            _temp.tiles_.emplace_back(hand_[i]);
            _block_temp.emplace_back(_temp);
        }
        for(int i = 0; i < 13; ++i){
            if(_t_num[i] == 0){
                _hu_temp.type_ = IndexToTileType(_t_index[i]);
            }

        }
        std::sort(_block_temp.begin(), _block_temp.end());
        _hu_temp.blocks_ = _block_temp;
        hu_.emplace_back(_hu_temp);
        ten_.emplace_back(_block_temp);
    }
    if(_t_sum == 13){
        //13面
        Hu _hu_temp_13;
        _hu_temp_13.man_ = ManType::_YAKUMAN;
        _hu_temp_13.fu_ = 25;
        _hu_temp_13.fan_ = 13;
        _hu_temp_13.basic_point_ = 8000;
        _hu_temp_13.ten_ = TenType::_TANKI;
        _hu_temp_13.yaku_.emplace_back(YakuType::_KOKUSHIMUSOU);
        for(int i = 0; i < 13; ++i){
            Block _temp(BlockType::_INCOMPLETED, TileTypeToBlockColor(static_cast<TileType>(_t_index[i])), 1, true);
            Tile _tile(IndexToTileType(_t_index[i]), false);
            _temp.tiles_.emplace_back(_tile);
            _hu_temp_13.blocks_.emplace_back(_temp);

        }
        ten_.emplace_back(_hu_temp_13.blocks_);
        for(int i = 0; i < 13; ++i){
            _hu_temp_13.type_ = IndexToTileType(_t_index[i]);
            hu_.emplace_back(_hu_temp_13);
        }
    }
}
