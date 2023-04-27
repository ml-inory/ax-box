//
// Created by 祈Inory on 2023/4/27.
// Copyright (c) 2023 BBK. All rights reserved.
//
#include "blob.h"

namespace ax {

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
}