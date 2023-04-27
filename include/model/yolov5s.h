//
// Created by 祈Inory on 2023/4/27.
// Copyright (c) 2023 BBK. All rights reserved.
//

#ifndef AX_BOX_YOLOV5S_H
#define AX_BOX_YOLOV5S_H

#include "model_base.h"
#include "model_util.h"

typedef struct {
    bool class_agnostic;
    int input_w;
    int input_h;
    float score_thresh;
    float nms_thresh;
} YOLOV5_Config;

class Yolov5s : public ModelBase {
public:
    Yolov5s(const YOLOV5_Config& config):
            m_config(config) {

    }

    ~Yolov5s() = default;

    bool Run(const cv::Mat& img, void* result) override {

    }

private:
    YOLOV5_Config m_config;
};

#endif //AX_BOX_YOLOV5S_H
