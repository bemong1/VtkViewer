cmake_minimum_required(VERSION 2.8)

# set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(LIST_MANAGER ./third_party/BemongLibrary/DataStruct/ListManager/ListManagerMutex.cpp)

project(Vtk_Viewer)

#add_library(ListManager ${LIST_MANAGER})
add_executable(Vtk_Viewer main.cpp VtkViewer.cpp ${LIST_MANAGER})

# set pkg-config
SET ( CMAKE_C_COMPILER "g++" )
find_package(PkgConfig REQUIRED)

pkg_check_modules(OPENCV REQUIRED IMPORTED_TARGET opencv4)

target_include_directories(Vtk_Viewer PUBLIC PkgConfig::OPENCV)
target_link_libraries(Vtk_Viewer PUBLIC PkgConfig::OPENCV)
target_compile_options(Vtk_Viewer PUBLIC ${OPENCV_CFLAGS_OTHER})

