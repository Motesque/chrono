#===============================================================================
# CMake configuration file for the Chrono WebGL module
# 
# Cannot be used stand-alone (it's loaded by CMake config. file in parent dir.)
#===============================================================================

option(ENABLE_MODULE_WEBGL "Enable the Chrono WebGL module" OFF)

# Return now if this module is not enabled
IF(NOT ENABLE_MODULE_WEBGL)
  RETURN()
ENDIF()

MESSAGE(STATUS "==== Chrono WebGL module ====")

#-------------------------------------------------------------------------------
# List all the files in the ChronoEngine_webgl library

SET(ChronoEngine_WEBGL_SOURCES
  ChWebGLApp.cpp
  ChHttpServer.cpp
  main.cpp
)

SET(ChronoEngine_WEBGL_HEADERS
  ChWebGLApp.h
  ChHttpServer.h
  ChApiWebGL.h
)

SOURCE_GROUP("" FILES
  ${ChronoEngine_WEBGL_SOURCES}
  ${ChronoEngine_WEBGL_HEADERS}
)

find_package (Python3 COMPONENTS Interpreter Development REQUIRED)


INCLUDE_DIRECTORIES(
  ${Python3_INCLUDE_DIRS}
  ../chrono_thirdparty/rapidjson
)

#ADD_LIBRARY(ChronoEngine_webgl SHARED
 # ${ChronoEngine_WEBGL_SOURCES}
 # ${ChronoEngine_WEBGL_HEADERS}
#)
ADD_EXECUTABLE(ChronoEngine_webgl 
  ${ChronoEngine_WEBGL_SOURCES}
  ${ChronoEngine_WEBGL_HEADERS}
)



SET_TARGET_PROPERTIES(ChronoEngine_webgl PROPERTIES
  COMPILE_FLAGS "${CH_CXX_FLAGS} ${CH_WEBGL_CXX_FLAGS}"
  LINK_FLAGS "${CH_LINKERFLAG_SHARED}")


target_compile_definitions(ChronoEngine_webgl PRIVATE "CH_API_COMPILE_WEBGL")

TARGET_LINK_LIBRARIES(ChronoEngine_webgl
  ChronoEngine
)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
TARGET_LINK_LIBRARIES(ChronoEngine_webgl
  ${MAC_LIBS}
  ${Python3_LIBRARIES}
)
ENDIF()

ADD_DEPENDENCIES (ChronoEngine_webgl ChronoEngine)

#-------------------------------------------------------------------------------
# Install the ChronoEngine_webgl library

INSTALL(TARGETS ChronoEngine_webgl
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

#INSTALL(FILES ${ChronoEngine_WEBGL_HEADERS} DESTINATION include/chrono_webgl)
INSTALL(TARGETS ChronoEngine_webgl DESTINATION /tmp/)
