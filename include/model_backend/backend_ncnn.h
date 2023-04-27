//
// Created by 祈Inory on 2023/4/27.
// Copyright (c) 2023 BBK. All rights reserved.
//

#ifndef AX_BOX_BACKEND_NCNN_H
#define AX_BOX_BACKEND_NCNN_H

#include "model_backend.h"
#include "ncnn/net.h"

class BackendNCNN : public ModelBackend {
public:
    // 默认构造
    BackendNCNN() = default;

    // 默认析构
    ~BackendNCNN() override = default;

    bool Init(const std::string& model_path) override;

    bool Run(const cv::Mat& img, std::vector<ax::Blob>& outputs, float* mean_vals, float* std_vals, bool bgr2rgb) override;

    void Release() override;

private:
    ncnn::Net net;
};

#endif //AX_BOX_BACKEND_NCNN_H
