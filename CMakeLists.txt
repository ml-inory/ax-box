cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(ax-box CXX)
set (CMAKE_CXX_STANDARD 11)

set(3RDLIB_ROOT ${CMAKE_SOURCE_DIR}/3rdlibs)

include_directories(${3RDLIB_ROOT}/jsoncpp/include)
include_directories(${3RDLIB_ROOT}/glog/include)
include_directories(${3RDLIB_ROOT}/ffmpeg/include)
include_directories(${3RDLIB_ROOT}/ffmpeg/include)
include_directories(${3RDLIB_ROOT}/opencv/include)
include_directories(${3RDLIB_ROOT}/ncnn/include)
include_directories(include)
include_directories(include/model_backend)

aux_source_directory(src/apps SRCS)
aux_source_directory(src/nodes SRCS)
aux_source_directory(include/model SRCS)
aux_source_directory(include/model_backend SRCS)
add_executable(${PROJECT_NAME} src/main.cpp src/app.cpp src/node_factory.cpp ${SRCS})

find_library(GLOG_LIBS NAMES glog HINTS ${3RDLIB_ROOT}/glog/lib)
find_library(JSONCPP_LIBS NAMES jsoncpp HINTS ${3RDLIB_ROOT}/jsoncpp/lib)
find_library(X264_LIBS NAMES x264 HINTS ${3RDLIB_ROOT}/x264/lib)
target_link_directories(${PROJECT_NAME} PUBLIC ${3RDLIB_ROOT}/ffmpeg/lib)
list(APPEND FFMPEG_LIBS
        avdevice avfilter postproc avformat avcodec swresample swscale avutil)

target_link_directories(${PROJECT_NAME} PUBLIC ${3RDLIB_ROOT}/opencv/lib)
target_link_directories(${PROJECT_NAME} PUBLIC ${3RDLIB_ROOT}/opencv/share/OpenCV/3rdparty/lib)
list(APPEND OPENCV_LIBS
        opencv_core opencv_imgproc opencv_imgcodecs opencv_calib3d opencv_features2d)
list(APPEND OPENCV_3RD_LIBS
        libjpeg-turbo libprotobuf quirc)
find_library(NCNN_LIBS NAMES ncnn HINTS ${3RDLIB_ROOT}/ncnn/lib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

target_link_libraries(${PROJECT_NAME} PUBLIC
        ${GLOG_LIBS}
        ${JSONCPP_LIBS}
        -Wl,--start-group
        ${X264_LIBS}
        ${FFMPEG_LIBS}
        -Wl,--end-group
        -Wl,--start-group
        ${OPENCV_LIBS}
        ${OPENCV_3RD_LIBS}
        -Wl,--end-group
        ${NCNN_LIBS}
        pthread
        dl
        m
        z)
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/install)