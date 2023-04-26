//
// Created by yangrongzhao on 2023/4/26.
//

#ifndef AX_BOX_MODEL_BACKEND_H
#define AX_BOX_MODEL_BACKEND_H

#include <string>
#include "opencv2/core.hpp"
#include "blob.h"

class ModelBackend {
public:
    ModelBackend() = default;

    virtual ~ModelBackend() = default;

    virtual bool Init(const std::string& model_path) = 0;

    virtual bool Run(const cv::Mat& img, std::vector<Blob>& outputs) = 0;
};

#endif //AX_BOX_MODEL_BACKEND_H
