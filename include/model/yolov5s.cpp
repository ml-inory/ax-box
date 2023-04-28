//
// Created by 祈Inory on 2023/4/27.
// Copyright (c) 2023 BBK. All rights reserved.
//
#include "yolov5s.h"
#include "opencv2/imgproc.hpp"
#include "glog/logging.h"

using namespace ax;
using namespace std;

static std::string COCO_NAMES[] = {
        "person",
        "bicycle",
        "car",
        "motorbike",
        "aeroplane",
        "bus",
        "train",
        "truck",
        "boat",
        "traffic light",
        "fire hydrant",
        "stop sign",
        "parking meter",
        "bench",
        "bird",
        "cat",
        "dog",
        "horse",
        "sheep",
        "cow",
        "elephant",
        "bear",
        "zebra",
        "giraffe",
        "backpack",
        "umbrella",
        "handbag",
        "tie",
        "suitcase",
        "frisbee",
        "skis",
        "snowboard",
        "sports ball",
        "kite",
        "baseball bat",
        "baseball glove",
        "skateboard",
        "surfboard",
        "tennis racket",
        "bottle",
        "wine glass",
        "cup",
        "fork",
        "knife",
        "spoon",
        "bowl",
        "banana",
        "apple",
        "sandwich",
        "orange",
        "broccoli",
        "carrot",
        "hot dog",
        "pizza",
        "donut",
        "cake",
        "chair",
        "sofa",
        "pottedplant",
        "bed",
        "diningtable",
        "toilet",
        "tvmonitor",
        "laptop",
        "mouse",
        "remote",
        "keyboard",
        "cell phone",
        "microwave",
        "oven",
        "toaster",
        "sink",
        "refrigerator",
        "book",
        "clock",
        "vase",
        "scissors",
        "teddy bear",
        "hair drier",
        "toothbrush",
};

inline float sigmoid(float x) {
    return 1.0f / (1.0f + expf(-x));
}

void
Yolov5s::decodeResult(std::vector<DetectResult> &result, const ax::Blob &data, int stride, vector<vector<int>> anchors,
                      float scoreThresh) {
    int anchor_num = anchors.size();
    int slice_len = data.channel / anchor_num;
    int num_classes = slice_len - 5;
    int grid_w = data.width;
    int grid_h = data.height;

    float *ptr;
    for (int c = 0; c < anchor_num; c++) {
        ptr = data.data + c * slice_len * grid_w * grid_h;
        for (int h = 0; h < grid_h; h++) {
            for (int w = 0; w < grid_w; w++, ptr++) {
                float score = sigmoid(ptr[4 * grid_w * grid_h]);
                if (score >= scoreThresh) {
                    vector<float> det(6);
                    det[1] = (sigmoid(ptr[0]) * 2 - 0.5f + w) * stride; //center_x
                    det[2] = (sigmoid(ptr[grid_w * grid_h]) * 2 - 0.5f + h) * stride; //center_y
                    det[3] = powf((sigmoid(ptr[2 * grid_w * grid_h]) * 2), 2) * anchors[c][0]; //w
                    det[4] = powf((sigmoid(ptr[3 * grid_w * grid_h]) * 2), 2) * anchors[c][1]; //h

                    det[1] = det[1] - det[3] / 2; //left
                    det[2] = det[2] - det[4] / 2; //top
                    det[3] = det[1] + det[3]; //right
                    det[4] = det[2] + det[4]; //bottom

                    for (int i = 5; i < num_classes + 5; i++) {
                        float conf = sigmoid(ptr[i * grid_w * grid_h]);
                        if (conf * score > det[0]) {
                            det[0] = conf * score; //score
                            det[5] = i - 5; //class_id
                        }
                    }

                    if (det[0] >= scoreThresh) {
                        DetectResult res;
                        res.class_id = (int) det[5];
                        res.class_name = COCO_NAMES[res.class_id];
                        res.score = det[0];
                        res.bbox = cv::Rect(det[1], det[2], det[3] - det[1], det[4] - det[2]);
                        if (m_config.want_classes.size() > 0) {
                            for (const auto i: m_config.want_classes) {
                                if (i == (int) det[5]) {
                                    result.push_back(res);
                                }
                            }
                        } else {
                            result.push_back(res);
                        }
                    }

                }
            }
        }
    }
}

