//
// Created by 祈Inory on 2023/4/27.
// Copyright (c) 2023 BBK. All rights reserved.
//
#include "nodes/DetectNode.h"
#include "model/model_util.h"
#include "opencv2/imgproc.hpp"

bool DetectNode::Init(const Json::Value &config) {
    if (!config.isMember("model_path")) {
        LOG(ERROR) << "need 'model_path' in config";
        return false;
    }

    std::string model_path = config["model_path"].asString();
    detector = CreateModel(MODEL_YOLOV5S);
    if (!detector->Init(model_path)) {
        LOG(ERROR) << "Init detector failed!";
        return false;
    }

    SetRunnable();
    return true;
}

void DetectNode::Run() {
    auto input_queue = GetInputQueue();
    auto output_queue = GetOutputQueue();

    auto packet = input_queue->Pop();
    if (!packet) {
//        LOG(WARNING) << "Did not receive frame";
        return;
    }

    cv::Mat img = packet->Get<cv::Mat>();
    std::vector<DetectResult> result;
    detector->Run(img, &result);
    LOG(INFO) << "Detected " << result.size() << " objs";
    cv::Mat draw_img = draw_detect_result(img, result);
    cv::resize(draw_img, draw_img, cv::Size(0,0), 0.5, 0.5);

    output_queue->Push(std::make_shared<Packet>(draw_img));
}

void DetectNode::Release() {
    StopRunning();
}
