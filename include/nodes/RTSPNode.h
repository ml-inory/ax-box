//
// Created by yangrongzhao on 2023/4/25.
//

#ifndef AX_BOX_RTSPNODE_H
#define AX_BOX_RTSPNODE_H

#include "node.h"

class RTSPNode : public Node {
public:
    RTSPNode() = default;

    ~RTSPNode() override = default;

    bool Init(const Json::Value& config) override;

    bool Run() override;

    void Release() override;

private:
    std::string m_rtsp_url;
};


#endif //AX_BOX_RTSPNODE_H
