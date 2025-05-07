#ifndef _MAHJONG_SHINOHAYU_NET_SERVER_565212asgsg_HEADER_
#define _MAHJONG_SHINOHAYU_NET_SERVER_565212asgsg_HEADER_


#include<stdexcept>
#include<system_error>
#include<memory>
#include<string>
#include<vector>
#include<span>
#include<atomic>

#include"basic.hpp"


#include<winsock2.h>
#include<ws2tcpip.h>


class sn_socket{
public:
    enum class ConnectType{TCP, UDP};

    // 构造函数，用于Accept创建新socket
    explicit sn_socket(SOCKET raw_socket) : sock_(raw_socket){}

    explicit sn_socket(ConnectType type = ConnectType::TCP){
        int sock_type = (type == ConnectType::TCP) ? SOCK_STREAM : SOCK_DGRAM;
        sock_ = socket(AF_INET6, sock_type, 0);
        if(sock_ == INVALID_SOCKET){
            throw std::system_error(WSAGetLastError(), std::system_category(), "socket() failed");
        }

        // 启用双栈支持，允许同时处理IPv4和IPv6
        int ipv6_only = 0;
        if(setsockopt(sock_, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&ipv6_only, sizeof(ipv6_only)) == SOCKET_ERROR){
            throw std::system_error(WSAGetLastError(), std::system_category(), "setsockopt() failed");
        }
    }

    // 移动语义支持
    sn_socket(sn_socket&& other) noexcept : sock_(other.sock_) {
        other.sock_ = INVALID_SOCKET;
    }
    sn_socket& operator=(sn_socket&& other) noexcept {
        if(this != &other){
            Close();
            sock_ = other.sock_;
            other.sock_ = INVALID_SOCKET;
        }
        return *this;
    }

    void Bind(const std::string& ip, uint16_t port){
        sockaddr_in6 addr = MakeAddress(ip, port);
        if(bind(sock_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR){
            throw std::system_error(WSAGetLastError(), std::system_category(), "bind() failed");
        }
    }

    void Connect(const std::string& ip, uint16_t port){
        sockaddr_in6 addr = MakeAddress(ip, port);
        if(connect(sock_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR){
            throw std::system_error(WSAGetLastError(), std::system_category(), "connect() failed");
        }
    }

    void Send(std::span<const char> data){
        int sent = send(sock_, data.data(), static_cast<int>(data.size()), 0);
        if(sent == SOCKET_ERROR){
            throw std::system_error(WSAGetLastError(), std::system_category(), "send() failed");
        }
        sent_bytes_ += sent;
    }

    std::vector<char> Receive(size_t max_size = 4096){
        std::vector<char> buffer(max_size);
        int received = recv(sock_, buffer.data(), static_cast<int>(max_size), 0);
        if(received == SOCKET_ERROR){
            throw std::system_error(WSAGetLastError(), std::system_category(), "recv() failed");
        }
        received_bytes_ += received;
        buffer.resize(received);
        return buffer;
    }

    // 启动监听，服务器端
    void Listen(int backlog = SOMAXCONN){
        if(listen(sock_, backlog) == SOCKET_ERROR){
            throw std::system_error(WSAGetLastError(), std::system_category(), "listen() failed");
        }
    }

    //接受新连接，返回新sn_socket对象
    sn_socket Accept(){
        SOCKET client_socket = accept(sock_, nullptr, nullptr);
        if(client_socket == INVALID_SOCKET){
            throw std::system_error(WSAGetLastError(), std::system_category(), "accept() failed");
        }
        return sn_socket(client_socket);
    }

    void Close(){
        if(sock_ != INVALID_SOCKET){
            closesocket(sock_);
            sock_ = INVALID_SOCKET;
        }
    }

    SOCKET NativeHandle() const noexcept {return sock_;}


    // 统计流量
    uint64_t GetSentBytes() const noexcept {return sent_bytes_;}
    uint64_t GetReceivedBytes() const noexcept {return received_bytes_;}
    void ResetCounters() noexcept {sent_bytes_ = 0; received_bytes_ = 0;}


    ~sn_socket(){Close();}

private:
    SOCKET sock_ = INVALID_SOCKET;

    std::atomic<uint64_t> sent_bytes_{0};
    std::atomic<uint64_t> received_bytes_{0};

    sockaddr_in6 MakeAddress(const std::string& ip, uint16_t port){
        sockaddr_in6 addr{};
        addr.sin6_family = AF_INET6;
        addr.sin6_port = htons(port);

        // 处理IPv4映射地址（如"::ffff:192.168.1.1")
        if(ip.empty() || ip == "0.0.0.0"){
            addr.sin6_addr = in6addr_any;   // 绑定所有接口
        } else{
            if(inet_pton(AF_INET6, ip.c_str(), &addr.sin6_addr) == 1){
                return addr;
            }

            // 若失败，尝试解析为IPv4并构造映射地址
            sockaddr_in ipv4_addr{};
            if(inet_pton(AF_INET, ip.c_str(), &ipv4_addr.sin_addr) == 1){
                // 构造 ::FFFF:IPv4格式
                memset(&addr.sin6_addr, 0, 12);
                addr.sin6_addr.s6_addr[10] = 0xFF;
                addr.sin6_addr.s6_addr[11] = 0xFF;
                memcpy(&addr.sin6_addr.s6_addr[12], &ipv4_addr.sin_addr, 4);
                return addr;
            }
            throw std::runtime_error("Invalid IP address: "+ip);
        } 
        return addr;
    }

};




class sock_manager : public singleton<sock_manager>{
    friend class singleton<sock_manager>;
public:
    sock_manager(){
        WSADATA wsa_data;
        if(WSAStartup(MAKEWORD(2,2), &wsa_data) != 0){
            throw std::runtime_error("WSAStartup failed");
        }
    }
    ~sock_manager(){WSACleanup();}

};









#endif //_MAHJONG_SHINOHAYU_NET_SERVER_565212asgsg_HEADER_