#!/bin/bash

mv ./src/chrono_python/CMakeLists.txt ./src/chrono_python/CMakeLists.txt.orig
sed  -e 's/set(Python_ADDITIONAL_VERSIONS 3.4)//g' \
    -e 's/find_package(PythonInterp REQUIRED)/find_package (Python3 COMPONENTS Interpreter Development REQUIRED)/g' \
    -e 's/set(DBG_SCRIPT false)/set(DBG_SCRIPT true)/g' \
    -e 's/find_package(PythonLibs.*//g' \
    -e 's/PYTHON_INCLUDE_DIR/Python3_INCLUDE_DIRS/g' \
    -e 's/PYTHON_LIBRARIES/Python3_LIBRARIES/g' \
    -e 's/PYTHON_EXECUTABLE/Python3_EXECUTABLE/g' \
    -e 's/\${PYTHON_LIBRARY}/\${CH_PYTHONLIB}/g' src/chrono_python/CMakeLists.txt.orig > src/chrono_python/CMakeLists.txt

# Patch for bullet double precision. The chrono_irrlicht library includes bullet headers for the debug drawing.
# If USE_BULLET_DOUBLE=ON, we need to add '#define BT_USE_DOUBLE_PRECISION' to make chrono_irrlicht btVector3 type match chrono_engine types
# Otherwise we copy double to floats which causes severe render issues (and might even cause crashes)
mv ./src/chrono_irrlicht/CMakeLists.txt ./src/chrono_irrlicht/CMakeLists.txt.orig
sed  -e '/COMPILE_DEFINITIONS "CH_API_COMPILE_IRRLICHT")/a\
    \
    if (USE_BULLET_DOUBLE) \
    SET_TARGET_PROPERTIES(ChronoEngine_irrlicht PROPERTIES \
    COMPILE_DEFINITIONS "BT_USE_DOUBLE_PRECISION") \
    endif()' src/chrono_irrlicht/CMakeLists.txt.orig > src/chrono_irrlicht/CMakeLists.txt

# install necessary dependencies
brew install cmake irrlicht eigen wget

pushd .
mkdir -p _chrono_build/macos && cd _chrono_build/macos
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_DEMOS=ON  -DENABLE_MODULE_PYTHON=ON  -DENABLE_MODULE_IRRLICHT=ON -DUSE_BULLET_DOUBLE=ON ../../
make -j4
make install
ln -s /usr/local/share/chrono/python/pychrono /Library/Frameworks/Python.framework/Versions/3.7/lib/python3.7/site-packages/pychrono
popd

