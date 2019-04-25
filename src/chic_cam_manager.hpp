/**
 * Coexistence Reality Software Framework (CRSF)
 * Copyright (c) Center of Human-centered Interaction for Coexistence. All rights reserved.
 * See the LICENSE.md file for more details.
 */

#pragma once

#include <asyncTask.h>

#include <boost/property_tree/ptree.hpp>

#include <render_pipeline/rppanda/gui/onscreen_text.hpp>
#include <render_pipeline/rppanda/showbase/direct_object.hpp>
#include <render_pipeline/rpcore/rpobject.hpp>

namespace rppanda {
class FunctionalTask;
}

namespace rpcore {
class RenderPipeline;
}

namespace crsf {
class TPhysicalCamera;
}

class PinholeCameraModule;

class CHICCamManager : public rppanda::DirectObject, public rpcore::RPObject
{
public:
    CHICCamManager(rpcore::RenderPipeline& pipeline, const boost::property_tree::ptree& props);
    virtual ~CHICCamManager();

    bool is_available() const;

private:
    AsyncTask::DoneStatus update(const rppanda::FunctionalTask* task);

    void apply_son_cal(const Event* ev);

    rpcore::RenderPipeline& pipeline_;
    const boost::property_tree::ptree& props_;
    PinholeCameraModule* pinhole_camera_module_ = nullptr;

    crsf::TPhysicalCamera* cam_left_;
    crsf::TPhysicalCamera* cam_right_;

    // camera space to HMD space
    LMatrix4f cam_to_hmd_[2];

    size_t controller_mat_window_size_ = 1;
    std::deque<LMatrix4f> hmd_mat_window_;
    std::deque<LMatrix4f> controller_mat_window_;
};
