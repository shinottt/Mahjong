#include"net_server.hpp"


int main(){

    try{
        auto winsock = sock_manager::get_instance();

        sn_socket server(sn_socket::ConnectType::TCP);
        server.Bind("::", 12345);       // 绑定所有IPv6接口
        server.Listen();

        sn_socket client = server.Accept();
        auto data = client.Receive();
        std::cout<<"Received: "<<std::string(data.begin(), data.end())<<std::endl;

        std::cout<<"Received: "<<client.GetReceivedBytes() <<" bytes"<<std::endl;

    } catch (const std::exception& e){
        std::cerr << "Error: "<<e.what()<<std::endl;
        return 1;
    }

    return 0;
}