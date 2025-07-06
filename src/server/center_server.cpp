#include"center_server.h"


std::vector<uint8_t> Session::serialize_message(MessageType type, const std::vector<uint8_t>& body){
    std::vector<uint8_t> data;

    // 创建消息头对象
    MessageHeader header;
    header.length = body.size();
    header.type = type;

    // 将消息头转换为字节数组
    // 添加头部
    uint8_t* header_ptr = reinterpret_cast<uint8_t*>(&header);
    data.insert(data.end(), header_ptr, header_ptr + sizeof(MessageHeader));

    // 添加消息体
    data.insert(data.end(), body.begin(), body.end());
    return data;
}



