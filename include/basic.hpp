#ifndef _MAHJONG_SHINOHAYU_BASIC_HEADER_
#define _MAHJONG_SHINOHAYU_BASIC_HEADER_

#include<iostream>
#include<memory>
#include<mutex>         // std::call_once, std::once_flag
#include<source_location>

inline void print_location(const std::string_view message, const std::source_location location = std::source_location::current()){
        std::clog << "文件: "
              << location.file_name() << '('
              << location.line() << ':'
              << location.column() << ") `"
              << location.function_name() << "`: "
              << message << '\n';
}

// 单例模式基类模板
template<typename T>
class singleton{
protected:
    singleton() = default;
    singleton(const singleton& ) = delete;
    singleton& operator=(const singleton&) = delete;
    static std::shared_ptr<T> _instance;
public:
    ~singleton(){}
    static std::shared_ptr<T> get_instance(){
        static std::once_flag flag;
        std::call_once(flag, [](){
            _instance = std::shared_ptr<T>(new T());
        });
        return _instance;
    }

    void print_address(){
        std::cout<< _instance.get() << std::endl;
    }
};
template<typename T>
std::shared_ptr<T> singleton<T>::_instance = nullptr;



#endif      //_MAHJONG_SHINOHAYU_BASIC_HEADER_