#!/usr/bin/env bash
SRC=`pwd`/3rdlib_src
DST=`pwd`/3rdlibs
if ! [ -d ${SRC} ]; then
  mkdir -p ${SRC}
fi

# jsoncpp
cd ${SRC}
git clone https://github.com/google/glog.git glog --depth=1
cd glog
mkdir -p build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=./install -DWITH_GTEST=OFF -DWITH_GFLAGS=OFF -DBUILD_SHARED_LIBS=OFF -DWITH_UNWIND=OFF
make -j8
make install

mkdir -p ${DST}
if [ -d ${DST}/glog ]; then
  rm -rf ${DST}/glog
fi
mv install ${DST}/glog

