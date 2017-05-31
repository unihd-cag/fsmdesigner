#/usr/bin/env bash
rm -rf build
rm -f CMakeCache.txt
rm -rf CMakeFiles
rm -f cmake_install.cmake
rm -f CPackConfig.cmake
rm -f CPackSourceConfig.cmake
rm -f CTestTestfile.cmake
rm -f CMakeLists.txt.user
rm -f FSMDesigner.cbp
rm -f Makefile
find . -name *moc_* | xargs rm -f
