# this one is important
INCLUDE(CMakeForceCompiler)
SET(CMAKE_SYSTEM_NAME Windows)
SET(CMAKE_SYSTEM_PROCESSOR x86_64-w64)

#SET(CMAKE_C_COMPILER /usr/bin/${CMAKE_SYSTEM_PROCESSOR}-mingw32-gcc)
#SET(CMAKE_CXX_COMPILER /usr/bin/${CMAKE_SYSTEM_PROCESSOR}-mingw32-g++)

SET(CMAKE_RC_COMPILER ${CMAKE_SYSTEM_PROCESSOR}-mingw32-windres)
SET(CMAKE_C_COMPILER ${CMAKE_SYSTEM_PROCESSOR}-mingw32-gcc)
SET(CMAKE_CXX_COMPILER ${CMAKE_SYSTEM_PROCESSOR}-mingw32-g++)

# where is the target environment
SET(ENV{QTDIR} /usr/${CMAKE_SYSTEM_PROCESSOR}-mingw32/)
SET(CMAKE_FIND_ROOT_PATH /usr/${CMAKE_SYSTEM_PROCESSOR}-mingw32)

#include_directories(/usr/${CMAKE_SYSTEM_PROCESSOR}-mingw32/include)
#include_directories(/usr/${CMAKE_SYSTEM_PROCESSOR}-mingw32/include/qt4)
#link_directories(/usr/${CMAKE_SYSTEM_PROCESSOR}-mingw32/lib)

# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


#INCLUDE(CMakeForceCompiler)