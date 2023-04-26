//
// Created by yangrongzhao on 2023/4/26.
//
#include "nodes/RTMPPushNode.h"

bool RTMPPushNode::createOutputStreams(AVFormatContext* inputFormatContext, AVFormatContext* outputFormatContext) {
    // 遍历输入流的所有轨道,拷贝编解码参数到输出流
    for(int i = 0 ; i < inputFormatContext->nb_streams ; i++) {
        // 为输出流创建轨道
        vs = avformat_new_stream(outputFormatContext, NULL);
        if(NULL == vs) {
            LOG(ERROR) << "can't create stream, index " << i;
            return false;
        }

        // 编解码参数在AVCodecParameters中保存,从输入流拷贝到输出流
        if(avcodec_parameters_copy(vs->codecpar, inputFormatContext->streams[i]->codecpar) < 0) {
            LOG(ERROR) << "can't copy codec paramters, stream index " << i;
            return false;
        }

        // codec_tag代表了音视频数据采用的码流格式,不同的封装格式如flv、mp4等的支持情况是不一样的
        // 上面的avcodec_parameters_copy将输出流的codec_tag从输入拷贝过来变成了一样的
        // 由于我们输出流在avformat_alloc_output_context2的时候写死了flv格式
        // 如果输入流不是flv而是mp4等格式的话就可能会出现mp4里某种codec_tag在flv不支持导致推流失败的情况
        // 这里我们可以用av_codec_get_id从输出流的oformat的支持的codec_tag列表里面查找codec_id
        // 如果和codecpar的codec_id不一致的话代表不支持
        if(av_codec_get_id(outputFormatContext->oformat->codec_tag, vs->codecpar->codec_tag) != vs->codecpar->codec_id) {
            // 这里将codec_tag设置为0,FFmpeg会根据编码codec_id从封装格式的codec_tag列表中找到一个codec_tag
            vs->codecpar->codec_tag = 0;
        }
    }
    return true;
}

bool RTMPPushNode::Init(const Json::Value &config) {
    if (!config.isMember("rtsp_url")) {
        LOG(ERROR) << "Cannot find 'rtsp_url' in config";
        return false;
    }

    if (!config.isMember("rtmp_url")) {
        LOG(ERROR) << "Cannot find 'rtmp_url' in config";
        return false;
    }

    m_rtsp_url = config["rtsp_url"].asString();
    m_rtmp_url = config["rtmp_url"].asString();

    // 注册所有网络协议
    avformat_network_init();

    // 打开文件流读取文件头解析出视频信息如轨道信息、时长等
    // mFormatContext初始化为NULL,如果打开成功,它会被设置成非NULL的值
    // 这个方法实际可以打开多种来源的数据,url可以是本地路径、rtmp地址等
    // 在不需要的时候通过avformat_close_input关闭文件流
    if(avformat_open_input(&pInputFormatCtx, m_rtsp_url.c_str(), NULL, NULL) < 0) {
        LOG(ERROR) << "open " << m_rtsp_url << " failed!";
        return false;
    }

    // 对于没有文件头的格式如MPEG或者H264裸流等,可以通过这个函数解析前几帧得到视频的信息
    if(avformat_find_stream_info(pInputFormatCtx, NULL) < 0) {
        LOG(ERROR) << "can't find stream info in " << m_rtsp_url;
        return false;
    }

    // 打印输入视频信息
    av_dump_format(pInputFormatCtx, 0, m_rtsp_url.c_str(), 0);

    // 创建输出流上下文,outputFormatContext初始化为NULL,如果打开成功,它会被设置成非NULL的值,在不需要的时候使用avformat_free_context释放
    // 输出流使用flv格式
    if(avformat_alloc_output_context2(&pOutputFormatCtx, NULL, "flv", m_rtmp_url.c_str()) < 0) {
        LOG(ERROR) << "can't alloc output context for " << m_rtmp_url;
        return false;
    }

    // 拷贝编解码参数
    if(!createOutputStreams(pInputFormatCtx, pOutputFormatCtx)) {
        return false;
    }

    // 打印输出视频信息
    av_dump_format(pOutputFormatCtx, 0, m_rtmp_url.c_str(), 1);

    // 打开输出流,结束的时候使用avio_close关闭
    if(avio_open(&pOutputFormatCtx->pb, m_rtmp_url.c_str(), AVIO_FLAG_WRITE) < 0) {
        LOG(ERROR) << "can't open avio " << m_rtmp_url;
        return false;
    }

    // 设置flvflags为no_duration_filesize用于解决下面的报错
    // [flv @ 0x14f808e00] Failed to update header with correct duration.
    // [flv @ 0x14f808e00] Failed to update header with correct filesize
    AVDictionary * opts = NULL;
    av_dict_set(&opts, "flvflags", "no_duration_filesize", 0);
    if(avformat_write_header(pOutputFormatCtx, opts ? &opts : NULL) < 0) {
        LOG(ERROR) << "write header to " << m_rtmp_url << " failed!";
        return false;
    }

    vpts = 0;

    SetRunnable();
    return true;
}

