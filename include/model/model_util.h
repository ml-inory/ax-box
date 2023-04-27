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
    cv::Rect bbox;
} DetectResult;

float IOU(const cv::Rect &lhs, const cv::Rect &rhs) {
    float max_x1 = (float) std::max(lhs.x, rhs.x);
    float max_y1 = (float) std::max(lhs.y, rhs.y);
    float min_x1 = (float) std::min(lhs.x, rhs.x);
    float min_y1 = (float) std::min(lhs.y, rhs.y);
    float min_x2 = (float) std::min(lhs.x + lhs.width, rhs.x + rhs.width);
    float min_y2 = (float) std::min(lhs.y + lhs.height, rhs.y + rhs.height);
    float max_x2 = (float) std::max(lhs.x + lhs.width, rhs.x + rhs.width);
    float max_y2 = (float) std::max(lhs.y + lhs.height, rhs.y + rhs.height);
    float inter_w = std::max(0.0f, min_x2 - max_x1);
    float inter_h = std::max(0.0f, min_y2 - max_y1);
    float outer_w = std::max(0.0f, max_x2 - min_x1);
    float outer_h = std::max(0.0f, max_y2 - min_y1);
    return inter_h * inter_w / (outer_h * outer_w);
}

std::vector<DetectResult> NMS(std::vector<DetectResult> &proposals, float nms_thresh = 0.4, bool class_agnostic=true) {
    std::sort(proposals.begin(), proposals.end(), [](const DetectResult& a, const DetectResult& b) { return a.score > b.score; });
    std::vector<float> vArea(proposals.size());
    for (int i = 0; i < int(proposals.size()); ++i) {
        vArea[i] = proposals.at(i).bbox.area();
    }
    for (int i = 0; i < int(proposals.size()); ++i) {
        for (int j = i + 1; j < int(proposals.size());) {
            float ovr = IOU(proposals[i].bbox, proposals[j].bbox);
            if (ovr >= nms_thresh) {
                if (class_agnostic) {
                    if (proposals[i].class_id == proposals[j].class_id) {
                        proposals.erase(proposals.begin() + j);
                        vArea.erase(vArea.begin() + j);
                    }
                    else {
                        j++;
                    }
                }
                else {
                    proposals.erase(proposals.begin() + j);
                    vArea.erase(vArea.begin() + j);
                }
            }
            else {
                j++;
            }
        }
    }
}

#endif //AX_BOX_MODEL_UTIL_H
