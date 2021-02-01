cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(gamma_spectrometer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

find_package(G4NumpyAnalysis 2.0.0 REQUIRED)

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include ${Geant4_INCLUDE_DIRS} ${G4NumpyAnalysis_INCLUDE_DIRS})

IF(VERBOSE)
add_compile_definitions(VERBOSE)
endif(VERBOSE)

file(GLOB sources ./src/*.cc)
file(GLOB headers ./include/*.hh)

set(SCRIPTS
  init_vis.mac
  vis.mac
  timeout
  run1.mac
  run2.mac
  )

foreach(_script ${SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

set(DIRS
    CsI_data
    )

foreach(_dir ${DIRS})
    file(COPY ${PROJECT_SOURCE_DIR}/${_dir} DESTINATION ${PROJECT_BINARY_DIR})
endforeach()

add_executable(gamma_spectrometer gamma_spectrometer.cc ${sources} ${headers})
target_link_libraries(gamma_spectrometer ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} ${G4NumpyAnalysis_LIBRARIES})
