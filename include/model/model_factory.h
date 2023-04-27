//
// Created by 祈Inory on 2023/4/27.
// Copyright (c) 2023 BBK. All rights reserved.
//

#ifndef AX_BOX_MODEL_FACTORY_H
#define AX_BOX_MODEL_FACTORY_H

#include "model_base.h"

enum MODEL_TYPE_E {
    MODEL_YOLOV5S = 0
};

std::shared_ptr<ModelBase> CreateModel(MODEL_TYPE_E type);

#endif //AX_BOX_MODEL_FACTORY_H