void RTMPPushNode::Run() {
    auto input_queue = GetInputQueue();
    cv::Mat frame;
    std::shared_ptr<Packet> input_packet;
    if (! (input_packet = input_queue->Pop())) {
        return;
    }
    frame = input_packet->Get<cv::Mat>();

    int fps = 25;
    int width = frame.cols;
    int height = frame.rows;

    // 2.初始化格式转换上下文
    if (img_convert_ctx == NULL) {
        // 4.初始化编码上下文
        // 4.1找到编码器
        const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
        if (NULL == codec) {
            LOG(ERROR) << "Can't find h264 encoder." ;
            return;
        }

        // 4.2创建编码器上下文
        codec_context = avcodec_alloc_context3(codec);
        if (NULL == codec_context) {
            LOG(ERROR) << "avcodec_alloc_context3 failed.";
            return;
        }

        // 4.3配置编码器参数
        // vc->flags           |= AV_CODEC_FLAG_GLOBAL_HEADER;
        codec_context->codec_id = codec->id;
        codec_context->thread_count = 8;

        // 压缩后每秒视频的bit流 50k
        codec_context->bit_rate = 50 * 1024 * 8;
        codec_context->width = width;
        codec_context->height = height;
        codec_context->time_base = { 1, fps };
        codec_context->framerate = { fps, 1 };

        // 画面组的大小，多少帧一个关键帧
        codec_context->gop_size = 50;
        codec_context->max_b_frames = 0;
        codec_context->pix_fmt = AV_PIX_FMT_YUV420P;
        codec_context->qmin = 10;
        codec_context->qmax = 51;

        AVDictionary* codec_options = nullptr;
        //(baseline | high | high10 | high422 | high444 | main)
        av_dict_set(&codec_options, "profile", "baseline", 0);
        av_dict_set(&codec_options, "preset", "superfast", 0);
        av_dict_set(&codec_options, "tune", "zerolatency", 0);

        // 4.4打开编码器上下文
        int ret_code = avcodec_open2(codec_context, codec, &codec_options);
        if (0 != ret_code) {
            LOG(ERROR) << "avcodec_open2 failed! ret=" << ret_code;
            return ;
        }
        LOG(INFO) << "avcodec_open2 success!";

        img_convert_ctx = sws_getCachedContext(img_convert_ctx,
                                           width, height, AV_PIX_FMT_BGR24,    // 源格式
                                           width, height, AV_PIX_FMT_YUV420P,  // 目标格式
                                           SWS_BILINEAR,    // 尺寸变化使用算法
                                           0, 0, 0);

        // 3.初始化输出的数据结构
        pYUV = av_frame_alloc();
        pYUV->format = AV_PIX_FMT_YUV420P;
        pYUV->width = width;
        pYUV->height = height;
        pYUV->pts = 0;

        // 分配yuv空间
        ret_code = av_frame_get_buffer(pYUV, 32);
        if (0 != ret_code) {
            LOG(ERROR) << "yuv init fail";
            return;
        }

        memset(&packet, 0, sizeof(packet));
    }
    if (NULL == img_convert_ctx) {
        LOG(ERROR) << "img_convert_ctx error";
        return;
    }

    uint8_t* in_data[AV_NUM_DATA_POINTERS] = { 0 };
    int in_size[AV_NUM_DATA_POINTERS] = { 0 };

    // rgb to yuv
    in_data[0] = frame.data;
    // 一行（宽）数据的字节数
    in_size[0] = frame.cols * frame.elemSize();
    int h = sws_scale(img_convert_ctx, in_data, in_size, 0, frame.rows,
                      pYUV->data, pYUV->linesize);
    if (h <= 0) { return; }

    // h264编码
    pYUV->pts = vpts;
    vpts++;

    int ret_code = avcodec_send_frame(codec_context, pYUV);
    if (0 != ret_code) { return; }

    ret_code = avcodec_receive_packet(codec_context, &packet);

    if (0 != ret_code || packet.buf !=nullptr) {//

    }
    else {
        LOG(ERROR) << "avcodec_receive_packet contiune.";
        return;
    }

    // 推流
    packet.pts = av_rescale_q(packet.pts, codec_context->time_base, vs->time_base);
    packet.dts = av_rescale_q(packet.dts, codec_context->time_base, vs->time_base);
    packet.duration = av_rescale_q(packet.duration,
                                 codec_context->time_base,
                                 vs->time_base);
    ret_code = av_interleaved_write_frame(pOutputFormatCtx, &packet);
    if (0 != ret_code)
    {
        LOG(ERROR) << "pack is error";
    }
    av_packet_unref(&packet);
    frame.release();

//    LOG(INFO) << "Write frame...";
}

void RTMPPushNode::Release() {
    StopRunning();

    avcodec_free_context(&codec_context);
    av_frame_free(&pYUV);
    avio_close(pOutputFormatCtx->pb);
    avformat_free_context(pOutputFormatCtx);
    sws_freeContext(img_convert_ctx);
}
