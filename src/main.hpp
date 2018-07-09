#pragma once

#include <pointerTo.h>

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
    PT(World) world_;
};
