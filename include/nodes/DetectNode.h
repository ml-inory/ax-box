//
// Created by 祈Inory on 2023/4/27.
// Copyright (c) 2023 BBK. All rights reserved.
//

#ifndef AX_BOX_DETECTNODE_H
#define AX_BOX_DETECTNODE_H

#include "node.h"
#include "model/model_factory.h"

class DetectNode : public Node {
public:
    DetectNode() = default;

    ~DetectNode() override = default;

    bool Init(const Json::Value& config) override;

    void Run() override;

    void Release() override;

private:
    std::shared_ptr<ModelBase> detector;
};

#endif //AX_BOX_DETECTNODE_H
