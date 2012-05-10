cmake_minimum_required (VERSION 2.8)
project (FSMDesigner)

# Set version number
set (FSMDesigner_VERSION_MAJOR 5)
set (FSMDesigner_VERSION_MINOR 2)
set (FSMDesigner_VERSION_PATCH 0)
  
##########################################
# Libraries needed
##########################################
   
set(CMAKE_MODULE_PATH "./macros/")

## QT4 ###################################
find_package(Qt4 COMPONENTS QtCore QtGui QtXml QtSVG QtScript REQUIRED )
include(${QT_USE_FILE})
 

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

IF (CMAKE_CROSSCOMPILING)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./build-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "./build-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/")
ELSE (CMAKE_CROSSCOMPILING)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./build/")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "./build/")
ENDIF (CMAKE_CROSSCOMPILING)

##########################################
# Sources definitions
##########################################

## Variables
set(GENERATED_PATH ${FSMDesigner_SOURCE_DIR}/CMakeFiles/generated/)

## Config
configure_file (
  "${FSMDesigner_SOURCE_DIR}/include/core/Utils.base.h"
  "${FSMDesigner_BINARY_DIR}/include/core/Utils.h"
 )
 
## GUI Packages definitions ##############
set(GUI_PACKAGES 
        gui
        gui/actions 
        gui/common 
        gui/fsm 
        gui/input 
        gui/items 
        gui/mergedGUI 
        gui/originGUI 
        gui/output
        gui/project  
        gui/resources 
        gui/settings 
        gui/scene 
        gui/state 
        gui/table 
        gui/tabbedGUI 
        gui/transitions 
        gui/undo 
        gui/verify)
  

## Includes ############################

#### Manual definitions
include_directories(
		"${FSMDesigner_SOURCE_DIR}/include"
		"${FSMDesigner_SOURCE_DIR}/include/core/"
		"${FSMDesigner_SOURCE_DIR}/include/core-qxml/"
		"${FSMDesigner_SOURCE_DIR}/include/genverilog/"
		"${FSMDesigner_SOURCE_DIR}/include/generate/"
		"${FSMDesigner_SOURCE_DIR}/include/verification/"
		"${GENERATED_PATH}")


#### GUI packages
foreach (package ${GUI_PACKAGES})
    include_directories("${FSMDesigner_SOURCE_DIR}/include/${package}")
endforeach(package)

## Sources #############################

#### Manual definitions
aux_source_directory(${FSMDesigner_SOURCE_DIR}/src/core/                core_src)
aux_source_directory(${FSMDesigner_SOURCE_DIR}/src/generate/            core_src)
aux_source_directory(${FSMDesigner_SOURCE_DIR}/src/core-qxml/           core-qxml_src)
aux_source_directory(${FSMDesigner_SOURCE_DIR}/src/genverilog/          genverilog_src)
aux_source_directory(${FSMDesigner_SOURCE_DIR}/src/verification/        verification_src)


#### GUI packages
foreach (package ${GUI_PACKAGES})
    aux_source_directory(${FSMDesigner_SOURCE_DIR}/src/${package}/ gui_src)
endforeach(package)


## Core library QT QXML
#####################
add_library(core_qxml_lib SHARED ${core_src} ${core-qxml_src})
target_link_libraries(core_qxml_lib ${QT_LIBRARIES})

## Verification  library
###########################
add_library(verification_lib SHARED ${verification_src})
target_link_libraries(verification_lib core_qxml_lib ${QT_LIBRARIES})

## Genverilog library
###########################
add_library(genverilog_lib SHARED ${genverilog_src})
target_link_libraries(genverilog_lib core_qxml_lib verification_lib)

## FSMDesigner main GUI
###########################

#### MOC
set(QT4_WRAP_CPP_OUT_DIR ${GENERATED_PATH})

