#ifndef _MAHJONG_SHINOHAYU_CENTER_SERVER_HEADER_
#define _MAHJONG_SHINOHAYU_CENTER_SERVER_HEADER_


#include<string>
#include<vector>
#include<deque>
#include<memory>
#include<map>

#include<asio.hpp>



#include"state.h"

// 消息类型枚举
enum class MessageType : uint8_t {
    CONNECTION_ACK = 0x01,       // 连接确认
    ROOM_LIST = 0x02,            // 房间列表
    ROOM_CREATED = 0x03,         // 房间创建成功
    PLAYER_JOINED = 0x04,        // 玩家加入房间
    OBSERVER_JOINED = 0x05,      // 观战者加入房间
    ROOM_STATE = 0x06,           // 房间状态
    GAME_START = 0x07,           // 游戏开始
    PLAYER_ACTION = 0x08,        // 玩家动作
    GAME_UPDATE = 0x09,          // 游戏状态更新
    CHAT_MESSAGE = 0x0A,         // 聊天消息
    ERROR_MESSAGE = 0xFF         // 错误消息
};

// 房间状态
enum class RoomState : uint8_t {
    WAITING = 0,    // 等待玩家加入
    IN_GAME = 1,    // 游戏中
    FINISHED = 2    // 游戏结束
};

struct MessageHeader{
    MessageType type;
    uint32_t length;      
};

struct PlayerInfo{
    int id;
    std::string name;
    bool ready;
};

// 房间信息
struct RoomInfo{
    int id;
    std::string name;
    int player_count;
    int observer_count;
    RoomState state;
};


// 会话基类
class Session : public std::enable_shared_from_this<Session>{
public:

    Session(asio::ip::tcp::socket socket):socket_(std::move(socket)){}
    ~Session()=default;

    void start(){read_header();}

    void send(const std::vector<uint8_t>& msg){
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if(!write_in_progress){
            do_write();
        }
    }

    void close(){socket_.close();}

    virtual void handle_message(const std::vector<uint8_t>& msg) = 0;

    asio::ip::tcp::socket& socket(){return socket_;}
protected:
    
    void read_header(){
        auto self(shared_from_this());
        asio::async_read(socket_, asio::buffer(&read_header_, sizeof(MessageHeader)), 
            [this, self](const asio::error_code& error, size_t /*length*/){
                if(!error && read_header_.length > 0){
                    read_body();
                }
                else{
                    handle_disconnect();
                }
            }
        );
    }

    void read_body() {
        auto self(shared_from_this());
        read_body_.resize(read_header_.length);
        asio::async_read(socket_, asio::buffer(read_body_),
            [this, self](asio::error_code ec, size_t /*length*/) {
                if (!ec) {
                    // 处理消息
                    handle_message(read_body_);
                    read_header();
                } else {
                    handle_disconnect();
                }
            }
        );
    }

    void do_write(){
        auto self(shared_from_this());
        asio::async_write(socket_, asio::buffer(write_msgs_.front()), 
            [this, self](const asio::error_code& error, size_t /*length*/){
                if(!error){
                    write_msgs_.pop_front();
                    if(!write_msgs_.empty()){
                        do_write();
                    }
                } else {
                    handle_disconnect();
                }
            }
        );
    }

    virtual void handle_disconnect(){
        // default: 断开连接
        socket_.close();
    }

    std::vector<uint8_t> serialize_message(MessageType type, const std::vector<uint8_t>& body);

    std::vector<uint8_t> create_error_message(const std::string& error_message){
        std::vector<uint8_t> body;
        body.insert(body.end(), error_message.begin(), error_message.end());
        return serialize_message(MessageType::ERROR_MESSAGE, body);
    }

    asio::ip::tcp::socket socket_;
    MessageHeader read_header_;
    std::vector<uint8_t> read_body_;
    std::deque<std::vector<uint8_t>> write_msgs_;

};


class PlayerSession : public Session{
public:
    PlayerSession(asio::ip::tcp::socket socket, int id, std::string name):Session(std::move(socket)), id_(id), name_(name){}

    int id() const {return id_;}
    std::string name() const {return name_;}
    void set_ready(bool ready){ready_ = ready;}
    bool is_ready() const {return ready_;}

    void handle_message(const std::vector<uint8_t>& msg) override {
        // 处理消息逻辑
    }

private:
    int id_;
    std::string name_;
    bool ready_ = false;
};


class ObserverSession : public Session{
public:
    ObserverSession(asio::ip::tcp::socket socket, int id):Session(std::move(socket)), id_(id) {}

    int id() const {return id_;}

    void handle_message(const std::vector<uint8_t>& msg) override {
        // 处理消息逻辑
    }
private:
    int id_;
};


class GameRoom : public std::enable_shared_from_this<GameRoom> {
public:
    


    int id() const {return id_;}
    std::string name() const {return name_;}
    RoomState state() const {return state_;}

    // 添加玩家
    bool addd_player(std::shared_ptr<PlayerSession> player){
        if(players_.size() >= 4){
            return false;
        }
        
        players_.push_back(player);
        player_infos_.push_back(PlayerInfo{player->id(), player->name(), false});

        if(creator_id_ == -1){
            creator_id_ = player->id();
        }

        broadcast_player_list();
        return true;
    }

    // 添加观众
    void add_observer(std::shared_ptr<ObserverSession> observer){
        observers_.push_back(observer);
        send_room_state(observer);
    }

