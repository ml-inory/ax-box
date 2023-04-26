//
// Created by yangrongzhao on 2023/4/26.
//

#ifndef AX_BOX_NODE_FACTORY_H
#define AX_BOX_NODE_FACTORY_H

#include <memory>
#include "node.h"

enum NODE_TYPE_E {
    NODE_RTSP_PULL = 0,
    NODE_RTMP_PUSH
};

std::shared_ptr<Node> CreateNode(NODE_TYPE_E type);

#endif //AX_BOX_NODE_FACTORY_H
