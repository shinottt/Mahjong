#ifndef _MAHJONG_CAREER_HEADER_
#define _MAHJONG_CAREER_HEADER_

#include<string>
#include<fstream>

const std::string SINGLE_CAREER_FILE_PATH = "single.dat";
const std::string NET_CAREER_FILE_PATH = "net.dat";


/*
记录各种数据

读写文件
*/



class Career{
public:



    void read_from_file_single();
    void write_to_file_single();
    void read_from_file_net();
    void write_to_file_net();


    void print_data_to_file_single();
    void print_data_to_console_single();
    void print_data_to_file_net();
    void print_data_to_console_net();


};

inline void Career::read_from_file_single(){
    std::fstream file_;
    file_.open(SINGLE_CAREER_FILE_PATH.c_str(), std::ios::binary | std::ios::in);
    while(file_.peek() != EOF){
        file_.read((char*)this, sizeof(*this));
    }
    file_.close();
    printf("career data read from file: %s\n", SINGLE_CAREER_FILE_PATH.c_str());
}


inline void Career::write_to_file_single(){
    std::fstream file_;
    file_.open(SINGLE_CAREER_FILE_PATH.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    file_.write((const char*)this, sizeof(*this));
    file_.close();
    printf("career data write to file: %s\n", SINGLE_CAREER_FILE_PATH.c_str());
}

inline void Career::read_from_file_net(){
    std::fstream file_;
    file_.open(NET_CAREER_FILE_PATH.c_str(), std::ios::binary | std::ios::in);
    while(file_.peek() != EOF){
        file_.read((char*)this, sizeof(*this));
    }
    file_.close();
    printf("career data read from file: %s\n", NET_CAREER_FILE_PATH.c_str());
}


inline void Career::write_to_file_net(){
    std::fstream file_;
    file_.open(NET_CAREER_FILE_PATH.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    file_.write((const char*)this, sizeof(*this));
    file_.close();
    printf("career data write to file: %s\n", NET_CAREER_FILE_PATH.c_str());
}


#endif  // _MAHJONG_CAREER_HEADER_