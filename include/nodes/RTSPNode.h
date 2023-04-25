//
// Created by yangrongzhao on 2023/4/25.
//

#ifndef AX_BOX_RTSPNODE_H
#define AX_BOX_RTSPNODE_H

#include "node.h"

class RTSPNode : public Node {
public:
    RTSPNode() = default;
    ~RTSPNode() = default;

    bool Init();

    bool Run();

    void Release();
};

#endif //AX_BOX_RTSPNODE_H
