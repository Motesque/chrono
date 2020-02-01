#include "chrono_threejs/ChThreeJsServer.h"
#include <memory>
namespace chrono {
namespace threejs {

} // end ns threejs
} // end ns chrono

int main() {
    using namespace chrono;
    using namespace chrono::threejs;
    std::shared_ptr<ChThreeJsServer> srv = std::make_shared<ChThreeJsServer>();
    srv->run("8999", "/tmp");
}