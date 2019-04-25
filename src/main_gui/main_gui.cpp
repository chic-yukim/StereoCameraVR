#include "main_gui/main_gui.hpp"

#include <imgui.h>

#include <fmt/format.h>

#include "chic_cam_manager.hpp"
#include "main.hpp"

MainGUI::MainGUI(MainApp& app) : app_(app)
{
    rppanda::Messenger::get_global_instance()->send(
        "imgui-setup-context",
        EventParameter(new rppanda::FunctionalTask([this](rppanda::FunctionalTask* task) {
            ImGui::SetCurrentContext(std::static_pointer_cast<ImGuiContext>(task->get_user_data()).get());
            accept("imgui-new-frame", [this](const Event*) { on_imgui_new_frame(); });
            return AsyncTask::DS_done;
        }, "MainApp::setup-imgui"))
    );
}

MainGUI::~MainGUI() = default;

void MainGUI::on_imgui_new_frame()
{
    static bool window = true;

    ImGui::Begin("StereoCameraVR", &window);

    ui_chic_cam_manager();

    ImGui::End();
}

void MainGUI::ui_chic_cam_manager()
{
    if (!app_.chic_cam_manager_)
        return;

    if (!ImGui::CollapsingHeader("CHIC Camera"))
        return;

    static float delta = 0.01f;
    ImGui::InputFloat("Son-cal Delta", &delta);

    if (ImGui::Button("+X"))
    {
        rppanda::Messenger::get_global_instance()->send(
            "CHICCamManager::son_cal",
            EventParameter(delta),
            EventParameter(0.0),
            EventParameter(0.0),
            true
        );
    }

    ImGui::SameLine();

    if (ImGui::Button("-X"))
    {
        rppanda::Messenger::get_global_instance()->send(
            "CHICCamManager::son_cal",
            EventParameter(-delta),
            EventParameter(0.0),
            EventParameter(0.0),
            true
        );
    }

    ImGui::SameLine();

    if (ImGui::Button("+Y"))
    {
        rppanda::Messenger::get_global_instance()->send(
            "CHICCamManager::son_cal",
            EventParameter(0.0),
            EventParameter(delta),
            EventParameter(0.0),
            true
        );
    }

    ImGui::SameLine();

    if (ImGui::Button("-Y"))
    {
        rppanda::Messenger::get_global_instance()->send(
            "CHICCamManager::son_cal",
            EventParameter(0.0),
            EventParameter(-delta),
            EventParameter(0.0),
            true
        );
    }

    ImGui::SameLine();

    if (ImGui::Button("+Z"))
    {
        rppanda::Messenger::get_global_instance()->send(
            "CHICCamManager::son_cal",
            EventParameter(0.0),
            EventParameter(0.0),
            EventParameter(delta),
            true
        );
    }

    ImGui::SameLine();

    if (ImGui::Button("-Z"))
    {
        rppanda::Messenger::get_global_instance()->send(
            "CHICCamManager::son_cal",
            EventParameter(0.0),
            EventParameter(0.0),
            EventParameter(-delta),
            true
        );
    }
}
