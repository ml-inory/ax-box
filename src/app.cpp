//
// Created by yangrongzhao on 2023/4/26.
//
#include "app.h"
#include <thread>

bool App::add_node(const std::string &name, NODE_TYPE_E type) {
    if (m_node_map.find(name) != m_node_map.end()) {
        LOG(ERROR) << "node " << name << " already exist!";
        return false;
    }

    auto node = CreateNode(type);
    if (!node) {
        LOG(ERROR) << "Cannot create node " << type;
        return false;
    }

    m_node_map.insert({name, node});
    return true;
}

bool App::init_nodes(const Json::Value &config) {
    for (auto & it : m_node_map) {
        if (!it.second->Init(config)) {
            LOG(ERROR) << "Init node " << it.first << " failed!";
            return false;
        }
    }
    return true;
}

void App::run_nodes() {
    LOG(INFO) << "Run nodes";
    for (auto& it: m_node_map) {
        std::thread t([&] {
            while (it.second->IsRunnable()) {
                it.second->Run();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
        t.detach();
    }
}

void App::Run() {
    run_nodes();
    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

bool App::connect_nodes(const std::string &top, const std::string &bottom, int queue_len) {
    auto top_node = m_node_map.find(top);
    if (top_node == m_node_map.end()) {
        LOG(ERROR) << "Cannot find top node " << top;
        return false;
    }

    auto bottom_node = m_node_map.find(bottom);
    if (bottom_node == m_node_map.end()) {
        LOG(ERROR) << "Cannot find bottom node " << bottom;
        return false;
    }

    auto queue = std::make_shared<LimitedQueue>(queue_len);
    top_node->second->SetOutputQueue(queue);
    bottom_node->second->SetInputQueue(queue);
    return true;
}
