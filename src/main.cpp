#include "main.hpp"

#include <spdlog/logger.h>

#include <render_pipeline/rppanda/showbase/showbase.hpp>
#include <render_pipeline/rpcore/globals.hpp>
#include <render_pipeline/rpcore/render_pipeline.hpp>
#include <render_pipeline/rpcore/util/movement_controller.hpp>
#include <render_pipeline/rpcore/pluginbase/day_manager.hpp>

#include <crsf/RenderingEngine/TGraphicRenderEngine.h>

#include "openvr_manager.hpp"
#include "chic_cam_manager.hpp"
#include "utils.hpp"

CRSEEDLIB_MODULE_CREATOR(MainApp);

// ************************************************************************************************
spdlog::logger* global_logger = nullptr;

MainApp::MainApp(void): crsf::TDynamicModuleInterface(CRMODULE_ID_STRING)
{
    global_logger = m_logger.get();

    rendering_engine_ = crsf::TGraphicRenderEngine::GetInstance();
    pipeline_ = rendering_engine_->GetRenderPipeline();
}

MainApp::~MainApp() = default;

void MainApp::OnLoad(void)
{
    pipeline_->get_daytime_mgr()->set_time("10:30");
}

void MainApp::OnStart(void)
{
    auto rendering_engine = crsf::TGraphicRenderEngine::GetInstance();

    rendering_engine->SetWindowTitle(CRMODULE_ID_STRING);

    openvr_manager_ = std::make_unique<OpenVRManager>(*pipeline_);
    if (!openvr_manager_->is_available())
    {
        openvr_manager_.reset();

        rendering_engine_->EnableControl();
        rendering_engine_->SetControllerInitialPosHpr(
            LVecBase3(0.0f, -2.2f, 1.0f),
            LVecBase3(0.0f, -25.0f, 0.0f));
        rendering_engine_->ResetControllerInitial();
    }

    chic_cam_manager_ = std::make_unique<CHICCamManager>(*pipeline_, m_property);
    if (!chic_cam_manager_->is_available())
        chic_cam_manager_.reset();
}

void MainApp::OnExit(void)
{
    remove_all_tasks();

    chic_cam_manager_.reset();
    openvr_manager_.reset();
}
