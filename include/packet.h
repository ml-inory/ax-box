//
// Created by yangrongzhao on 2023/4/25.
//

#ifndef AX_BOX_PACKET_H
#define AX_BOX_PACKET_H

#include <memory>
#include <chrono>
#include <typeindex>
#include <fstream>
#include <sstream>

/**
 * @brief: Stream和Port中的数据包.
 *
 */
class Packet {
public:
    // 创建一个空的Packet，IsEmpty()为true
    Packet() :
            m_typeIndex(std::type_index(typeid(void))),
            m_realObj(nullptr) {

    }

    /**
     * @brief: Packet 构造函数
     * @example: Packet p(10);  int value = p.Get<int>();
     * @example: Packet p(cv::Mat());  const auto& img = p.Get<cv::Mat>();
     * @tparam T
     * @param： p_realObj       传入的真实数据
     * @see E_PACKET_TYPE
     */
    template<typename T>
    Packet(const T &p_realObj):
            m_typeIndex(std::type_index(typeid(T))),
            m_realObj(std::make_shared<Wrapper < T> > (p_realObj)) {

    }

    /**
     * @brief: Packet 拷贝构造函数，时间戳是一样的
     *
     * @param： other
     */
    Packet(const Packet &other) = default;

    Packet &operator=(const Packet &other) {
        if (this == &other) {
            return *this;
        }

        this->m_typeIndex = other.m_typeIndex;
        this->m_realObj = other.m_realObj;

        return *this;
    }

    virtual ~Packet() = default;

    /**
     * @brief: 判断真实数据是否类型U
     * @example: packet.Is<int>();
     * @tparam U
     * @return：true
     * @return：false
     */
    template<typename U>
    bool Is() const {
        return (m_typeIndex == std::type_index(typeid(U)));
    }

    /**
     * @brief: 获取const T&类型的数据，如果实际上不是这个类型的话会报错
     *
     * @tparam T
     * @return：const T&
     */
    template<class T>
    inline const T &Get() const {
        if (!Is<T>()) {
            throw std::bad_cast();
        }
        auto *derived = static_cast<Wrapper <T> *>(m_realObj.get());
        return *((T *) derived->GetData());
    }

    /**
     * @brief: 返回 是否是空的Packet
     *
     * @return：true
     * @return：false
     */
    bool IsEmpty() const {
        return (m_realObj == nullptr);
    }

private:
    /**
     * @brief: 类型擦除Idiom，通过PacketBase和Wrapper的组合可以包装任意类型
     *
     */
    struct PacketBase {
        virtual ~PacketBase() {}

        virtual void *GetData() = 0;
    };

    template<typename T>
    struct Wrapper : public PacketBase {
        T data;

        Wrapper(const T &t) :
                data(std::move(t)) {}

        void *GetData() override {
            return (void *) &data;
        }
    };

private:
    std::type_index m_typeIndex;
    std::shared_ptr<PacketBase> m_realObj;
};


#endif //AX_BOX_PACKET_H
