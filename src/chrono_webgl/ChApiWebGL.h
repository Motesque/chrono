// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2014 projectchrono.org
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================

#ifndef CHAPIWEBGL_H
#define CHAPIWEBGL_H

#include "chrono/ChVersion.h"
#include "chrono/core/ChPlatform.h"

// When compiling this library, remember to define CH_API_COMPILE_WEBGL
// (so that the symbols with 'ChApiWebGL' in front of them will be
// marked as exported). Otherwise, just do not define it if you
// link the library to your code, and the symbols will be imported.

#if defined(CH_API_COMPILE_WEBGL)
#define ChApiWebGL ChApiEXPORT
#else
#define ChApiWebGL ChApiIMPORT
#endif

/**
    @defgroup webgl_module WEBGL module
    @brief Runtime visualization with Three.js

    This module can be used to provide 3D realtime rendering
    in Chrono::Engine.

*/

namespace chrono {

/// @addtogroup webgl_module
/// @{

/// Namespace with classes for the Irrlicht module.
namespace webgl {}

/// @}
}

#endif