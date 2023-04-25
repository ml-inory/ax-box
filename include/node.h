//
// Created by yangrongzhao on 2023/4/25.
//

#ifndef AX_BOX_NODE_H
#define AX_BOX_NODE_H

#include "json/json.h"
#include "glog/logging.h"

class Node {
public:
    Node() = default;

    virtual ~Node() = default;

    virtual bool Init(const Json::Value& config) = 0;

    virtual bool Run() = 0;

    virtual void Release() = 0;
};

#endif //AX_BOX_NODE_H
