#pragma once

#include <crsf/CRAPI/TDynamicModuleInterface.h>

#include <render_pipeline/rppanda/showbase/direct_object.hpp>

class OpenVRManager;
class CHICCamManager;

namespace rpcore {
class RenderPipeline;
}

namespace crsf {
class TGraphicRenderEngine;
}

class MainApp : public crsf::TDynamicModuleInterface, public rppanda::DirectObject
{
public:
    MainApp();
    ~MainApp() override;

    void OnLoad() override;
    void OnStart() override;
    void OnExit() override;

private:
    rpcore::RenderPipeline* pipeline_;
    crsf::TGraphicRenderEngine* rendering_engine_;

    std::unique_ptr<OpenVRManager> openvr_manager_;
    std::unique_ptr<CHICCamManager> chic_cam_manager_;
};
