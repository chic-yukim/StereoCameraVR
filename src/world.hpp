#pragma once

#include <asyncTask.h>

#include <render_pipeline/rppanda/showbase/direct_object.hpp>
#include <render_pipeline/rpcore/rpobject.hpp>
#include <render_pipeline/rppanda/actor/actor.hpp>
#include <render_pipeline/rppanda/interval/actor_interval.hpp>
#include <render_pipeline/rppanda/interval/lerp_interval.hpp>

#include <boost/property_tree/ptree.hpp>

class OpenVRManager;
class CHICCamManager;

namespace rppanda {
class FunctionalTask;
}

namespace rpcore {
class RenderPipeline;
class MovementController;
}

class World : public rppanda::DirectObject, public rpcore::RPObject
{
public:
    World(rpcore::RenderPipeline& pipeline, const boost::property_tree::ptree& props);
    virtual ~World();

    void start();

private:
    void setup_event();

    const boost::property_tree::ptree& props_;

    rpcore::RenderPipeline& pipeline_;
    std::unique_ptr<rpcore::MovementController> controller_;

    std::unique_ptr<OpenVRManager> openvr_manager_;
    std::unique_ptr<CHICCamManager> chic_cam_manager_;
};
