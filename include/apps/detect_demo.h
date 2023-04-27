//
// Created by 祈Inory on 2023/4/27.
// Copyright (c) 2023 BBK. All rights reserved.
//

#ifndef AX_BOX_DETECT_DEMO_H
#define AX_BOX_DETECT_DEMO_H

#include "app.h"

class DetectDemo : public App {
public:
    DetectDemo() = default;

    ~DetectDemo() override = default;

    bool Init(const Json::Value& config) override;
};

#endif //AX_BOX_DETECT_DEMO_H
