cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0060 NEW)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3 -g")

project( proximity )


find_package(OpenCV REQUIRED)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

find_package(yaml-cpp REQUIRED)
message(STATUS "YAML-CPP library status:")
message(STATUS "    include path: ${YAML_CPP_INCLUDE_DIR}")
message(STATUS "    libraries: ${YAML_CPP_LIBRARIES}")

find_package(Eigen3 REQUIRED)
find_package(Boost COMPONENTS system)
find_package(TensorflowCC REQUIRED)
find_package(CUDA)


add_definitions(-Wuninitialized)
add_definitions(-Wreturn-type)
add_definitions(-Wsign-compare)
add_definitions(-Wuninitialized)
add_definitions(-Wunused-function)
add_definitions(-Wunused-label)    
add_definitions(-Wunused-value) 
add_definitions(-Wunused-variable)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX14)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")
elseif(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
elseif(COMPILER_SUPPORTS_CXX0X)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pthread")
else()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories( ${EIGEN3_INCLUDE_DIR} )
include_directories(${Boost_INCLUDE_DIRS})

set(LIBRARY_OUTPUT_PATH ../lib)
set(EXECUTABLE_OUTPUT_PATH ../bin)

include(${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CMakeLists.txt)
utils()

include(${CMAKE_CURRENT_SOURCE_DIR}/src/config/CMakeLists.txt)
config()

include(${CMAKE_CURRENT_SOURCE_DIR}/src/object_detection/CMakeLists.txt)
object_detection()

include(${CMAKE_CURRENT_SOURCE_DIR}/src/homography/CMakeLists.txt)
homography()

include(${CMAKE_CURRENT_SOURCE_DIR}/src/features/CMakeLists.txt)
features()

include(${CMAKE_CURRENT_SOURCE_DIR}/src/matching/CMakeLists.txt)
matchingtracks()

include(${CMAKE_CURRENT_SOURCE_DIR}/src/thirdPart/CMakeLists.txt)
thirdpartutils()

include(${CMAKE_CURRENT_SOURCE_DIR}/src/segmentation/CMakeLists.txt)
segmentationlib()

include(${CMAKE_CURRENT_SOURCE_DIR}/src/apps/CMakeLists.txt)
applications()
