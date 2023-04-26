//
// Created by yangrongzhao on 2023/4/26.
//

#ifndef AX_BOX_LIMITEDQUEUE_H
#define AX_BOX_LIMITEDQUEUE_H

#include <queue>
#include <memory>
#include <mutex>

#include "packet.h"

class LimitedQueue {
public:
    explicit LimitedQueue(int max_len = -1):
        m_max_len(max_len),
        m_len(0) {

    }

    ~LimitedQueue() = default;

    bool Push(std::shared_ptr<Packet> packet) {
        std::lock_guard<std::mutex> lock_guard(m_mutex);
        if (m_max_len > 0 && m_len >= m_max_len) {
            return false;
        }
        m_queue.push(packet);
        m_len++;
        return true;
    }

    std::shared_ptr<Packet> Pop() {
        std::lock_guard<std::mutex> lock_guard(m_mutex);
        if (m_len <= 0) {
            return nullptr;
        }
        auto packet = m_queue.front();
        m_queue.pop();
        m_len = std::max(0, m_len - 1);
        return packet;
    }

private:
    int m_max_len;
    int m_len;
    std::queue<std::shared_ptr<Packet>> m_queue;
    std::mutex m_mutex;
};

#endif //AX_BOX_LIMITEDQUEUE_H
