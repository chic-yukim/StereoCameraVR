
set(header_include
)

# grouping
source_group("include" FILES ${header_include})

set(module_headers
    ${header_include}
)

set(source_src_main_gui
    "${PROJECT_SOURCE_DIR}/src/main_gui/main_gui.cpp"
    "${PROJECT_SOURCE_DIR}/src/main_gui/main_gui.hpp"
)

set(source_src
    "${PROJECT_SOURCE_DIR}/src/chic_cam_manager.cpp"
    "${PROJECT_SOURCE_DIR}/src/chic_cam_manager.hpp"
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/src/main.hpp"
    "${PROJECT_SOURCE_DIR}/src/openvr_manager.cpp"
    "${PROJECT_SOURCE_DIR}/src/openvr_manager.hpp"
    "${PROJECT_SOURCE_DIR}/src/utils.cpp"
    "${PROJECT_SOURCE_DIR}/src/utils.hpp"
)

# grouping
source_group("src\\main_gui\\imgui" FILES ${source_src_main_gui_imgui})
source_group("src\\main_gui" FILES ${source_src_main_gui})
source_group("src" FILES ${source_src})

set(module_sources
    ${source_src_main_gui}
    ${source_src}
)