foreach (package ${GUI_PACKAGES})
    file(GLOB package_moc_src ${FSMDesigner_SOURCE_DIR}/include/${package}/*.h )
    qt4_wrap_cpp(gui_moced_src ${package_moc_src})
endforeach(package)
 

#### UI
set(QT4_WRAP_UI_OUT_DIR ${GENERATED_PATH})

foreach (package ${GUI_PACKAGES})
    file(GLOB package_ui_src ${FSMDesigner_SOURCE_DIR}/include/${package}/*.ui )
    list(APPEND gui_ui_src ${package_ui_src})
endforeach(package)

qt4_wrap_ui(gui_ui_headers ${gui_ui_src} -h)

#### Resources
set(QT4_ADD_RESOURCES_OUT_DIR ${GENERATED_PATH})

file(GLOB gui_resources_src ${FSMDesigner_SOURCE_DIR}/src/gui/resources/*.qrc )
QT4_ADD_RESOURCES(gui_ui_resources ${gui_resources_src})


#### Custom target to put together generated files
add_custom_target(gui_generated SOURCES ${gui_ui_src} ${gui_ui_resources} ${gui_moced_src})

#### Create and link library
add_library(gui_lib SHARED ${gui_src} ${gui_ui_src} ${gui_ui_resources} ${gui_moced_src})
target_link_libraries(gui_lib genverilog_lib core_qxml_lib verification_lib ${QT_LIBRARIES})

##########################################
# Gui Executable
##########################################
IF(WIN32)
	add_executable(fsmdesigner WIN32 src/fsmdesigner.cpp  )
ELSE (WIN32)
	add_executable(fsmdesigner src/fsmdesigner.cpp  )
ENDIF(WIN32)
target_link_libraries(fsmdesigner gui_lib)

##########################################
### Genverilog 	Executable
##########################################
add_executable(fsmdesignerGenVerilog src/fsmdesignerGenVerilog.cpp)
target_link_libraries(fsmdesignerGenVerilog genverilog_lib )



##########################################
# Normal Local install
# Installs to CMAKE_INSTALL_PREFIX
##########################################
SET(TARGETS_TO_INSTALL fsmdesignerGenVerilog fsmdesigner gui_lib genverilog_lib core_qxml_lib verification_lib)	




## Install binary and libraries
install(TARGETS ${TARGETS_TO_INSTALL} 
        RUNTIME DESTINATION bin COMPONENT runtime
        LIBRARY DESTINATION lib COMPONENT runtime)


	    
## Install includes   
install(DIRECTORY include/ 
		DESTINATION include/fsmdesigner
		COMPONENT headers
        FILES_MATCHING PATTERN "*.h")

##########################################
## Package installers
##########################################

# Define version of package
SET(CPACK_PACKAGE_VERSION_MAJOR ${FSMDesigner_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${FSMDesigner_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${FSMDesigner_VERSION_PATCH})
SET(CPACK_RESOURCE_FILE_LICENSE "${FSMDesigner_SOURCE_DIR}/Copyright.txt")

## Create an archive
#SET(CPACK_GENERATOR TBZ2 ZIP)


## Debian ##########################################
IF(NOT WIN32)

    ## Determine revision
    SET(revisionsCount 0)
    execute_process(
        COMMAND git log --all --pretty=oneline
        COMMAND wc -l
        OUTPUT_VARIABLE revisionsCount
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    
    # Determine Architecture
    execute_process(
        COMMAND dpkg --print-architecture
        OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE
        OUTPUT_STRIP_TRAILING_WHITESPACE)
        
    ## Append to patch version
    SET(CPACK_PACKAGE_VERSION_PATCH ${FSMDesigner_VERSION_PATCH}~${revisionsCount})
	SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "richard.leys@ziti.uni-heidelberg.de")
	SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt4-xml (>= 4.6)")
	SET(CPACK_GENERATOR ${CPACK_GENERATOR} DEB)
ENDIF(NOT WIN32)

## Windows installer ###############################
IF(WIN32)

     ## Package QT  Libraries DLLs
     FOREACH(qtlib ${QT_LIBRARIES})
     	string(REGEX REPLACE
     				"lib([a-zA-Z0-9]+)\\.a"
     				"\\1.dll"
     				resLib
     				${qtlib})
     	message(STATUS "Packaging ${resLib}")
     	INSTALL(FILES ${resLib}
    	    DESTINATION bin
    	    COMPONENT runtime)
     ENDFOREACH(qtlib ${QT_LIBRARIES})
    
     ## Runtime component is required
     set(CPACK_COMPONENT_RUNTIME_REQUIRED true)
 
    ## Executable
    SET(CPACK_PACKAGE_EXECUTABLES "fsmdesigner" "FSMDesigner")

    ## Set Components
    SET(CPACK_COMPONENTS_ALL runtime headers)
    SET(CPACK_GENERATOR ${CPACK_GENERATOR} NSIS)

ENDIF(WIN32)

## Include CPAck ##########################################
INCLUDE(CPack)

##########################################
# Documentation Generation
##########################################
INCLUDE(Documentation)
option(BUILD_DOCUMENTATION ON)
option(DOCUMENTATION_HTML_HELP ON)


