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
    int num_classes;
    std::vector<float> mean_vals;
    std::vector<float> std_vals;
    std::vector<int> want_classes;
    std::vector<std::vector<std::vector<int>>> anchors;
} YOLOV5_Config;

class Yolov5s : public ModelBase {
public:
    Yolov5s(const YOLOV5_Config& config):
        ModelBase(),
        m_config(config) {

    }

    ~Yolov5s() = default;

    bool Run(const cv::Mat& img, void* result) override;

private:
    cv::Mat preprocess(const cv::Mat& img);
    void postprocess(const std::vector<ax::Blob>& output_blobs, std::vector<DetectResult>& result);
    void decodeResult(std::vector<DetectResult>& result, const ax::Blob& data, int stride, std::vector<std::vector<int>> anchors, float scoreThresh, int frameWidth, int frameHeight);

private:
    YOLOV5_Config m_config;
    int m_frame_width, m_frame_height;
};

#endif //AX_BOX_YOLOV5S_H
