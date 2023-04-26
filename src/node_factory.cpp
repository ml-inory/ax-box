//
// Created by yangrongzhao on 2023/4/26.
//
#include "node_factory.h"

#include "nodes/RTSPPullNode.h"
#include "nodes/RTMPPushNode.h"

std::shared_ptr<Node> CreateNode(NODE_TYPE_E type) {
    switch (type) {
        case NODE_RTSP_PULL:
            return std::shared_ptr<Node>(new RTSPPullNode);
        case NODE_RTMP_PUSH:
            return std::shared_ptr<Node>(new RTMPPushNode);

        default:
            return nullptr;
    }
}