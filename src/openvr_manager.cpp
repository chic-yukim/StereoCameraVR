/**
 * Coexistence Reality Software Framework (CRSF)
 * Copyright (c) Center of Human-centered Interaction for Coexistence. All rights reserved.
 * See the LICENSE.md file for more details.
 */

#include "openvr_manager.hpp"

#include <fmt/format.h>

#include <render_pipeline/rpcore/render_pipeline.hpp>
#include <render_pipeline/rpcore/pluginbase/manager.hpp>
#include <render_pipeline/rppanda/task/task_manager.hpp>
#include <render_pipeline/rpcore/globals.hpp>
#include <render_pipeline/rppanda/showbase/showbase.hpp>

#include <openvr_plugin.hpp>

#include <crsf/CRModel/TWorldObject.h>

OpenVRManager::OpenVRManager(rpcore::RenderPipeline& pipeline) : pipeline_(pipeline)
{
    if (!pipeline_.get_plugin_mgr()->is_plugin_enabled("openvr"))
        return;

    openvr_plugin_ = static_cast<rpplugins::OpenVRPlugin*>(pipeline_.get_plugin_mgr()->get_instance("openvr")->downcast());

    add_task([this](rppanda::FunctionalTask*) {
        process_controller_event();
        return AsyncTask::DoneStatus::DS_cont;
    }, "OpenVRManager::process_controller_event");
}

OpenVRManager::~OpenVRManager()
{
    remove_all_tasks();
}

void OpenVRManager::print_serials()
{
    openvr_plugin_->get_device_node_group().ls();
}

void OpenVRManager::process_controller_event(void)
{
    auto vr_system = openvr_plugin_->get_vr_system();

    // Process SteamVR controller state
    for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++)
    {
        if (openvr_plugin_->get_tracked_device_class(unDevice) != vr::TrackedDeviceClass_Controller)
            continue;

        vr::VRControllerState_t state;
        if (!vr_system->GetControllerState(unDevice, &state, sizeof(state)))
            continue;

        const EventParameter ev_param(static_cast<int>(unDevice));

        for (int id = vr::EVRButtonId::k_EButton_System; id < vr::EVRButtonId::k_EButton_Max; ++id)
        {
            const auto button_mask = vr::ButtonMaskFromId(static_cast<vr::EVRButtonId>(id));
            const bool pressed = (button_mask & state.ulButtonPressed) != 0;
            const bool last_pressed = (button_mask & controller_last_states_[unDevice]) != 0;

            if (pressed && !last_pressed)
            {
                const char* name = vr_system->GetButtonIdNameFromEnum(static_cast<vr::EVRButtonId>(id));

                rppanda::Messenger::get_global_instance()->send(
                    std::string("openvr::") + name, ev_param);
            }
            else if (!pressed && last_pressed)
            {
                const char* name = vr_system->GetButtonIdNameFromEnum(static_cast<vr::EVRButtonId>(id));

                rppanda::Messenger::get_global_instance()->send(
                    std::string("openvr::") + name + "-up", ev_param);
            }
        }

        controller_last_states_[unDevice] = state.ulButtonPressed;
    }
}
