//
// Created by 祈Inory on 2023/4/27.
// Copyright (c) 2023 BBK. All rights reserved.
//
#include "apps/detect_demo.h"

bool DetectDemo::Init(const Json::Value &config) {
    if (!add_node("input", NODE_RTSP_PULL)) {
        LOG(ERROR) << "Add node 'input' failed!";
        return false;
    }

    if (!add_node("detect", NODE_DETECT)) {
        LOG(ERROR) << "Add node 'detect' failed!";
        return false;
    }

    if (!add_node("output", NODE_RTMP_PUSH)) {
        LOG(ERROR) << "Add node 'output' failed!";
        return false;
    }

    if (!init_nodes(config)) {
        LOG(ERROR) << "Init nodes failed!";
        return false;
    }

    connect_nodes("input", "detect", 100);
    connect_nodes("detect", "output", 100);
    return true;
}

