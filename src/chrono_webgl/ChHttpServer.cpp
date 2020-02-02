/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#include "mongoose.h"
#include <string>
#include <iostream>

#include "chrono_webgl/ChHttpServer.h"

namespace chrono {
namespace webgl {
  
ChHttpServer::ChHttpServer() {

}

ChHttpServer::~ChHttpServer() {

}

static struct mg_serve_http_opts s_http_server_opts;

void ChHttpServer::HandleSumCall(struct mg_connection *nc, struct http_message *hm) {
  char n1[100], n2[100];
  double result;

  /* Get form variables */
  mg_get_http_var(&hm->body, "n1", n1, sizeof(n1));
  mg_get_http_var(&hm->body, "n2", n2, sizeof(n2));

  /* Send headers */
  mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

  /* Compute the result and send it back as a JSON object */
  result = strtod(n1, NULL) + strtod(n2, NULL);
  mg_printf_http_chunk(nc, "{ \"result\": %lf }", result);
  mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
}

void ChHttpServer::EventHandler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;

  switch (ev) {
    case MG_EV_HTTP_REQUEST:
      if (mg_vcmp(&hm->uri, "/api/v1/sum") == 0) {
        HandleSumCall(nc, hm); /* Handle RESTful call */
      } else if (mg_vcmp(&hm->uri, "/printcontent") == 0) {
        char buf[100] = {0};
        memcpy(buf, hm->body.p,
               sizeof(buf) - 1 < hm->body.len ? sizeof(buf) - 1 : hm->body.len);
        printf("%s\n", buf);
      } else {
        mg_serve_http(nc, hm, s_http_server_opts); /* Serve static content */
      }
      break;
    default:
      break;
  }
}

int ChHttpServer::run(const std::string& port, const std::string& doc_root ) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  struct mg_bind_opts bind_opts;
  int i;
  char *cp;
  const char *err_str;

  mg_mgr_init(&mgr, NULL);

  s_http_server_opts.document_root = doc_root.c_str();
  
  /* Set HTTP server options */
  memset(&bind_opts, 0, sizeof(bind_opts));
  bind_opts.error_string = &err_str;
  nc = mg_bind_opt(&mgr, port.c_str(), EventHandler, bind_opts);
  if (nc == NULL) {
    std::cerr << "Error starting server on port " << port << ":"  << *bind_opts.error_string << std::endl;
  }

  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.enable_directory_listing = "yes";

  std::cout << "Starting RESTful server on port " << port << ":" << s_http_server_opts.document_root << std::endl;
  
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}

}

}