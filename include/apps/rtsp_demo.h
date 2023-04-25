//
// Created by 祈Inory on 2023/4/25.
// Copyright (c) 2023 BBK. All rights reserved.
//

#ifndef AX_BOX_RTSP_DEMO_H
#define AX_BOX_RTSP_DEMO_H

#include "app.h"

class RTSPDemo : public App {
public:
    RTSPDemo() = default;

    ~RTSPDemo() override = default;

    bool Init(const Json::Value& config) override;

    bool Run() override;
};

#endif //AX_BOX_RTSP_DEMO_H
