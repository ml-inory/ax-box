//
// Created by 祈Inory on 2023/4/27.
// Copyright (c) 2023 BBK. All rights reserved.
//

#include "backend_ncnn.h"

#include "ncnn/mat.h"


bool BackendNCNN::Init(const std::string& model_path) {
    std::string param, bin;
    size_t pos = model_path.find_last_of('.');
    if (pos == std::string::npos) {
        return false;
    }
    std::string model_name = model_path.substr(0, pos);
    param = model_name + ".param";
    bin = model_name + ".bin";

    // 使能Vulkan
    net.opt.use_vulkan_compute = 0;

    if (0 != net.load_param(param.c_str())) {
        return false;
    }

    if (0 != net.load_model(bin.c_str())) {
        return false;
    }

    return true;
}

/**
 * @brief 运行模型
 * @param p_cvImg       输入图像
 * @param p_outputBlobs 输出数据
 * @return
 */
bool BackendNCNN::Run(const cv::Mat& img, std::vector<Blob>& outputs) {
    ncnn::Mat in;
    in = ncnn::Mat::from_pixels_resize(img.data, ncnn::Mat::PIXEL_RGB, img.cols, img.rows, img.cols, img.rows);

    auto ex = net.create_extractor();
    ex.set_light_mode(true);
    ex.set_num_threads(4);

    if (0 != ex.input(0, in)) {
        return false;
    }

    outputs.clear();
    for (const auto& name : net.output_names()) {
        ncnn::Mat out;
        if (0 != ex.extract(name, out)) {
            return false;
        }
        outputs.emplace_back(name, out.c, out.h, out.w, (float*)out.data);
    }

    return true;
}

/**
 * @brief 释放模型
 * @return
 */
void BackendNCNN::Release() {

}