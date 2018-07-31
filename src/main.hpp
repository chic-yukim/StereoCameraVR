#pragma once

#include <crsf/CRAPI/TDynamicModuleInterface.h>

class World;

class StereoCameraVR : public crsf::TDynamicModuleInterface
{
public:
    StereoCameraVR();

    void OnLoad() override;
    void OnStart() override;
    void OnExit() override;

private:
    std::unique_ptr<World> world_;
};