    // 移除玩家
    void remove_player(int player_id){
        auto it = std::find_if(players_.begin(), players_.end(),
            [player_id](const auto& p){return p->id() == player_id;}
        );

        if(it != players_.end()){
            players_.erase(it);

            player_infos_.erase(std::remove_if(player_infos_.begin(), player_infos_.end(),
                [player_id](const auto& info){return info.id == player_id;}
            ), player_infos_.end());

            if(creator_id_ == player_id && !players_.empty()){
                creator_id_ = players_.front()->id();    //  重新指定房主
            }

            broadcast_player_list();

            if(state_ == RoomState::WAITING && players_.size() < 4){
                reset_ready_states();
            }
            /*
            可以让robot接管
            if(state_ == RoomState::IN_GAME){
                end_game();    //  结束游戏

            }
            */
        }
    }

    void remove_observer(int observer_id){
        observers_.erase(std::remove_if(observers_.begin(), observers_.end(),
            [observer_id](const auto& observer){return observer->id() == observer_id;}
        ), observers_.end());
    }

    void player_ready(int player_id, bool ready){
        for(auto& info : player_infos_){
            if(info.id == player_id){
                info.ready = ready;
                break;
            }
        }

        broadcast_player_list();
        check_start_game();
    }

    // Todo
    bool start_game(){
        if(players_.size() < 4){
            return false;
        }

        // 检查玩家是否都已准备
        for(const auto& info : player_infos_){
            if(!info.ready){
                return false;
            }
        }

        state_ = RoomState::IN_GAME;
        game_active_ = true;

        /*游戏逻辑 start*/

        /*游戏逻辑 end*/

        return true;
    }

    void broadcast(const std::vector<uint8_t>& msg){
        for(const auto& player : players_){
            player->send(msg);
        }
        
        for(const auto& observer : observers_){
            observer->send(msg);
        }
    }

    RoomInfo get_room_info() const {
        return RoomInfo{id_, name_, static_cast<int>(players_.size()), static_cast<int>(observers_.size()), state_};
    }

private:

    // 广播玩家列表
    void broadcast_player_list() {
        std::vector<uint8_t> body;
        
        // 房间ID
        uint32_t room_id = id_;
        body.insert(body.end(), reinterpret_cast<uint8_t*>(&room_id), 
                   reinterpret_cast<uint8_t*>(&room_id) + sizeof(room_id));
        
        // 玩家数量
        uint8_t player_count = player_infos_.size();
        body.push_back(player_count);
        
        // 玩家信息
        for (const auto& info : player_infos_) {
            // 玩家ID
            uint32_t id = info.id;
            body.insert(body.end(), reinterpret_cast<uint8_t*>(&id), 
                       reinterpret_cast<uint8_t*>(&id) + sizeof(id));
            
            // 玩家名称长度和名称
            uint8_t name_len = info.name.size();
            body.push_back(name_len);
            body.insert(body.end(), info.name.begin(), info.name.end());
            
            // 准备状态
            body.push_back(static_cast<uint8_t>(info.ready));
        }
        
        auto msg = serialize_message(MessageType::ROOM_STATE, body);
        broadcast(msg);
    }

    // 发送房间状态给指定会话
    void send_room_state(std::shared_ptr<Session> session) {
        std::vector<uint8_t> body;
        
        // 房间ID和名称
        uint32_t room_id = id_;
        body.insert(body.end(), reinterpret_cast<uint8_t*>(&room_id), 
                   reinterpret_cast<uint8_t*>(&room_id) + sizeof(room_id));
        
        uint8_t name_len = name_.size();
        body.push_back(name_len);
        body.insert(body.end(), name_.begin(), name_.end());
        
        // 房间状态
        body.push_back(static_cast<uint8_t>(state_));
        
        // 玩家数量
        uint8_t player_count = players_.size();
        body.push_back(player_count);
        
        // 观战者数量
        uint8_t observer_count = observers_.size();
        body.push_back(observer_count);
        
        auto msg = serialize_message(MessageType::ROOM_STATE, body);
        session->send(msg);
    }

    // 检查是否可以开始游戏
    void check_start_game() {
        if (players_.size() < 4) return;
        
        for (const auto& info : player_infos_) {
            if (!info.ready) return;
        }
        
        start_game();
    }

    // 重置准备状态
    void reset_ready_states() {
        for (auto& info : player_infos_) {
            info.ready = false;
        }
        broadcast_player_list();
    }



    std::vector<uint8_t> serialize_message(MessageType type, const std::vector<uint8_t>& body) {
        std::vector<uint8_t> data;
        MessageHeader header;
        header.length = body.size();
        header.type = type;
        
        // 添加头部
        uint8_t* header_ptr = reinterpret_cast<uint8_t*>(&header);
        data.insert(data.end(), header_ptr, header_ptr + sizeof(MessageHeader));
        
        // 添加消息体
        data.insert(data.end(), body.begin(), body.end());
        return data;
    }

    int id_;
    std::string name_;
    RoomState state_ = RoomState::WAITING;
    int creator_id_ = -1;   //  房主id

    std::vector<std::shared_ptr<PlayerSession>> players_;
    std::vector<std::shared_ptr<ObserverSession>> observers_;
    std::vector<PlayerInfo> player_infos_;
    bool game_active_;
    int current_player_;


};


class RoomManager : public std::enable_shared_from_this<RoomManager> {
public:
    RoomManager(asio::io_context& io) : io_(io), next_room_id(1){}

private:
    asio::io_context& io_;
    int next_room_id;
    std::map<int, std::shared_ptr<GameRoom>> rooms_;

};



#endif      //_MAHJONG_SHINOHAYU_CENTER_SERVER_HEADER_