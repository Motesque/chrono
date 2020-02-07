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
// include the source code of the python module as raw string
unsigned char chrono_webgl_server_py[] =  
    #include "chrono_webgl/chrono_webgl_server.py"
;

namespace chrono {
namespace webgl {



ChHttpServerPython::ChHttpServerPython() : 
    python_thread() {

}

ChHttpServerPython::~ChHttpServerPython() {

}

static PyObject* chrono_webgl_server_module;
static volatile int server_rc = -1;
static ChHttpServerInterface::websocket_message_received_cb_t websocket_message_received_cb;

enum EventType {
    EventType_Http_Server_Error = -1,
    EventType_Http_Server_Ok = 0,
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
        case EventType_Http_Server_Ok: { server_rc = 0;} break;
        case EventType_Http_Server_Error: { server_rc = 1;} break;
        case EventType_Client_WS_Message: {
            // call back to "host"
            if (websocket_message_received_cb) {
                websocket_message_received_cb(nullptr,77);
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

void ChHttpServerPython::RunPython(int port, const std::string& web_root) {
    // inject our on-the-fly module
    PyImport_AppendInittab("chrono_webgl", &PyInit_ChronoWebGL);
    Py_Initialize();
    PyObject *builtins = PyEval_GetBuiltins();
    PyObject *compile = PyDict_GetItemString(builtins, "compile");
    PyObject *code = PyObject_CallFunction(compile, "sss", chrono_webgl_server_py, "chrono_webgl_server", "exec");
    chrono_webgl_server_module = PyImport_ExecCodeModule("chrono_webgl_server", code);

    if (chrono_webgl_server_module != NULL) {
      std::cout << "Loaded chrono_webgl_server module." << std::endl;
      PyObject* func_main_obj = PyObject_GetAttrString(chrono_webgl_server_module, "main");  
      PyObject* arglist = Py_BuildValue("(is)", port, web_root.c_str());
      CHECK_CALL_OBJECT(PyObject_CallObject(func_main_obj, arglist));
      Py_DECREF(chrono_webgl_server_module);
      Py_DECREF(arglist);
      chrono_webgl_server_module = NULL;
      Py_DECREF(func_main_obj);
    }
    else {
       PyErr_Print();
       std::cout << "Cannot load chrono_webgl_server Python module. Please check the module_path" << std::endl;
    }
    Py_FinalizeEx();
}

int ChHttpServerPython::Start(int port, const std::string& web_root, websocket_message_received_cb_t cb) {
  if (python_thread.joinable()) {
      // already started. stop first
      return -1;
  }
  websocket_message_received_cb = cb;
  python_thread = std::thread(&ChHttpServerPython::RunPython, this, port, web_root);
  // do not return until the server is op and running
  while (server_rc == -1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  return server_rc;
}

int ChHttpServerPython::Stop() {
    //https://docs.python.org/3/c-api/init.html#gilstate
    // if we have python running, notify it to end
   if (chrono_webgl_server_module) {
      PyGILState_STATE gstate;
      gstate = PyGILState_Ensure(); 
      PyObject* f = PyObject_GetAttrString(chrono_webgl_server_module, "notify_shutdown");  
      if (f) {
        // this causes our pyton function in python_thread to exit and in consequence exits the whole thread
        CHECK_CALL_OBJECT(PyObject_CallObject(f, nullptr));
        Py_DECREF(f);
      }
      else {
          PyErr_Print();
      }
      PyGILState_Release(gstate);
   }
   python_thread.join();
   python_thread = std::thread();
   std::cout << "ChHttpServerPython closed" << std::endl;
   return 0;
}

 int ChHttpServerPython::SendWebsocketMessage(const uint8_t* data, size_t size)
 {
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure(); 
    PyObject* func_queue_ws_data = PyObject_GetAttrString(chrono_webgl_server_module, "queue_ws_data");  
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