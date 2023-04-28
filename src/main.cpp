//
// Created by yangrongzhao on 2023/4/25.
//
#include <cstdio>
#include <fstream>
#include <memory>
#include "opencv2/imgcodecs.hpp"

#include "glog/logging.h"
#include "json/json.h"
#include "apps/rtsp_demo.h"
#include "apps/detect_demo.h"

#include "model/model_factory.h"
#include "model/model_util.h"

int yolov5_demo(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s [model_path] [image_path]\n", argv[0]);
        return -1;
    }
    std::string model_path(argv[1]);
    std::string img_path(argv[2]);
    LOG(INFO) << "model_path: " << model_path;
    LOG(INFO) << "img_path: " << img_path;

    auto model = CreateModel(MODEL_YOLOV5S);
    if (!model->Init(model_path)) {
        LOG(ERROR) << "model init failed!";
        return -1;
    }

    cv::Mat img = cv::imread(img_path);
    if (img.empty()) {
        LOG(ERROR) << "Read image " << img_path << " failed!";
        return -1;
    }

    std::vector<DetectResult> result;
    if (!model->Run(img, &result)) {
        LOG(ERROR) << "Run model failed!";
        return -1;
    }

    cv::Mat out = draw_detect_result(img, result);
    cv::imwrite("yolov5_demo.jpg", out);
    LOG(INFO) << "Write image to yolov5_demo.jpg";

    return 0;
}

int detect_demo(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [config file]\n", argv[0]);
        return -1;
    }

    const char* config_file = argv[1];
    LOG(INFO) << "config file: " << config_file;

    std::ifstream ifs(config_file);
    if (!ifs.is_open()) {
        LOG(ERROR) << "Open config file " << config_file << " failed!";
        return -1;
    }

    Json::Value config;
    ifs >> config;
    std::unique_ptr<App> app(new DetectDemo);
    if (!app->Init(config)) {
        LOG(ERROR) << "Init app failed!";
        return -1;
    }
    LOG(INFO) << "App init success";

    app->Run();

    return 0;
}

int main(int argc, char** argv) {
    FLAGS_logtostderr = 1;
    google::InitGoogleLogging(argv[0]);

    detect_demo(argc, argv);
}
