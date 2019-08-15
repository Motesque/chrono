// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2019 projectchrono.org
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Dan Negrut
// =============================================================================

#pragma once
#include <vector>
#include "chrono/core/ChVector.h"
#include "chrono/core/ChMatrix33.h"
#include "chrono_granular/physics/ChGranular.h"
#include "chrono_granular/physics/ChGranularTriMesh.h"
#include "chrono/geometry/ChTriangleMeshConnected.h"

#define MESH_INFO_PRINTF(...)     \
    if (mesh_verbosity == INFO) { \
        printf(__VA_ARGS__);      \
    }

class CH_GRANULAR_API ChGranularChronoTriMeshAPI {
  public:
    enum MESH_VERBOSITY { QUIET = 0, INFO = 1 };

  private:
    MESH_VERBOSITY mesh_verbosity;
    /// Clean copy of mesh soup interacting with granular material in unified memory. Stored in UU
    chrono::granular::ChTriangleSoup<float3>* meshSoup;
    chrono::granular::ChGranParams_trimesh* tri_params;

  public:
    /// Setup data structures associated with triangle mesh
    void setupTriMesh(const std::vector<chrono::geometry::ChTriangleMeshConnected>& all_meshes,
                      unsigned int nTriangles,
                      std::vector<float> masses,
                      std::vector<bool> inflated,
                      std::vector<float> inflation_radii);

    /// Load triangle meshes into granular system. MUST happen before initialize is called
    void load_meshes(std::vector<std::string> objfilenames,
                     std::vector<chrono::ChMatrix33<float>> rotscale,
                     std::vector<float3> translations,
                     std::vector<float> masses,
                     std::vector<bool> inflated,
                     std::vector<float> inflation_radii);

    /// Verbosity level of the system

    /// Set simualtion verbosity -- used to check on very large, slow simulations or debug
    void setVerbosity(MESH_VERBOSITY level) { mesh_verbosity = level; }
};

class CH_GRANULAR_API ChGranularSMC_API {
  private:
    chrono::granular::ChSystemGranularSMC* pSMCgranSystem;

  public:
    ChGranularSMC_API() : pSMCgranSystem(NULL) {}
    // Set particle positions in UU
    void setElemsPositions(const std::vector<chrono::ChVector<float>>& points);

    // set the gran systems that the user talks to; beef up the API so that the gran system is bulid through the API,
    // instead of passing a gran system pointer to the API (the API builds the gran system; not the API coming in after
    // gran system is up
    void setGranSystemSMC(chrono::granular::ChSystemGranularSMC* granSystem) { pSMCgranSystem = granSystem; }
};