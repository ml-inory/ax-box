//
// Created by yangrongzhao on 2023/4/25.
//
#include <cstdio>
#include "glog/logging.h"

int main(int argc, char **argv) {
    FLAGS_logtostderr = 1;
    google::InitGoogleLogging(argv[0]);

    LOG(INFO) << "Hello, World";
    return 0;
}
