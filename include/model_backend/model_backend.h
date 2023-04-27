//
// Created by yangrongzhao on 2023/4/26.
//

#ifndef AX_BOX_MODEL_BACKEND_H
#define AX_BOX_MODEL_BACKEND_H

#include <string>
#include <memory>
#include "opencv2/core.hpp"
#include "blob.h"
#include "glog/logging.h"

class ModelBackend {
public:
    ModelBackend() = default;

    virtual ~ModelBackend() = default;

    virtual bool Init(const std::string& model_path) = 0;

    virtual bool Run(const cv::Mat& img, std::vector<ax::Blob>& outputs, float* mean_vals, float* std_vals, bool bgr2rgb) = 0;

    virtual void Release() = 0;

    cv::Size GetInputSize() const {
        return m_input_size;
    }

protected:
    cv::Size m_input_size;
};

std::shared_ptr<ModelBackend> GetBackend();

#endif //AX_BOX_MODEL_BACKEND_H
