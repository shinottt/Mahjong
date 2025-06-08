#ifndef _MAHJONG_SHINOHAYU_CENTER_SERVER_HEADER_
#define _MAHJONG_SHINOHAYU_CENTER_SERVER_HEADER_

#include<thread>
#include<atomic>

#include"state.h"


class center_server{
public:

    explicit center_server(){
        is_exit_ = false;
    }

    void handle_connect();


    void init();

    ~center_server(){
        is_exit_ = true;
        if(work_thread_.joinable()){
            work_thread_.join();
        }
    }

private:

    state state_;

    std::thread work_thread_;
    std::atomic<bool> is_exit_;

};






#endif      //_MAHJONG_SHINOHAYU_CENTER_SERVER_HEADER_