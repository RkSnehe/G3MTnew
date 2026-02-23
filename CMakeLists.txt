cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(Tutorial)

find_package(Geant4 REQUIRED ui_all vis_all)

add_executable(sim
    sim.cc
    src/DetectorConstruction.cc
    src/ActionInitialization.cc
    src/PrimaryGenerator.cc
    src/PhysicsList.cc
    src/SensitiveDetector.cc
    src/RunAction.cc
)

target_include_directories(sim PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(sim PRIVATE
    ${Geant4_LIBRARIES}
)

file(GLOB MACRO_FILES "macros/*.mac")
file(COPY ${MACRO_FILES} DESTINATION ${CMAKE_BINARY_DIR})
