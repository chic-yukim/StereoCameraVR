#include "main.hpp"
#include "world.hpp"

#include <crsf/RenderingEngine/TGraphicRenderEngine.h>

CRSEEDLIB_MODULE_CREATOR(StereoCameraVR);

// ************************************************************************************************
StereoCameraVR::StereoCameraVR(void): crsf::TDynamicModuleInterface(CRMODULE_ID_STRING)
{
}

void StereoCameraVR::OnLoad(void)
{
    auto rendering_engine = crsf::TGraphicRenderEngine::GetInstance();

    world_ = new World(
        *rendering_engine->GetRenderPipeline(),
        m_property);
}

void StereoCameraVR::OnStart(void)
{
    auto rendering_engine = crsf::TGraphicRenderEngine::GetInstance();

    rendering_engine->SetWindowTitle(CRMODULE_ID_STRING);

    world_->start();
}

void StereoCameraVR::OnExit(void)
{
    world_.clear();
}
