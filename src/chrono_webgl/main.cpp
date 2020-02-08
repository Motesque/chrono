#include "chrono_webgl/ChHttpServer.h"
#include <chrono>
#include <memory>
#include <iostream>
#include "Python.h"
#include <cstdlib>
#include "flatbuffers/flatbuffers.h"
#include "assets_generated.h"

void on_message(const uint8_t* data, size_t size) {
    std::cout << "on_message " << size << std::endl;
}
////https://github.com/KageKirin/flatGLM/blob/master/schemas/flatmath.fbs
int main(int argc, char *argv[]) {
    using namespace chrono::webgl;

    Vector3f boxSize(1,2,3);
    flatbuffers::FlatBufferBuilder builder;
    auto cylShape = CreateCylinderShape(builder, 1, 0.1);
    auto cmdNewShape = CreateCommandNewShape(builder, 0, AnyShape_CylinderShape, cylShape.Union());
    auto cmdMsg1 = CreateWebGLMessage(builder, 1, AnyCommand_CommandNewShape, cmdNewShape.Union());
    std::vector<flatbuffers::Offset<WebGLMessage>> messages;
    messages.push_back(cmdMsg1);
    auto vector = builder.CreateVector(messages);
    auto msg = CreateWebGLMessageList(builder, vector);
    builder.Finish(msg);
    std::cout << "size: " << builder.GetSize() << std::endl;
    chrono::webgl::ChHttpServerPython srv;
    srv.Start(8888, "/Users/tobiaslang/dev/chrono/src/chrono_webgl/resources",  on_message);
    //uint8_t buf[512*1024];
    auto start = std::chrono::system_clock::now();
    for (int i=0;i < 10000 ; i++) {
        srv.SendWebsocketMessage(builder.GetBufferPointer(),builder.GetSize());
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end-start;
    std::cout << "Duration" << diff.count() << " sec\n";
    srv.Stop() ;
}
