/**
 * Coexistence Reality Software Framework (CRSF)
 * Copyright (c) Center of Human-centered Interaction for Coexistence. All rights reserved.
 * See the LICENSE.md file for more details.
 */

#include "chic_cam_manager.hpp"

#include <fmt/ostream.h>

#include <render_pipeline/rpcore/globals.hpp>
#include <render_pipeline/rppanda/showbase/showbase.hpp>

#include <crsf/RenderingEngine/TGraphicRenderEngine.h>
#include <crsf/CRModel/TWorld.h>
#include <crsf/CRModel/TPhysicalCamera.h>
#include <crsf/RenderingEngine/GraphicRenderEngine/TTexture.h>
#include <crsf/CoexistenceInterface/TImageMemoryObject.h>
#include <crsf/CREngine/TDynamicModuleManager.h>

#include "utils.hpp"

CHICCamManager::CHICCamManager(rpcore::RenderPipeline& pipeline, const boost::property_tree::ptree& props) :
    RPObject("CHICCamManager"), pipeline_(pipeline), props_(props)
{
    if (!is_available())
        return;

    if (bool SYNC_CAMERA_IMAGE = false)
    {
        //auto world = crsf::TGraphicRenderEngine::GetInstance()->GetWorld();
        //auto cam = world->Find("vstcamera")->GetPtrOf<crsf::TPhysicalCamera>();
        //cam->GetTexture()->GetImageMemoryObject()->AttachImageListener("CHICCamManager::update", [this](crsf::TImageMemoryObject*) {
        //    update(nullptr);
        //});
    }
    else
    {
        add_task([this](const rppanda::FunctionalTask* task) {
            return update(task);
        }, "CHICCamManager::update", -40);
    }

    static const std::string identity_string(
        "1 0 0 0 "
        "0 1 0 0 "
        "0 0 1 0 "
        "0 0 0 1");

    // row major
    cam_to_hmd_[0] = parse_to_mat4(props_.get("camera_calibration.left", identity_string), LMatrix4f::ident_mat());
    cam_to_hmd_[1] = parse_to_mat4(props_.get("camera_calibration.right", identity_string), LMatrix4f::ident_mat());

    // row to column
    cam_to_hmd_[0].transpose_in_place();
    cam_to_hmd_[1].transpose_in_place();

    controller_mat_window_size_ = props_.get<size_t>("camera_calibration.window_size", 1);

    accept("CHICCamManager::inc_window_size", [this](const Event* ev) { ++controller_mat_window_size_; std::cout << controller_mat_window_size_ << std::endl; });
    accept("CHICCamManager::dec_window_size", [this](const Event* ev) { controller_mat_window_size_ = (std::max)(1llu, controller_mat_window_size_ - 1); std::cout << controller_mat_window_size_ << std::endl; });

    accept("CHICCamManager::son_cal", [this](const Event* ev) { apply_son_cal(ev); });
}

CHICCamManager::~CHICCamManager() = default;

bool CHICCamManager::is_available() const
{
    return crsf::TDynamicModuleManager::GetInstance()->IsModuleEnabled("pinhole_camera");
}

AsyncTask::DoneStatus CHICCamManager::update(const rppanda::FunctionalTask* task)
{
    crsf::TGraphicRenderEngine* rendering_engine = crsf::TGraphicRenderEngine::GetInstance();
    crsf::TWorld* cr_world = rendering_engine->GetWorld();

    auto openvr_devices = rpcore::Globals::render.find("openvr_devices");

    auto controller = openvr_devices.find("vr_controller_vive*");

    auto hmd_to_origin = openvr_devices.find("generic_hmd").get_mat();

    hmd_mat_window_.push_back(hmd_to_origin);
    controller_mat_window_.push_back(controller.get_mat());

    if (controller_mat_window_.size() >= controller_mat_window_size_)
    {
        while (controller_mat_window_.size() > controller_mat_window_size_)
        {
            hmd_mat_window_.pop_front();
            controller_mat_window_.pop_front();
        }

        hmd_to_origin = hmd_mat_window_.front();
        hmd_mat_window_.pop_front();
        controller.set_mat(controller_mat_window_.front());
        controller_mat_window_.pop_front();
    }

    cr_world->Find("vstcamera")->SetMatrix(cam_to_hmd_[0] * hmd_to_origin);
    cr_world->Find("vstcamera2")->SetMatrix(cam_to_hmd_[1] * hmd_to_origin);

    auto cam = rpcore::Globals::base->get_cam(0);
    auto leye = cam.find("left_eye");
    auto reye = cam.find("right_eye");

    if (!leye || !reye)
    {
        error("There are no two virtual camera.");
    }
    else
    {
        leye.set_mat(rpcore::Globals::render, cr_world->Find("vstcamera")->GetNodePath().get_mat(rpcore::Globals::render));
        reye.set_mat(rpcore::Globals::render, cr_world->Find("vstcamera2")->GetNodePath().get_mat(rpcore::Globals::render));
    }

    return AsyncTask::DS_cont;
}

void CHICCamManager::apply_son_cal(const Event* ev)
{
    static int eye_index = 0;
    if (ev->get_num_parameters() == 0)
    {
        eye_index = 1 - eye_index;
        return;
    }

    if (ev->get_num_parameters() != 3)
    {
        error("Invalid number of parameters.");
        return;
    }

    double x = ev->get_parameter(0).get_double_value();
    double y = ev->get_parameter(1).get_double_value();
    double z = ev->get_parameter(2).get_double_value();

    cam_to_hmd_[eye_index](3, 0) += x;
    cam_to_hmd_[eye_index](3, 1) += y;
    cam_to_hmd_[eye_index](3, 2) += z;

    info(fmt::format("{} : {}", eye_index == 0 ? "Left" : "Right", cam_to_hmd_[eye_index].get_row3(3)));
}