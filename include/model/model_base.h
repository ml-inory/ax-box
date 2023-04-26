//
// Created by 祈Inory on 2023/4/27.
// Copyright (c) 2023 BBK. All rights reserved.
//

#ifndef AX_BOX_MODEL_BASE_H
#define AX_BOX_MODEL_BASE_H

#include "model_backend/model_backend.h"

class ModelBase {
public:
    ModelBase() {
        m_backend = GetBackend();
    }

    virtual ~ModelBase() = default;

    virtual bool Init(const std::string& model_path) {
        return m_backend->Init(model_path);
    }

    virtual bool Run(const cv::Mat& img, void* result) = 0;

    virtual void Release() { }

protected:
    std::shared_ptr<ModelBackend> m_backend;
};

#endif //AX_BOX_MODEL_BASE_H
