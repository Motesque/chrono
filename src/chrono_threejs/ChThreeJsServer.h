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
#ifndef CHAPITHREEJSSERVER_H
#define CHAPITHREEJSSERVER_H

#include <string>
#include "chrono_threejs/ChApiThreeJs.h"

namespace chrono {
namespace threejs {

class ChApiThreeJs ChThreeJsServer {
  public:
    ChThreeJsServer();
    virtual ~ChThreeJsServer();
    int run(const std::string& port, const std::string& doc_root );
  private:
    static void EventHandler(struct mg_connection *nc, int ev, void *ev_data);
    static void HandleSumCall(struct mg_connection *nc, struct http_message *hm);
};

} // end namespace threejs
} // end namespace chrono

#endif