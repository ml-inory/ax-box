cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(ax-box CXX)

set(3RDLIB_ROOT ${CMAKE_SOURCE_DIR}/3rdlibs)

include_directories(${3RDLIB_ROOT}/jsoncpp/include)
include_directories(${3RDLIB_ROOT}/glog/include)
include_directories(${3RDLIB_ROOT}/ffmpeg/include)
include_directories(${3RDLIB_ROOT}/ffmpeg/include)
include_directories(${3RDLIB_ROOT}/opencv/include)
include_directories(include)

aux_source_directory(src SRCS)
aux_source_directory(src/apps SRCS)
aux_source_directory(src/nodes SRCS)
add_executable(${PROJECT_NAME} src/main.cpp ${SRCS})

find_library(GLOG_LIBS NAMES glog HINTS ${3RDLIB_ROOT}/glog/lib)
find_library(JSONCPP_LIBS NAMES jsoncpp HINTS ${3RDLIB_ROOT}/jsoncpp/lib)
find_library(X264_LIBS NAMES x264 HINTS ${3RDLIB_ROOT}/x264/lib)
target_link_directories(${PROJECT_NAME} PUBLIC ${3RDLIB_ROOT}/ffmpeg/lib)
list(APPEND FFMPEG_LIBS
        avdevice avfilter postproc avformat avcodec swresample swscale avutil)

target_link_directories(${PROJECT_NAME} PUBLIC ${3RDLIB_ROOT}/opencv/lib)
list(APPEND OPENCV_LIBS
        opencv_core opencv_imgproc opencv_imgcodecs)

message(STATUS ${FFMPEG_LIBS})
target_link_libraries(${PROJECT_NAME} PUBLIC
        ${GLOG_LIBS}
        ${JSONCPP_LIBS}
        -Wl,--start-group
        ${X264_LIBS}
        ${FFMPEG_LIBS}
        -Wl,--end-group
        ${OPENCV_LIBS}
        pthread
        dl
        m
        z)
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/install)