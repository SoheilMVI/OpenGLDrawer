cmake_minimum_required(VERSION 3.16)
set(proj_name OpenGLDrawer)
project(${proj_name} LANGUAGES CXX)

# Find required Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Gui Concurrent OpenGL Quick)

# Standard Qt project setup
qt_standard_project_setup()

# Collect source files
file(GLOB SOURCES
    src/*.cpp
    src/BaseEngine/*.cpp
    src/Components/*.cpp
    src/Windows/*.cpp
)

# Collect header files
file(GLOB HEADERS
    include/*.h
    include/BaseEngine/*.h
    include/Components/*.h
    include/Windows/*.h
)

# Collect shader files
file(GLOB SHADERS
    Shaders/v110/*.frag
    Shaders/v110/*.vert
    Shaders/v330/*.frag
    Shaders/v330/*.vert
    Shaders/v440/*.frag
    Shaders/v440/*.vert
)

# Define the executable
qt_add_executable(${proj_name}
    ${SOURCES}
    ${HEADERS}
    ${SHADERS}
)

# Set target properties
set_target_properties(${proj_name} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# Link Qt libraries
target_link_libraries(${proj_name} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::OpenGL
    Qt6::Quick
    Qt6::Concurrent
)

# Specify include directories
target_include_directories(${proj_name} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/BaseEngine
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Components
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Windows
)

target_compile_definitions(${proj_name} PRIVATE
    PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

# Install the executable
install(TARGETS ${proj_name}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Generate deployment script
qt_generate_deploy_app_script(
    TARGET ${proj_name}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

# Add QML module
qt_add_qml_module(${proj_name}
    URI OpenGLUnderQML
    QML_FILES Main.qml
    NO_RESOURCE_TARGET_PATH
)

# Install the deployment script
install(SCRIPT ${deploy_script})
