cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

set(name g4had)
project(${name})

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()
add_definitions(-DG4ANALYSIS_USE) 

include(${Geant4_USE_FILE})

find_package(ROOT REQUIRED)
if(NOT ROOT_FOUND)
  message(STATUS "g4had: ROOT package not found. --> g4had disabled")
  return()
endif()

message(STATUS "${ROOT_USE_FILE}=${${ROOT_USE_FILE}}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIR})

link_directories( ${ROOT_LIBRARY_DIR} )


#--ProdTuple_t------------------------------------------------------------------------------------
ROOT_GENERATE_DICTIONARY( "${CMAKE_CURRENT_SOURCE_DIR}/src/ProdTuple_tDict"
                          "${CMAKE_CURRENT_SOURCE_DIR}/include/ProdTuple_t.hh"
                          "${CMAKE_CURRENT_SOURCE_DIR}/include/TrackInfo_t.hh"
                          LINKDEF  
                          "${CMAKE_CURRENT_SOURCE_DIR}/include/Linkdef.h")
#                          OPTIONS -p)

file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc )

list( APPEND sources src/ProdTuple_tDict.cxx )

add_library(CrossSection SHARED  ${sources} )

set(libsuffix .so)

set(ROOT_LIBRARY_PROPERTIES ${ROOT_LIBRARY_PROPERTIES}
      SUFFIX ${libsuffix})

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


add_executable(${name} ${name}.cc)

target_link_libraries(${name}  CrossSection   ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
#target_link_libraries( ${name}     
#                       ${Geant4_LIBRARIES}  
#                       ${ROOT_LIBRARIES}
#                     )

install (TARGETS ${name} DESTINATION bin)
install (TARGETS CrossSection DESTINATION lib)
install (FILES  DESTINATION bin)
