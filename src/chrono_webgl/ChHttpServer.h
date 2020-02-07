// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2020 projectchrono.org
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Tobias Lang
// =============================================================================
#ifndef CHHTTPSERVER_H
#define CHHTTPSERVER_H

#include <string>
#include <thread>    
#include <functional>
#include "chrono_webgl/ChApiWebGL.h"


namespace chrono {
namespace webgl {


class ChApiWebGL ChHttpServerInterface {
  public:
    using websocket_message_received_cb_t =  std::function<void(const uint8_t* data, size_t size)>;
    virtual int Start(int port, const std::string& web_root, websocket_message_received_cb_t cb) = 0;
    virtual int Stop() = 0;
    virtual int SendWebsocketMessage(const uint8_t* data, size_t size) = 0;
};

class ChApiWebGL ChHttpServerPython : public ChHttpServerInterface {
  public:
    ChHttpServerPython();
    virtual ~ChHttpServerPython();
    int Start(int port, const std::string& web_root, websocket_message_received_cb_t cb);
    int Stop();
    int SendWebsocketMessage(const uint8_t* data, size_t size);
  private:
    void RunPython(int port, const std::string& web_root);
  private:
    std::thread python_thread;
};


} // end namespace webgl
} // end namespace chrono

#endif