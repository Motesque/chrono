/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#include "mongoose.h"
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "chrono_webgl/ChHttpServer.h"
#include <chrono>
namespace chrono {
namespace webgl {
  
ChHttpServer::ChHttpServer() {

}

ChHttpServer::~ChHttpServer() {

}

static struct mg_serve_http_opts s_http_server_opts;
static std::vector<mg_connection*> connected_ws_clients;

static int is_websocket(const struct mg_connection *nc) {
  return nc->flags & MG_F_IS_WEBSOCKET;
}

static void eventHandler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;

  switch (ev) {
    case MG_EV_WEBSOCKET_HANDSHAKE_DONE: {
      std::cout << "websocket handshake done" << std::endl;
      connected_ws_clients.push_back(nc);
      /* New websocket connection. Tell everybody. */
      //broadcast(nc, mg_mk_str("++ joined"));
      break;
    }
    case MG_EV_WEBSOCKET_FRAME: {
      struct websocket_message *wm = (struct websocket_message *) ev_data;
      /* New websocket message. Tell everybody. */
      struct mg_str d = {(char *) wm->data, wm->size};
      // broadcast(nc, d);
       std::cout << "websocket data frame received" << std::endl;
      break;
    }
    case MG_EV_HTTP_REQUEST: {
      mg_serve_http(nc, hm, s_http_server_opts); /* Serve static content */
      break;
    }
    case MG_EV_CLOSE: {
      /* Disconnect. Tell everybody. */
      if (is_websocket(nc)) {
        auto it = std::find(connected_ws_clients.begin(), connected_ws_clients.end(), nc);
        if (it != connected_ws_clients.end()) {
          std::cout << "removing ws client" << std::endl;
          connected_ws_clients.erase(it);
        }
        std::cout << "websocket closed" << std::endl;
        //broadcast(nc, mg_mk_str("-- left"));
      }
      break;
    }
    default:
      break;
  }
}

static volatile bool http_thread_should_run = false;

void run(const std::string& port, const std::string& doc_root, ChHttpServer* instance) {
  static struct mg_mgr mgr;
  struct mg_connection *nc;
  struct mg_bind_opts bind_opts;
  int i;
  char *cp;
  const char *err_str = "nix";

  mg_mgr_init(&mgr, NULL);

  s_http_server_opts.document_root = doc_root.c_str();
  
  /* Set HTTP server options */
  memset(&bind_opts, 0, sizeof(bind_opts));
  bind_opts.error_string = &err_str;
  nc = mg_bind_opt(&mgr, port.c_str(), eventHandler, bind_opts);
  if (nc == NULL) {
    std::cerr << "Error starting server on port " << port << ":"  << *bind_opts.error_string << std::endl;
  }

  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.enable_directory_listing = "yes";
  std::string msg = "Motesque";
  const size_t buf_size= 1024*8;
  uint8_t* buf = new uint8_t[buf_size];
  srand(11);
  std::chrono::high_resolution_clock timer;
  auto now = timer.now();
  auto last = now;
  while(http_thread_should_run) {
    now = timer.now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();
    if (deltaTime > 16) {
      //last = timer.now();
      //std::cout << "conencted ws clients: " << connected_ws_clients.size() << ":" << deltaTime << std::endl;
      std::for_each(connected_ws_clients.begin(), connected_ws_clients.end(), [&](mg_connection* conn) {
          mg_send_websocket_frame(conn, WEBSOCKET_OP_BINARY, buf, buf_size);
      });
    }
    int n = mg_mgr_poll(&mgr, 100);  
  }

  mg_mgr_free(&mgr);
}
std::string ChHttpServer::doc_root = "";

int ChHttpServer::open(const std::string& port, const std::string& doc_root ) {
  ChHttpServer::doc_root = doc_root;
  s_http_server_opts.document_root = ChHttpServer::doc_root.c_str();
  std::cout << "Starting RESTful server on port " << port << ":" << s_http_server_opts.document_root << std::endl;
  http_thread_should_run = true;
  http_thread = std::thread(run, port, doc_root, this);
  return 0;
}

int ChHttpServer::close() {
   http_thread_should_run = false;
   http_thread.join();
   return 0;
}



}

}