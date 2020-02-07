#include "chrono_webgl/ChHttpServer.h"
#include <chrono>
#include <memory>
#include <iostream>
#include "Python.h"
#include <cstdlib>

void on_message(const uint8_t* data, size_t size) {
    std::cout << "on_message " << size << std::endl;
}
int main(int argc, char *argv[]) {

    chrono::webgl::ChHttpServerPython srv;
    srv.Start(8888, "/Users/tobiaslang/dev/chrono/src/chrono_webgl/resources",  on_message);
    uint8_t buf[512*1024];
    auto start = std::chrono::system_clock::now();
    for (int i=0;i < 10000 ; i++) {
        srv.SendWebsocketMessage(buf,sizeof(buf));
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end-start;
    std::cout << "Duration" << diff.count() << " sec\n";
    srv.Stop() ;
}
