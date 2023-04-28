//
// Created by 祈Inory on 2023/4/28.
// Copyright (c) 2023 BBK. All rights reserved.
//
#include "model/model_factory.h"
#include "glog/logging.h"
#include "model/model_util.h"
#include "opencv2/imgcodecs.hpp"

int main(int argc, char** argv) {
    FLAGS_logtostderr = 1;
    google::InitGoogleLogging(argv[0]);

    if (argc < 3) {
        fprintf(stderr, "Usage: %s [model_path] [image_path]\n");
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