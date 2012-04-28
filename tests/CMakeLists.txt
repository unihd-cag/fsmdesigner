cmake_minimum_required (VERSION 2.8)
project (FSMDesigner_Regressions)

# Set version number
set (FSMDesigner_Regressions_VERSION_MAJOR 5)
set (FSMDesigner_Regressions_VERSION_MINOR 1)
set (FSMDesigner_Regressions_VERSION_PATCH 0)


##########################################
# Libraries needed
##########################################

set(CMAKE_MODULE_PATH "${FSMDesigner_Regressions_SOURCE_DIR}/../cpp/macros/")

## Boost test
set (LIBRARIES ${LIBRARIES} boost_unit_test_framework)

## QT4 
find_package(Qt4 COMPONENTS QtCore QtGui QtXml QtSVG QtScript REQUIRED )
include(${QT_USE_FILE})
set (LIBRARIES ${LIBRARIES} ${QT_LIBRARIES})

## FSMDesigner
link_directories(${FSMDesigner_Regressions_SOURCE_DIR}/../cpp/build/)
set (LIBRARIES ${LIBRARIES} core_qxml_lib genverilog_lib gui_lib)

##########################################
# Compilation Parameters
##########################################

## Debug
set(CMAKE_BUILD_TYPE Debug)

## Show compiler command lines
set(CMAKE_VERBOSE_MAKEFILE false)


##########################################
## Output directories
##########################################
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./build/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "./build/")

##########################################
# Sources definitions
##########################################


## Config
configure_file (
  "${FSMDesigner_Regressions_SOURCE_DIR}/include/common/Version.base.h"
  "${FSMDesigner_Regressions_BINARY_DIR}/include/common/Version.h"
 )
 
## Packages definitions ##############
set(PACKAGES 
        common 
        convert 
        core
        genverilog)

## Includes ############################

#### Manual definitions
include_directories("${FSMDesigner_Regressions_SOURCE_DIR}/../cpp/include/${package}")

#### packages
foreach (package ${PACKAGES})
    include_directories("${FSMDesigner_Regressions_SOURCE_DIR}/include/${package}")
    include_directories("${FSMDesigner_Regressions_SOURCE_DIR}/../cpp/include/${package}")
endforeach(package)

## Sources #############################
 
#### Packages
foreach (package ${PACKAGES})
    aux_source_directory(${FSMDesigner_Regressions_SOURCE_DIR}/src/${package} src)
endforeach(package)

 

##########################################
### Regression 	Executables
##########################################

## Global Regressions (includes everybody)
add_executable(fsmDesignerRegressions src/fsmDesignerRegressions.cpp ${src})
target_link_libraries(fsmDesignerRegressions ${LIBRARIES} )





