cmake_minimum_required(VERSION 3.16)
project(microelectronics_sim)

# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake to switch this off
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
include(${Geant4_USE_FILE})

find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/include)

# Locate sources and headers for this project
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Add the executable, and link it to the Geant4 libraries
add_executable(microelectronics_sim microelectronics_sim.cc ${sources} ${headers})
target_link_libraries(microelectronics_sim ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
target_link_options(microelectronics_sim PRIVATE -Wl,-rpath,/opt/root/lib -Wl,-rpath-link,/opt/root/lib)

# Copy all scripts to the build directory (for ease of execution)
foreach(_script init_vis.mac run.mac vis.mac)
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
  )
endforeach()
