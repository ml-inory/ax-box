//
// Created by yangrongzhao on 2023/4/25.
//
#include <cstdio>
#include <fstream>
#include <memory>

#include "glog/logging.h"
#include "json/json.h"
#include "apps/rtsp_demo.h"
#include "apps/detect_demo.h"

int main(int argc, char **argv) {
    FLAGS_logtostderr = 1;
    google::InitGoogleLogging(argv[0]);

    if (argc < 2) {
        fprintf(stderr, "Usage: %s [config file]\n", argv[0]);
        return -1;
    }

    const char* config_file = argv[1];
    LOG(INFO) << "config file: " << config_file;

    std::ifstream ifs(config_file);
    if (!ifs.is_open()) {
        LOG(ERROR) << "Open config file " << config_file << " failed!";
        return -1;
    }

    Json::Value config;
    ifs >> config;
    std::unique_ptr<App> app(new DetectDemo);
    if (!app->Init(config)) {
        LOG(ERROR) << "Init app failed!";
        return -1;
    }
    LOG(INFO) << "App init success";

    app->Run();

    return 0;
}
