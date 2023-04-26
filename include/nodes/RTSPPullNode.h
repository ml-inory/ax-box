//
// Created by yangrongzhao on 2023/4/25.
//

#ifndef AX_BOX_RTSPPULLNODE_H
#define AX_BOX_RTSPPULLNODE_H

#include "node.h"
#include "opencv2/core.hpp"

#ifdef __cplusplus
extern "C"
{
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswresample/swresample.h"
#include "libavutil/pixfmt.h"
#include "libswscale/swscale.h"
#ifdef __cplusplus
};
#endif

class RTSPPullNode : public Node {
public:
    RTSPPullNode() = default;

    ~RTSPPullNode() override = default;

    bool Init(const Json::Value& config) override;

    void Run() override;

    void Release() override;

private:
    std::string m_rtsp_url;

    // 格式上下文
    AVFormatContext *pFormatCtx{};
    // 解码器上下文
    AVCodecContext *pCodecCtx{};
    // 解码器
    AVCodec *pCodec{};
    // 图像
    AVFrame *pFrame{}, *pFrameBGR{};
    // 解码包
    AVPacket *packet{};
    // 输出缓存
    uint8_t *out_buffer{};
    // 图像转换
    struct SwsContext *img_convert_ctx{};
    // 视频索引
    int videoStream{};
};




#endif //AX_BOX_RTSPPULLNODE_H
