//
// Created by yangrongzhao on 2023/4/26.
//

#ifndef AX_BOX_BLOB_H
#define AX_BOX_BLOB_H

#include <cstdlib>
#include <cstring>

//#include "blob_shape.h"

#define BLOB_NAME_LEN   128

namespace ax {
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
}

#endif //AX_BOX_BLOB_H
