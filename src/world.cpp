#include "world.hpp"

#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

#include <render_pipeline/rppanda/showbase/showbase.hpp>
#include <render_pipeline/rpcore/globals.hpp>
#include <render_pipeline/rpcore/render_pipeline.hpp>
#include <render_pipeline/rpcore/util/movement_controller.hpp>
#include <render_pipeline/rpcore/pluginbase/day_manager.hpp>

#include "openvr_manager.hpp"
#include "chic_cam_manager.hpp"
#include "utils.hpp"

World::World(rpcore::RenderPipeline& pipeline, const boost::property_tree::ptree& props):
    RPObject("World"), pipeline_(pipeline), props_(props)
{
    pipeline_.get_daytime_mgr()->set_time("10:30");
}

World::~World()
{
    remove_all_tasks();
}

void World::start()
{
    openvr_manager_ = new OpenVRManager(pipeline_);
    if (!openvr_manager_->is_available())
    {
        openvr_manager_.clear();
    }

    chic_cam_manager_ = new CHICCamManager(pipeline_, props_);
    if (!chic_cam_manager_->is_available())
        chic_cam_manager_.clear();

    setup_event();
}

void World::setup_event()
{
    if (!openvr_manager_)
    {
        // Init movement controller
        controller_ = std::make_unique<rpcore::MovementController>(rpcore::Globals::base);
        controller_->set_initial_position_hpr(
            LVecBase3f(0, -10, 5),
            LVecBase3f(0, 0, 0));
        controller_->setup();
    }
}
