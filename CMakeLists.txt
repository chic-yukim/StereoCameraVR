cmake_minimum_required(VERSION 3.12)

set(CRMODULE_ID "StereoCameraVR")
project(crmodule_${CRMODULE_ID}
    VERSION 0.1.0
    DESCRIPTION "StereoCameraVR"
    LANGUAGES CXX
)

# === configure ====================================================================================
set_property(GLOBAL PROPERTY USE_FOLDERS ON)    # Project Grouping

# find CRSF SDK
if(NOT CRSF_SDK_DIR)
    string(REPLACE "\\" "/" CRSF_SDK_DIR "$ENV{CRSF_SDK_DIR}")
    message("CRSF SDK: ${CRSF_SDK_DIR}")
endif()
list(APPEND CMAKE_MODULE_PATH "${CRSF_SDK_DIR}/cmake")

set(CRMODULE_INSTALL_DIR "${CRMODULE_ID}")

# === project specific packages ===
include(FindPackages)

find_package(imgui CONFIG REQUIRED)
set_target_properties(imgui::imgui PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE)

render_pipeline_find_plugins("openvr;ar_render")
CRSeedLib_FIND_MODULES("pinhole_camera")
# ==================================================================================================

# === target =======================================================================================
include("${PROJECT_SOURCE_DIR}/files.cmake")
add_library(${PROJECT_NAME} MODULE ${module_sources} ${module_headers})

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /MP /wd4251
        $<$<VERSION_GREATER:${MSVC_VERSION},1800>:/utf-8>
        $<$<VERSION_GREATER:${MSVC_VERSION},1900>:/permissive->

        # note: windows.cmake in vcpkg
        $<$<CONFIG:Release>:/Oi /Gy /Z7>
    )

    set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS_RELWITHDEBINFO    " /INCREMENTAL:NO /OPT:REF /OPT:ICF ")
    set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS_RELEASE           " /DEBUG /INCREMENTAL:NO /OPT:REF /OPT:ICF ")
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
endif()

target_compile_definitions(${PROJECT_NAME}
    PRIVATE CRMODULE_ID_STRING="${CRMODULE_ID}"
    $<$<BOOL:${${PROJECT_NAME}_USE_lidar360}>:SUMMER_CRUSH_USE_LIDAR=1>
)

target_include_directories(${PROJECT_NAME}
    PRIVATE "${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE $<$<NOT:$<BOOL:${Boost_USE_STATIC_LIBS}>>:Boost::dynamic_linking>
    CRSeedLib fmt::fmt-header-only spdlog::spdlog
    imgui::imgui

    rpplugins::openvr rpplugins::ar_render

    crmodule::pinhole_camera
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    FOLDER "MyProject"
    DEBUG_POSTFIX ${CRSeedLib_DEBUG_POSTFIX}
    RELWITHDEBINFO_POSTFIX ${CRSeedLib_RELWITHDEBINFO_POSTFIX}
    VERSION ${PROJECT_VERSION}
)

# configure local debugging for MSVC
if((MSVC) AND (WIN32))
    set(${PROJECT_NAME}_JUNCTION_DIRS "resources" "config")
    configure_build_directory("application" "${${PROJECT_NAME}_JUNCTION_DIRS}")
endif()
# ==================================================================================================

# === install ======================================================================================
install(TARGETS ${PROJECT_NAME} DESTINATION ${CRMODULE_ID})

# install module files.
install(FILES "${PROJECT_SOURCE_DIR}/config.json" DESTINATION ${CRMODULE_ID} CONFIGURATIONS Release)
if(EXISTS "${PROJECT_SOURCE_DIR}/resources")
    install(DIRECTORY "${PROJECT_SOURCE_DIR}/resources" DESTINATION ${CRMODULE_ID} CONFIGURATIONS Release)
endif()

# install application files
if(EXISTS "${PROJECT_SOURCE_DIR}/config")
    install(DIRECTORY "${PROJECT_SOURCE_DIR}/config" DESTINATION ${CRMODULE_ID})
endif()
# ==================================================================================================
