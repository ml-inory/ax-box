//
// Created by yangrongzhao on 2023/4/26.
//

#ifndef AX_BOX_BLOB_H
#define AX_BOX_BLOB_H

#include <cstdlib>
#include <cstring>

//#include "blob_shape.h"

#define BLOB_NAME_LEN   128

/**
 * @brief 模型输出数据
 */
class Blob {
public:
    // 名称
    char name[BLOB_NAME_LEN]{};

    // 通道数
    int channel;

    // 高度
    int height;

    // 宽度
    int width;

    // 数据长度，单位 sizeof(float)
    int data_len;

    // 数据长度，单位 字节
    int data_len_in_byte;

    // 数据指针
    float *data;

    /**
     * @brief 默认构造，空数据
     */
    Blob();

    // 析构
    ~Blob();

    /**
     * @brief 构造
     * @param name_
     * @param c
     * @param h
     * @param w
     * @param data_
     */
    Blob(const char* name_, int c, int h, int w, float* data_ = nullptr);

    Blob& operator=(const Blob& other);

    Blob(const Blob& other);

    Blob& create(int c_, int h_, int w_);
};

Blob::Blob():
        Blob(nullptr, 0, 0, 0, nullptr) {

}

Blob::~Blob() {
    delete[] data;
}

Blob::Blob(const char* name_, int c, int h, int w, float* data_):
        channel(c),
        height(h),
        width(w),
        data_len(c * h * w),
        data_len_in_byte(data_len * (int)sizeof(float))
{
    if (data_len > 0)
        data = new float[data_len];
    else
        data = nullptr;

    strncpy(name, name_, BLOB_NAME_LEN);

    if (data_)
        memcpy(data, data_, data_len_in_byte);
}

Blob::Blob(const Blob& other):
        Blob(other.name, other.channel, other.height, other.width, other.data)
{

}

Blob& Blob::operator=(const Blob& other) {
    if (&other == this) {
        return *this;
    }

    delete[] data;

    channel = other.channel;
    height = other.height;
    width = other.width;
    data_len = channel * height * width;
    data_len_in_byte = data_len * (int)sizeof(float);
    if (data_len > 0)
        data = new float[data_len];
    else
        data = nullptr;

    strncpy(name, other.name, BLOB_NAME_LEN);

    if (other.data)
        memcpy(data, other.data, data_len_in_byte);
}

Blob& Blob::create(int c_, int h_, int w_) {
    delete[] data;

    channel = c_;
    height = h_;
    width = w_;
    data_len = channel * height * width;
    data_len_in_byte = data_len * (int)sizeof(float);
    memset(name, 0, BLOB_NAME_LEN);
    data = new float[data_len];
    return *this;
}

#endif //AX_BOX_BLOB_H
