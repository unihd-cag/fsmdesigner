cmake_minimum_required (VERSION 2.8)
project (FSMDesigner)

# Set version number
#########################
set (FSMDesigner_VERSION_MAJOR 5)
set (FSMDesigner_VERSION_MINOR 2)
set (FSMDesigner_VERSION_PATCH 0)

### Determine revision
SET(revisionsCount 0)
IF(WIN32)
    execute_process(
        COMMAND git-get-revision.bat
        OUTPUT_VARIABLE revisionsCount
        OUTPUT_STRIP_TRAILING_WHITESPACE)
ELSE(WIN32)
    execute_process(
        COMMAND git log --all --pretty=oneline
        COMMAND wc -l
        OUTPUT_VARIABLE revisionsCount
        OUTPUT_STRIP_TRAILING_WHITESPACE)
ENDIF(WIN32)

## Add to Patch Version
set (FSMDesigner_VERSION_PATCH ${FSMDesigner_VERSION_PATCH}~${revisionsCount})
message(STATUS "Patch version set from GIT to: ${revisionsCount}")
  
  




## Debian ##########################################
IF(NOT WIN32)
  
    
    # Determine Architecture
    execute_process(
        COMMAND dpkg --print-architecture
        OUTPUT_VARIABLE arch
        OUTPUT_STRIP_TRAILING_WHITESPACE)
		
    ## Append Arch to patch version
    SET(CPACK_PACKAGE_VERSION_PATCH ${CPACK_PACKAGE_VERSION_PATCH}-${arch})

    set (CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${arch})
        
    ## Append Architecture to patch version
    SET(CPACK_PACKAGE_VERSION_PATCH ${FSMDesigner_VERSION_PATCH}-${arch})
    SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "richard.leys@ziti.uni-heidelberg.de")
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt4-xml (>= 4.6)")
    SET(CPACK_GENERATOR ${CPACK_GENERATOR} DEB)


ENDIF(NOT WIN32)




