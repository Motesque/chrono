/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include "chrono_webgl/ChHttpServer.h"
#include "Python.h"
#include <chrono>
// to generate use xxd -i chrono_webgl_server.py
#include "chrono_webgl/chrono_webgl_server_py.h"
namespace chrono {
namespace webgl {

PyObject* module_obj;

ChHttpServer::ChHttpServer() {

}

ChHttpServer::~ChHttpServer() {

}
static volatile int server_ready=0;
static ChHttpServer::ws_message_received_cb_t ws_message_received_cb;

enum EventType {
    EventType_Http_Server_Ready = 0,
    EventType_Client_WS_Message = 1,
    EventType_Client_HTTP_Message = 2
};

static PyObject* chrono_webgl_post_event(PyObject *self, PyObject *args)
{
    int event_type = -1;
    PyObject* event_data = nullptr; 
    if(!PyArg_ParseTuple(args, "iO", &event_type, &event_data)) { 
        return NULL;
    }
    switch (event_type) {
        case EventType_Http_Server_Ready: {server_ready = 1;} break;
        case EventType_Client_WS_Message: {
            if (ws_message_received_cb) {
                ws_message_received_cb(nullptr,77);
            }
            // assume PyObject is a byte buffer
            PyObject* objectsRepresentation = PyObject_Repr(event_data);
            std::cout << "EventType_Client_WS_Message: " << std::string(PyUnicode_AsUTF8(objectsRepresentation)) << std::endl;
            
        } break;
        default: ;
    };
    return Py_BuildValue("");
}

static PyMethodDef ChronoWebGLMethods[] = {
    {"post_event", chrono_webgl_post_event, METH_VARARGS,
     "Return whether the embedded Python runtime should exit"},
    {NULL, NULL, 0, NULL}
};

static PyModuleDef ChronoWebGLModule = {
    PyModuleDef_HEAD_INIT, "chrono_webgl", NULL, -1, ChronoWebGLMethods,
    NULL, NULL, NULL, NULL
};

static PyObject* PyInit_ChronoWebGL(void)
{
    return PyModule_Create(&ChronoWebGLModule);
}

#define CHECK_CALL_OBJECT(FUNC)  PyObject* val = FUNC; if (val) { Py_DECREF(val);} else  {PyErr_Print();}

void ChHttpServer::run(int port, const std::string& web_root) {
    // inject our on-the-fly module
    PyImport_AppendInittab("chrono_webgl", &PyInit_ChronoWebGL);
    Py_Initialize();
    // add the module_path to the PYTHON_PATH. The symplest way is to run a small Python script 
    // as it is platform independent
    // std::string cmd_py_path = std::string("import sys;sys.path.insert(0,'") + module_path + std::string("')");
    // PyRun_SimpleString(cmd_py_path.c_str());
    // PyObject* module_name = PyUnicode_DecodeFSDefault("chrono_webgl_server");
    // xxd -i chrono_webgl_server.py
    PyObject *builtins = PyEval_GetBuiltins();
    PyObject *compile = PyDict_GetItemString(builtins, "compile");
    PyObject *code = PyObject_CallFunction(compile, "sss", chrono_webgl_server_py, "chrono_webgl_server", "exec");
    module_obj = PyImport_ExecCodeModule("chrono_webgl_server", code);

    //module_obj = PyImport_Import(module_name);
    //Py_DECREF(module_name);
    if (module_obj != NULL) {
      std::cout << "Loaded chrono_webgl_server module." << std::endl;
      PyObject* func_main_obj = PyObject_GetAttrString(module_obj, "main");  
      PyObject* arglist = Py_BuildValue("(is)", port, web_root.c_str());
      CHECK_CALL_OBJECT(PyObject_CallObject(func_main_obj, arglist));
      Py_DECREF(module_obj);
      Py_DECREF(arglist);
      module_obj = NULL;
      Py_DECREF(func_main_obj);
    }
    else {
       PyErr_Print();
       std::cout << "Cannot load chrono_webgl_server Python module. Please check the module_path" << std::endl;
    }
    Py_FinalizeEx();
}

int ChHttpServer::open(int port, const std::string& web_root, ws_message_received_cb_t cb) {
  ws_message_received_cb = cb;
  python_thread = std::thread(&ChHttpServer::run, this, port, web_root);
  while (!server_ready) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  return 0;
}

int ChHttpServer::close() {
    //https://docs.python.org/3/c-api/init.html#gilstate
    // if we have python running, notify it to end
   if (module_obj) {
      PyGILState_STATE gstate;
      gstate = PyGILState_Ensure(); 
      PyObject* f = PyObject_GetAttrString(module_obj, "notify_shutdown");  
      if (f) {
        CHECK_CALL_OBJECT(PyObject_CallObject(f, nullptr));
        Py_DECREF(f);
      }
      else {
          PyErr_Print();
      }
      PyGILState_Release(gstate);
   }
   python_thread.join();
   std::cout << "ChHttpServer closed" << std::endl;
   return 0;
}

 int ChHttpServer::send_ws_message(const uint8_t* data, size_t size)
 {
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure(); 
    PyObject* func_queue_ws_data = PyObject_GetAttrString(module_obj, "queue_ws_data");  
    if (func_queue_ws_data) {
         PyObject* arglist = Py_BuildValue("(y#)", data, size);
         CHECK_CALL_OBJECT(PyObject_CallObject(func_queue_ws_data, arglist));
         Py_DECREF(arglist);
         Py_DECREF(func_queue_ws_data);
    }
    PyGILState_Release(gstate);
    return 0;
 }

}

}