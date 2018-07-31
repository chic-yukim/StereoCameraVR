#include "main.hpp"
#include "world.hpp"

#include <crsf/RenderingEngine/TGraphicRenderEngine.h>

CRSEEDLIB_MODULE_CREATOR(StereoCameraVR);

// ************************************************************************************************
spdlog::logger* global_logger = nullptr;

StereoCameraVR::StereoCameraVR(void): crsf::TDynamicModuleInterface(CRMODULE_ID_STRING)
{
    global_logger = m_logger.get();
}

void StereoCameraVR::OnLoad(void)
{
    auto rendering_engine = crsf::TGraphicRenderEngine::GetInstance();

    world_ = std::make_unique<World>(
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
    world_.reset();
}
