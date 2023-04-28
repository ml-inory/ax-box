//
// Created by 祈Inory on 2023/4/27.
// Copyright (c) 2023 BBK. All rights reserved.
//
#include "model_factory.h"

#include "model/yolov5s.h"

std::shared_ptr<ModelBase> CreateModel(MODEL_TYPE_E type) {
    switch (type) {
        case MODEL_YOLOV5S: {
            YOLOV5_Config config;
            config.input_w = 640;
            config.input_h = 640;
            config.num_classes = 80;
            config.class_agnostic = true;
            config.score_thresh = 0.25f;
            config.nms_thresh = 0.4f;
            config.mean_vals = {0.f, 0.f, 0.f};
            config.std_vals = {1 / 255.f, 1 / 255.f, 1 / 255.f};
            config.anchors = {{{10, 13}, {16, 30}, {33, 23}},
                              {{30, 61}, {62, 45}, {59, 119}},
                              {{116, 90}, {156, 198}, {373, 326}}};
            config.want_classes = {0};
            return std::shared_ptr<ModelBase>(new Yolov5s(config));
        }

        default:
            return nullptr;
    }
}