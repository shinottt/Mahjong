#include"net_server.hpp"


int main(){

    try{
        auto winsock = sock_manager::get_instance();

        sn_socket client(sn_socket::ConnectType::TCP);
        client.Connect("127.0.0.1", 12345);       // IPv6本地环回
        client.Send(std::span<const char>("Hello Server IPv6!", 18));
        std::cout<<"Sent: "<<client.GetSentBytes() <<" bytes"<<std::endl;
    } catch (const std::exception& e){
        std::cerr << "Error: "<<e.what()<<std::endl;
        return 1;
    }


    return 0;
}