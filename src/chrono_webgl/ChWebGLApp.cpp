#include "chrono_webgl/ChHttpServer.h"
#include "chrono_webgl/ChWebGLApp.h"
#include <memory>
#include <iostream>
namespace chrono {
namespace webgl {

ChWebGLApp::ChWebGLApp(ChSystem* system, int port, const std::string& web_root):
    system(system),
    server(new ChHttpServer()),
    port(port),
    web_root(web_root),
    step_manage(false),
    try_realtime(false),
    plot_cog_frames(false),
    timestep(0.003),
    symbol_scale(0.1) 
{
}

void ChWebGLApp::AssetBindAll()
{
    server->open(port, web_root, nullptr);
}

ChWebGLApp::~ChWebGLApp() {
    server->close();
    server.reset();
}

void ChWebGLApp::BeginScene() {

}

void ChWebGLApp::DoStep() {

}

void ChWebGLApp::DrawAll() {

}

void ChWebGLApp::EndScene() {

}


} // end ns webgl
} // end ns chrono

