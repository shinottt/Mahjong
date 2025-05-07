#include"center_server.h"


void center_server::handle_connect(){

    work_thread_ = std::thread([this](){
        sn_socket server(sn_socket::ConnectType::TCP);
        server.Bind("::", 12345);
        server.Listen();

        fd_set master_set;
        FD_ZERO(&master_set);
        FD_SET(server.NativeHandle(), &master_set);

        while(!is_exit_){
            fd_set read_set = master_set;
            int socket_count = select(0, &read_set, nullptr, nullptr, nullptr);

            for(int i = 0; i < socket_count; ++i){
                SOCKET sock = read_set.fd_array[i];
                if(sock == server.NativeHandle()){
                    // 接受新连接
                    sn_socket client = server.Accept();
                    FD_SET(client.NativeHandle(), &master_set);
                    std::cout<<"New client connected"<<std::endl;
                } else {
                    // 处理客户端的数据
                    sn_socket client(sock);
                    try{
                        //Todo

                    } catch(const std::exception& e){
                        std::cerr << "Error: "<<e.what()<<std::endl;
                        return 1;
                    }
                }
            }
        }

    });

}


