//
// Created by yangrongzhao on 2023/4/25.
//

#ifndef AX_BOX_AXBOX_H
#define AX_BOX_AXBOX_H

#include "json/json.h"
#include "glog/logging.h"
#include "node.h"
#include "node_factory.h"

#include <map>
#include <string>
#include <memory>

class App {
public:
    App() = default;

    virtual ~App() = default;

    virtual bool Init(const Json::Value& config) = 0;

    virtual void Run();

protected:
    bool add_node(const std::string& name, NODE_TYPE_E type);
    bool init_nodes(const Json::Value& config);
    void run_nodes();
    bool connect_nodes(const std::string& top, const std::string& bottom, int queue_len = -1);

private:
    std::map<std::string, std::shared_ptr<Node>> m_node_map;
};

#endif //AX_BOX_AXBOX_H
