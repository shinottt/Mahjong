#ifndef _MAHJONG_CAREER_HEADER_
#define _MAHJONG_CAREER_HEADER_

#include<string>
#include<fstream>


#include"sn_log.h"

const std::string CAREER_FILE_PATH = "career.dat";


/*
记录各种数据

读写文件
*/
class Career{
public:

    //各种役的数量
    int riichi_ = 0;
    int double_riichi_ = 0;
    int ippatsu_ = 0;
    int menzentsumo_ = 0;




    void read_from_file();
    void write_to_file();


};


void Career::read_from_file(){
    std::fstream file_;
    file_.open(CAREER_FILE_PATH.c_str(), std::ios::binary | std::ios::in);
    while(file_.peek() != EOF){
        file_.read((char*)this, sizeof(*this));
    }
    file_.close();
    sn_consolelog_info("career data read from file: {}", CAREER_FILE_PATH);
}


void Career::write_to_file(){
    std::fstream file_;
    file_.open(CAREER_FILE_PATH.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    file_.write((const char*)this, sizeof(*this));
    file_.close();
    sn_consolelog_info("career data write to file: {}", CAREER_FILE_PATH);
}







#endif  // _MAHJONG_CAREER_HEADER_