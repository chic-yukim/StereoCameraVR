/**
 * Coexistence Reality Software Framework (CRSF)
 * Copyright (c) Center of Human-centered Interaction for Coexistence. All rights reserved.
 * See the LICENSE.md file for more details.
 */

#pragma once

#include <render_pipeline/rppanda/showbase/direct_object.hpp>

#include <openvr.h>

namespace rpcore {
class RenderPipeline;
}

namespace rpplugins {
class OpenVRPlugin;
}

class OpenVRManager : public rppanda::DirectObject
{
public:
    OpenVRManager(rpcore::RenderPipeline& pipeline);
    virtual ~OpenVRManager();

    ALLOC_DELETED_CHAIN(OpenVRManager);

    bool is_available() const;

    rpplugins::OpenVRPlugin* get_plugin() const;

    void print_serials();

private:
    void process_controller_event();
    void update_texts();

    rpcore::RenderPipeline& pipeline_;
    rpplugins::OpenVRPlugin* openvr_plugin_ = nullptr;

    uint64_t controller_last_states_[vr::k_unMaxTrackedDeviceCount];
};

inline bool OpenVRManager::is_available() const
{
    return openvr_plugin_ != nullptr;
}

inline rpplugins::OpenVRPlugin* OpenVRManager::get_plugin() const
{
    return openvr_plugin_;

}
