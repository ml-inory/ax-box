//
// Created by yangrongzhao on 2023/4/25.
//

#ifndef AX_BOX_AXBOX_H
#define AX_BOX_AXBOX_H

#include "json/json.h"
#include "glog/logging.h"
#include "node.h"

#include <map>
#include <string>
#include <memory>

class App {
public:
    App() = default;

    virtual ~App() = default;

    virtual bool Init(const Json::Value& config) = 0;

    virtual bool Run() = 0;

private:
    bool add_node(const std::string& name);

private:
    std::map<std::string, std::shared_ptr<Node>> m_node_map;
};

#endif //AX_BOX_AXBOX_H
