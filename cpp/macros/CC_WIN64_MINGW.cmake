# this one is important
SET(CMAKE_SYSTEM_NAME Windows)
SET(CMAKE_SYSTEM_PROCESSOR amd64)

SET(CMAKE_C_COMPILER /usr/bin/amd64-mingw32msvc-gcc)
SET(CMAKE_CXX_COMPILER /usr/bin/amd64-mingw32msvc-gcc)

# where is the target environment
SET(CMAKE_FIND_ROOT_PATH  /usr/amd64-mingw32msvc/ $ENV{QTDIR})

# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


