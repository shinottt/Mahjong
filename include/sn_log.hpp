#ifndef _SN_LOG_SINGLETON_HEADER_
#define _SN_LOG_SINGLETON_HEADER_

#include<iostream>
#include<queue>
#include<memory>
#include<string>
#include<mutex>
#include<condition_variable>
#include<thread>
#include<chrono>
#include<fstream>
#include<atomic>
#include<sstream>
#include<vector>
#include<iomanip>       // put_time
#include<stdexcept>     //异常处理

// 将多个参数连接成字符串
template<typename... Args>
inline std::string concatenate_string(const Args&&... args){
    std::ostringstream oss;
    ((oss<<args), ...);
    return oss.str();
}

// 将单个参数转换为字符串
template<typename T>
std::string param_to_string(T&& t){
    std::ostringstream oss;
    oss << t;
    return oss.str();
}

// 将参数列表转换为字符串列表
template<typename... Args>
void multiparam_to_string(std::vector<std::string>& str_vec, Args&&... args){
    (str_vec.emplace_back(param_to_string(args)), ...);
}

// 格式化字符串
template<typename... Args>
std::string format(std::string fmt, Args&&... args){
    size_t args_index = 0;

    std::vector<std::string> str_vec = {param_to_string(std::forward<Args>(args))...};

    std::ostringstream oss;
    for(auto it = fmt.begin(); it != fmt.end(); ++it){
        if(*it == '{'){
            if(*(it+1) == '}'){
                if(args_index >= str_vec.size()){
                    oss << "{}";
                } else{
                    oss << str_vec[args_index];               
                }
                ++args_index;
                ++it;
            }
        } else{
            oss << *it;
        }
    }
    if(args_index < str_vec.size()){
        for(; args_index < str_vec.size(); ++args_index){
            oss << str_vec[args_index];
        }
    }
    return oss.str();
} 

// 日志级别枚举类型
enum class LogLevel{
    DEBUG_, INFO_, WARNING_, ERROR_, FATAL_
};

enum class LogType{
    CONSOLE, FILE, NET
};

using LOG_MSG_PACKAGE = std::tuple<std::string, LogType, LogLevel>;

inline std::string get_current_time(){
    std::ostringstream oss;
    const std::chrono::time_point<std::chrono::system_clock> now_ = std::chrono::system_clock::now();
    const std::time_t now_t_ = std::chrono::system_clock::to_time_t(now_);
    oss <<" [" <<std::put_time(std::localtime(&now_t_), "%Y-%m-%d %H:%M:%S") <<"] ";
    return oss.str();
}

// 日志级别转换为字符串
inline std::string log_level_to_string(LogLevel level){
    switch(level){
        case LogLevel::INFO_: return " [INFO] ";
        case LogLevel::DEBUG_: return " [DEBUG] ";
        case LogLevel::WARNING_: return " [WARNING] ";
        case LogLevel::ERROR_: return " [ERROR] ";
        case LogLevel::FATAL_: return " [FATAL] ";
        default: return " [UNKNOWN] ";
    }
}

const std::string cmd_color_reset ="\033[0m";
const std::string cmd_color_cyan = "\033[36m";
const std::string cmd_color_white= "\033[37m";
const std::string cmd_color_yellow = "\033[33m";
const std::string cmd_color_red ="\033[31m";
const std::string cmd_color_purple = "\033[35m";


inline std::string console_color(LogLevel level){
    switch(level){
        case LogLevel::DEBUG_:
            return cmd_color_cyan;
            break;
        case LogLevel::INFO_:
            return cmd_color_white;
            break;
        case LogLevel::WARNING_:
            return cmd_color_yellow;
            break;
        case LogLevel::ERROR_:
            return cmd_color_red;
            break;
        case LogLevel::FATAL_:
            return cmd_color_purple;
            break;
        default:
            throw std::runtime_error("Unknown log type");
            return cmd_color_reset;
            break;
    }
}


// 单例模式基类模板
template<typename T>
class singleton_log{
protected:
    singleton_log() = default;
    singleton_log(const singleton_log& ) = delete;
    singleton_log& operator=(const singleton_log&) = delete;
    static std::shared_ptr<T> _instance;
public:
    ~singleton_log(){}
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
std::shared_ptr<T> singleton_log<T>::_instance = nullptr;

// 日志队列
class log_queue{
public:
    void push(const LOG_MSG_PACKAGE& msg){
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(msg);
        cond_var_.notify_one();
    }
    bool pop(LOG_MSG_PACKAGE& msg){
        std::unique_lock<std::mutex> lock(mutex_);
        cond_var_.wait(lock, [this](){return !queue_.empty() || is_shutdown_;});     // 防止虚假唤醒
        // 消费逻辑
        if(is_shutdown_ && queue_.empty()){
            return false;
        }

        msg = queue_.front();
        queue_.pop();
        return true;
    }

    void shutdown(){
        std::lock_guard<std::mutex> lock(mutex_);
        is_shutdown_ = true;
        cond_var_.notify_all();
    }             

private:
    std::queue<LOG_MSG_PACKAGE> queue_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
    bool is_shutdown_ = false;
};

class logger : public singleton_log<logger>{
    friend class singleton_log<logger>;
public:

    void log_init_console(){
        exit_flag_ = false;
    }

    void log_init_file(const std::string& filename){
        log_file_.open(filename.c_str(), std::ios::out | std::ios::app);
        exit_flag_ = false;
        if(!log_file_.is_open()){
            throw std::runtime_error("Failed to open log file");
        }
    }
    // Todo
    void log_init_net(std::string netname){
        exit_flag_ = false;
    }

    //Todo
    void log_init_all(std::string filename = "log.txt", std::string netname = "127.0.0.1"){
        log_init_console();
        log_init_file(filename);
        log_init_net(netname);
    }

    void log_start(){
        worker_thread_ = std::thread([this](){
            LOG_MSG_PACKAGE msg;
            while(log_queue_.pop(msg)){
                switch(std::get<1>(msg)){
                    case LogType::CONSOLE :
                        console_log(msg);break;
                    case LogType::FILE :
                        file_log(msg);break;
                    case LogType::NET :
                        net_log(msg);break;
                default:
                    throw std::runtime_error("Unknown log type");
                    break;
                }
            }
        });
    }

    ~logger(){
        exit_flag_ = true;
        log_queue_.shutdown();
        if(worker_thread_.joinable()){
            worker_thread_.join();
        }

        if(log_file_.is_open()){
            log_file_.close();
        }
        std::cout << "logger destroyed" << std::endl;
    }
    template<typename... Args>
    void log(LogLevel level, LogType type, const std::string& format_str, Args&&... args){
        LOG_MSG_PACKAGE msg{
            log_level_to_string(level) + get_current_time() + format(format_str, std::forward<Args>(args)...),
            type,
            level
        };
        log_queue_.push(msg);
    }

private:
    logger() = default;

    log_queue log_queue_;
    std::thread worker_thread_;
    std::fstream log_file_;
    std::string log_net_;   // 日志网络地址，Todo
    std::atomic<bool> exit_flag_;

    void console_log(LOG_MSG_PACKAGE& msg){
        std::cout<<console_color(std::get<2>(msg))<<std::get<0>(msg)<<cmd_color_reset<<std::endl;
    }

    void file_log(LOG_MSG_PACKAGE& msg) {
        log_file_<<std::get<0>(msg)<<std::endl;
    }

    void net_log(LOG_MSG_PACKAGE& msg){

    }
};



#endif //_SN_LOG_HEADER_