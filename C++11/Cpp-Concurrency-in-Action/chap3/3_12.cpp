#include <iostream>
#include <mutex>
#include <thread>
#include <exception>

// 使用 std::call_once 作为类成员的延迟初始化(线程安全)
// g++ -Wall  -g -std=c++11 3_12.cpp -o 3_12 -lpthread

struct connection_info{};

struct data_packet{};

struct connection_handle{
    void send_data(data_packet const&)    {}
    data_packet receive_data(){
        return data_packet();
    }
};

struct remote_connection_manager{
    connection_handle open(connection_info const&){
        return connection_handle();
    }
} connection_manager;


class X{
private:
    connection_info connection_details;
    connection_handle connection;
    std::once_flag connection_init_flag;

    void open_connection(){
        std::cout << " void open_connection()" << std::endl;
        connection = connection_manager.open(connection_details);
    }

public:
    X(connection_info const& connection_details_):
        connection_details(connection_details_)   {}

    void send_data(data_packet const& data) {
        try{
             std::call_once(connection_init_flag,&X::open_connection,this);
        }catch(std::exception& ex) {
            std::cout << ex.what() << std::endl;
        }
        connection.send_data(data);
    }

    data_packet receive_data()    {
        try{
             std::call_once(connection_init_flag,&X::open_connection,this);
        }catch(std::exception& ex) {
            std::cout << ex.what() << std::endl;
        }
        return connection.receive_data();
    }
};

int main(){
    connection_info info;
    X x(info);
    data_packet data = x.receive_data();
    x.send_data(data);
}