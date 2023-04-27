//
// Created by 祈Inory on 2023/4/27.
// Copyright (c) 2023 BBK. All rights reserved.
//
#include "model_util.h"
#include "opencv2/imgproc.hpp"


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

std::vector<DetectResult> NMS(std::vector<DetectResult> &proposals, float nms_thresh, bool class_agnostic) {
    std::vector<DetectResult> result;
    std::sort(proposals.begin(), proposals.end(), [](const DetectResult& a, const DetectResult& b) { return a.score > b.score; });
    for (auto it = proposals.begin(); it != proposals.end();) {
        result.push_back(*it);
        for (auto j = it + 1; j != proposals.end(); ) {
            float ovr = IOU(it->bbox, j->bbox);
            if (ovr >= nms_thresh) {
                if (class_agnostic) {
                    if (it->class_id == j->class_id) {
                        j = proposals.erase(j);
                    } else {
                        j++;
                    }
                } else {
                    j = proposals.erase(j);
                }
            } else {
                j++;
            }
        }
        it = proposals.erase(it);
    }
    return result;
}

cv::Mat draw_detect_result(const cv::Mat& img, const std::vector<DetectResult>& result, cv::Scalar color, int thickness)
{
    cv::Mat out_img = img.clone();

    int src_w = img.cols;
    int src_h = img.rows;

    for (size_t i = 0; i < result.size(); i++)
    {
        const DetectResult & bbox = result[i];
        //fprintf(stderr, "%d = %.5f at %.2f %.2f %.2f %.2f\n", bbox.label, bbox.score,
        //    bbox.x1, bbox.y1, bbox.x2, bbox.y2);
        cv::rectangle(out_img, bbox.bbox, color, thickness);

        char text[256];
        sprintf(text, "%s %.2f", bbox.class_name.c_str(), bbox.score);

        int baseLine = 0;
        cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.4, 1, &baseLine);

        int x = bbox.bbox.x;
        int y = bbox.bbox.y - label_size.height - baseLine;
        if (y < 0)
            y = 0;
        if (x + label_size.width > out_img.cols)
            x = out_img.cols - label_size.width;

        cv::rectangle(out_img, cv::Rect(cv::Point(x, y), cv::Size(label_size.width, label_size.height + baseLine)),
                      color, -1);

        cv::putText(out_img, text, cv::Point(x, y + label_size.height),
                    cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255));
    }
    return out_img;
}