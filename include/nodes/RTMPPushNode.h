//
// Created by yangrongzhao on 2023/4/26.
//

#ifndef AX_BOX_RTMPPUSHNODE_H
#define AX_BOX_RTMPPUSHNODE_H

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

class RTMPPushNode : public Node {
public:
    RTMPPushNode() = default;

    ~RTMPPushNode() override = default;

    bool Init(const Json::Value &config) override;

    void Run() override;

    void Release() override;

private:
    bool createOutputStreams(AVFormatContext* inputFormatContext, AVFormatContext* outputFormatContext);

private:
    std::string m_rtsp_url;
    std::string m_rtmp_url;

    // 格式上下文
    AVFormatContext *pInputFormatCtx{};
    AVFormatContext *pOutputFormatCtx{};
    // 编码器
    AVCodecContext* codec_context;
    // 图像
    AVFrame *pYUV{};
    // 解码包
    AVPacket packet{};
    AVStream* vs;
    // 图像转换
    struct SwsContext *img_convert_ctx{};
    // 视频索引
    int videoStream{};
    int vpts;
};

#endif //AX_BOX_RTMPPUSHNODE_H
