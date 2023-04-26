//
// Created by 祈Inory on 2023/4/25.
// Copyright (c) 2023 BBK. All rights reserved.
//
#include "nodes/RTSPPullNode.h"
#include <chrono>
#include <memory>
#include <thread>

using namespace std::chrono;

bool RTSPPullNode::Init(const Json::Value& config) {
    if (!config.isMember("rtsp_url")) {
        LOG(ERROR) << "Cannot find 'rtsp_url' in config";
        return false;
    }

    m_rtsp_url = config["rtsp_url"].asString();

    // 初始化FFMPEG
    av_register_all();
    // 分配 AVFormatContext
    pFormatCtx = avformat_alloc_context();
    // 打开视频文件
    if (avformat_open_input(&pFormatCtx, m_rtsp_url.c_str(), NULL, NULL) != 0) {
        LOG(ERROR) << "Open video_path " << m_rtsp_url << " failed!";
        return false;
    }
    // 分析流信息
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        LOG(ERROR) << "Cannot find stream info!";
        return false;
    }
    // 获取videoStream
    videoStream = -1;
    for (int i = 0; i < pFormatCtx->nb_streams; ++i) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
        }
    }
    if (videoStream == -1) {
        LOG(ERROR) << "videoStream not found!";
        return false;
    }
    // 查找解码器
    pCodecCtx = pFormatCtx->streams[videoStream]->codec;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL) {
        LOG(ERROR) << "decoder not found!";
        return false;
    }
    // 打开解码器
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        LOG(ERROR) << "Cannot open decoder!";
        return false;
    }
    // 分配图像内存
    pFrame = av_frame_alloc();
    pFrameBGR = av_frame_alloc();
    // 分配输出缓存
    int numBytes = avpicture_get_size(AV_PIX_FMT_BGR24, pCodecCtx->width, pCodecCtx->height);
    out_buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
    avpicture_fill((AVPicture *) pFrameBGR, out_buffer, AV_PIX_FMT_BGR24,
                   pCodecCtx->width, pCodecCtx->height);
    // 分配图像转换器
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);
    if (!img_convert_ctx) {
        LOG(ERROR) << "init sws_ctx failed!";
        return false;
    }
    // 分配packet
    int y_size = pCodecCtx->width * pCodecCtx->height;
    packet = (AVPacket*)malloc(sizeof(AVPacket));
    av_new_packet(packet, y_size);
    //输出视频信息
    av_dump_format(pFormatCtx, 0, m_rtsp_url.c_str(), 0);

    SetRunnable();
    return true;
}

void RTSPPullNode::Run() {
    auto output_queue = GetOutputQueue();

    cv::Mat img;

    auto start = steady_clock::now();
    int got_picture = 0;
    if (av_read_frame(pFormatCtx, packet) >= 0) {
        if (packet->stream_index == videoStream) {
            int ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
            if (ret < 0) {
                LOG(WARNING) << "decode failed";
                return;
            }

            if (got_picture) {
                sws_scale(img_convert_ctx,
                          pFrame->data,
                          pFrame->linesize, 0, pCodecCtx->height, pFrameBGR->data,
                          pFrameBGR->linesize);

                img.create(pCodecCtx->height, pCodecCtx->width, CV_8UC3);
                memcpy(img.data, pFrameBGR->data[0], 3 * pCodecCtx->height * pCodecCtx->width);

//                        static int img_num = 0;
//                        cv::imwrite(std::to_string(img_num) + ".jpg", img);
//                        img_num++;
//                        log_info("Send image\n");
                output_queue->Push(std::make_shared<Packet>(img));
            }
        }
        av_free_packet(packet);

        auto end = steady_clock::now();
        float cost_time = duration_cast<milliseconds>(end - start).count();
        if (cost_time < 40.0f) {
            std::this_thread::sleep_for(std::chrono::milliseconds(long(40 - cost_time)));
        }
//        LOG(INFO) << "Received frame...";
    }
}

void RTSPPullNode::Release() {
    StopRunning();

    av_free(out_buffer);
    av_free(pFrameBGR);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
}