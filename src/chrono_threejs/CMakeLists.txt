#===============================================================================
# CMake configuration file for the Chrono threejs module
# 
# Cannot be used stand-alone (it's loaded by CMake config. file in parent dir.)
#===============================================================================

option(ENABLE_MODULE_THREEJS "Enable the Chrono ThreeJs module" OFF)

# Return now if this module is not enabled
IF(NOT ENABLE_MODULE_THREEJS)
  RETURN()
ENDIF()

MESSAGE(STATUS "==== Chrono ThreeJs module ====")

#-------------------------------------------------------------------------------
# List all the files in the ChronoEngine_threejs library

SET(ChronoEngine_THREEJS_SOURCES
  ChThreeJsApp.cpp
  ChThreeJsServer.cpp
  mongoose.cpp
)

SET(ChronoEngine_THREEJS_HEADERS
  ChThreeJsApp.h
  ChThreeJsServer.h
  ChApiThreeJs.h
  mongoose.h
)

SOURCE_GROUP("" FILES
  ${ChronoEngine_THREEJS_SOURCES}
  ${ChronoEngine_THREEJS_HEADERS}
)


INCLUDE_DIRECTORIES()

#ADD_LIBRARY(ChronoEngine_threejs SHARED
#  ${ChronoEngine_THREEJS_SOURCES}
#  ${ChronoEngine_THREEJS_HEADERS}
#)

ADD_EXECUTABLE(ChronoEngine_threejs 
  ${ChronoEngine_THREEJS_SOURCES}
  ${ChronoEngine_THREEJS_HEADERS}
)

SET_TARGET_PROPERTIES(ChronoEngine_threejs PROPERTIES
  COMPILE_FLAGS "${CH_CXX_FLAGS} ${CH_THREEJS_CXX_FLAGS}"
  LINK_FLAGS "${CH_LINKERFLAG_SHARED}")


target_compile_definitions(ChronoEngine_threejs PRIVATE "CH_API_COMPILE_THREEJS")

TARGET_LINK_LIBRARIES(ChronoEngine_threejs
  ChronoEngine
  ${LIBWEBSOCKETS_LIBRARIES}
)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
TARGET_LINK_LIBRARIES(ChronoEngine_threejs
  ${MAC_LIBS}
)
ENDIF()

ADD_DEPENDENCIES (ChronoEngine_threejs ChronoEngine)

#-------------------------------------------------------------------------------
# Install the ChronoEngine_threejs library

INSTALL(TARGETS ChronoEngine_threejs
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

#INSTALL(FILES ${ChronoEngine_THREEJS_HEADERS} DESTINATION include/chrono_threejs)
INSTALL(TARGETS ChronoEngine_threejs DESTINATION /tmp/)
