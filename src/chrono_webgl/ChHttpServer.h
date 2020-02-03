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
#include "chrono_webgl/ChApiWebGL.h"

namespace chrono {
namespace webgl {

class ChApiWebGL ChHttpServer {
  public:
    ChHttpServer();
    virtual ~ChHttpServer();
    int open(const std::string& port, const std::string& doc_root );
    int close();
    int send_message();
  private:
    std::thread http_thread;
    static std::string doc_root;
};

} // end namespace webgl
} // end namespace chrono

#endif