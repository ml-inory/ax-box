//
// Created by 祈Inory on 2023/4/27.
// Copyright (c) 2023 BBK. All rights reserved.
//

#ifndef AX_BOX_MODEL_UTIL_H
#define AX_BOX_MODEL_UTIL_H

#include <vector>
#include "opencv2/core.hpp"

typedef struct {
    int class_id;
    float score;
    std::string class_name;
    cv::Rect bbox;
} DetectResult;

float IOU(const cv::Rect &lhs, const cv::Rect &rhs);
std::vector<DetectResult> NMS(std::vector<DetectResult> &proposals, float nms_thresh = 0.4, bool class_agnostic=true);
cv::Mat draw_detect_result(const cv::Mat& img, const std::vector<DetectResult>& result, cv::Scalar color=cv::Scalar(0,255,0), int thickness=2);


#endif //AX_BOX_MODEL_UTIL_H
