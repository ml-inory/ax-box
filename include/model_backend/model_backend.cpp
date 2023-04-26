//
// Created by 祈Inory on 2023/4/27.
// Copyright (c) 2023 BBK. All rights reserved.
//
#include "model_backend.h"
#include "backend_ncnn.h"

std::shared_ptr<ModelBackend> GetBackend() {
    return std::shared_ptr<ModelBackend>(new BackendNCNN);
}