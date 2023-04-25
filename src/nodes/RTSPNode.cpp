//
// Created by 祈Inory on 2023/4/25.
// Copyright (c) 2023 BBK. All rights reserved.
//
#include "nodes/RTSPNode.h"

bool RTSPNode::Init(const Json::Value& config) {
    if (!config.isMember("rtsp_url")) {
        LOG(ERROR) << "Cannot find 'rtsp_url' in config";
        return false;
    }
    return true;
}

bool RTSPNode::Run() {
    return false;
}

void RTSPNode::Release() {

}