void letterbox(cv::Mat src, cv::Mat &dst, int letterbox_cols, int letterbox_rows) {
    /* letterbox process to support different letterbox size */
    float scale_letterbox;
    int resize_rows;
    int resize_cols;
    if ((letterbox_rows * 1.0 / src.rows) < (letterbox_cols * 1.0 / src.cols)) {
        scale_letterbox = (float) letterbox_rows * 1.0f / (float) src.rows;
    } else {
        scale_letterbox = (float) letterbox_cols * 1.0f / (float) src.cols;
    }
    resize_cols = int(scale_letterbox * (float) src.cols);
    resize_rows = int(scale_letterbox * (float) src.rows);

    dst.create(letterbox_rows, letterbox_cols, CV_8UC3);

    cv::resize(src, src, cv::Size(resize_cols, resize_rows));

    int top = (letterbox_rows - resize_rows) / 2;
    int bot = (letterbox_rows - resize_rows + 1) / 2;
    int left = (letterbox_cols - resize_cols) / 2;
    int right = (letterbox_cols - resize_cols + 1) / 2;

    // Letterbox filling
    cv::copyMakeBorder(src, dst, top, bot, left, right, cv::BORDER_CONSTANT, cv::Scalar(114, 114, 114));
}

void scale_back_letterbox(std::vector<DetectResult>& result, const cv::Mat& src, int letterbox_cols, int letterbox_rows) {
    /* letterbox process to support different letterbox size */
    float scale_letterbox;
    int resize_rows;
    int resize_cols;
    if ((letterbox_rows * 1.0 / src.rows) < (letterbox_cols * 1.0 / src.cols)) {
        scale_letterbox = (float) letterbox_rows * 1.0f / (float) src.rows;
    } else {
        scale_letterbox = (float) letterbox_cols * 1.0f / (float) src.cols;
    }
    resize_cols = int(scale_letterbox * (float) src.cols);
    resize_rows = int(scale_letterbox * (float) src.rows);

    int top = (letterbox_rows - resize_rows) / 2;
    int bot = (letterbox_rows - resize_rows + 1) / 2;
    int left = (letterbox_cols - resize_cols) / 2;
    int right = (letterbox_cols - resize_cols + 1) / 2;

    cv::Rect bound(0, 0, src.cols, src.rows);
    for (auto& res : result) {
        // adjust offset to original unpadded
        float x0 = (res.bbox.x - left) / scale_letterbox;
        float y0 = (res.bbox.y - top) / scale_letterbox;
        float x1 = (res.bbox.x + res.bbox.width - left) / scale_letterbox;
        float y1 = (res.bbox.y + res.bbox.height - top) / scale_letterbox;

        res.bbox = cv::Rect(x0, y0, x1 - x0, y1 - y0) & bound;
    }
}

bool Yolov5s::Run(const cv::Mat &img, void *result) {
    m_frame_width = img.cols;
    m_frame_height = img.rows;
    cv::Mat in = preprocess(img);
    float mean_vals[3] = {m_config.mean_vals[0], m_config.mean_vals[1], m_config.mean_vals[2]};
    float std_vals[3] = {m_config.std_vals[0], m_config.std_vals[1], m_config.std_vals[2]};

    std::vector<Blob> output_blobs;
    if (!m_backend->Run(in, output_blobs, mean_vals, std_vals, true)) {
        LOG(ERROR) << "Backend run failed!";
        return false;
    }
    auto *pResult = (std::vector<DetectResult> *) result;
    postprocess(output_blobs, img, *pResult);
    return true;
}

cv::Mat Yolov5s::preprocess(const cv::Mat &img) {
    cv::Mat dst;
    letterbox(img, dst, m_config.input_w, m_config.input_h);
    return dst;
}

void Yolov5s::postprocess(const std::vector<ax::Blob> &output_blobs, const cv::Mat& img, std::vector<DetectResult> &result) {
    if (output_blobs.empty()) {
        LOG(ERROR) << "Got no outputs!";
        return;
    }
    int strides[3] = {8, 16, 32};
    for (int i = 0; i < output_blobs.size(); i++) {
        decodeResult(result, output_blobs[i], strides[i], m_config.anchors[i], m_config.score_thresh);
    }
    result = NMS(result, m_config.nms_thresh, m_config.class_agnostic);
    scale_back_letterbox(result, img, m_config.input_w, m_config.input_h);
}
