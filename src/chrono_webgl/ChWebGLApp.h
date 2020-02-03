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

#ifndef CHWEBGLAPP_H
#define CHWEBGLAPP_H

#include <vector>

#include "chrono/core/ChRealtimeStep.h"
#include "chrono/physics/ChSystem.h"
#include "chrono/core/ChTransform.h"

#include "chrono_webgl/ChApiWebGL.h"


namespace chrono {
namespace webgl {

/// @addtogroup webgl_module
/// @{
class ChHttpServer;

class ChApiWebGL ChWebGLApp {
  public:
    /// Create the web resources to serve three.js web page
    ChWebGLApp(ChSystem* psystem, int port, const std::string& web_root);

    /// Safely delete all web resources 
    virtual ~ChWebGLApp();

    //// Accessor functions
    ChSystem* GetSystem() { return system; }

    /// Set/Get the time step for time integration. This value is used when
    /// calling DoStep() in a loop, to advance the simulation by one timestep.
    void SetTimestep(double val) { timestep= val;}
    double GetTimestep() { return timestep; }

    /// If set to true, you can use DoStep() in the simulation loop to advance the
    /// simulation by one timestep. Otherwise, you have to handle the time
    /// stepping by yourself, e.g. by calling ChSystem::DoStepDynamics().
    void SetStepManage(bool val) { step_manage = val; }
    bool GetStepManage() { return step_manage; }

    /// If set to true, the function DoStep() will try to use a timestep that is
    /// the same as that used to refresh the interface and compute physics (i.e.,
    /// it attempts to keep soft-realtime performance).
    /// Note: the realtime step is bounded above by the timestep specified through
    /// SetTimestep()! This clamping will happen if there's too much load.
    void SetTryRealtime(bool val) { try_realtime = val; }
    bool GetTryRealtime() { return try_realtime; }

    /// Set/Get the simulation state (running or paused)
    void Restart() {}
 
	/// Set if the COG frames will be plotted
	void SetPlotCOGFrames(bool val) { plot_cog_frames = val; }

    /// Set the scale for symbol drawing (link frames, COGs, etc.)
    void SetSymbolscale(double val) { symbol_scale=val;}
    double GetSymbolscale() { return symbol_scale; }

    /// Use this function to hook a custom event receiver to the application.
    //void SetUserEventReceiver(irr::IEventReceiver* mreceiver) { user_receivers.push_back(mreceiver); }


    void BeginScene();
    /// Call this important function inside a cycle like
    ///    while(should_run) {...}
    /// in order to advance the physics by one timestep. The value of the timestep
    /// can be set via SetTimestep(). Optionally, you can use SetTryRealtime(true)
    /// if your simulation can run in realtime.
    /// Alternatively, if you want to use ChSystem::DoStepDynamics() directly in
    /// your loop, use SetStepManage(false).
    virtual void DoStep();

    /// Call this important function inside a loop like
    ///    while(application.GetDevice()->run()) {...}
    /// in order to get the redrawing of all 3D shapes and all the GUI elements.
    virtual void DrawAll();

    /// Call this to end the scene draw at the end of each animation frame
    virtual void EndScene();

    void  AssetBindAll();

    //
    // Some wizard functions for 'easy setup' of the application window:
    //
    void AddTypicalLogo(const std::string& mlogofilename = GetChronoDataFile("logo_chronoengine_alpha.png")) {
        //ChIrrWizard::add_typical_Logo(GetDevice(), mlogofilename);
    }

    void AddTypicalCamera(const ChVector<>& pos,
                          const ChVector<>& dir) {
        //ChIrrWizard::add_typical_Camera(GetDevice(), mpos, mtarg);
    }

  private:
    // The ChronoEngine system:
    ChSystem* system;
    std::shared_ptr<ChHttpServer>  server;

    // The port to serve the web page:
    int port;
    std::string web_root;

    bool step_manage;
    bool try_realtime;
    bool plot_cog_frames;
    double timestep;
    double symbol_scale; 
   
    ChRealtimeStepTimer m_realtime_timer;
    friend class ChHttpServer;
};

/// @} webgl_module

}  // end namespace webgl
}  // end namespace chrono

#endif
