//
// Created by yangrongzhao on 2023/4/25.
//

#ifndef AX_BOX_NODE_H
#define AX_BOX_NODE_H

#include "json/json.h"
#include "glog/logging.h"
#include "LimitedQueue.h"

class Node {
public:
    Node():
        m_runnable(false) {

    }

    virtual ~Node() = default;

    virtual bool Init(const Json::Value& config) = 0;

    virtual void Run() = 0;

    virtual void Release() = 0;

    void SetRunnable() {
        m_runnable = true;
    }

    [[nodiscard]] bool IsRunnable() const {
        return m_runnable;
    }

    void StopRunning() {
        m_runnable = false;
    }

    void SetInputQueue(std::shared_ptr<LimitedQueue>& input_queue) {
        m_input_queue = input_queue;
    }

    void SetOutputQueue(std::shared_ptr<LimitedQueue>& output_queue) {
        m_output_queue = output_queue;
    }

    [[nodiscard]] std::shared_ptr<LimitedQueue> GetInputQueue() const {
        return m_input_queue;
    }

    [[nodiscard]] std::shared_ptr<LimitedQueue> GetOutputQueue() const {
        return m_output_queue;
    }

private:
    std::shared_ptr<LimitedQueue> m_input_queue;
    std::shared_ptr<LimitedQueue> m_output_queue;
    bool m_runnable;
};

#endif //AX_BOX_NODE_H
