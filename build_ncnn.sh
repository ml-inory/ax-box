#!/usr/bin/env bash
SRC=`pwd`/3rdlib_src
DST=`pwd`/3rdlibs
if ! [ -d ${SRC} ]; then
  mkdir -p ${SRC}
fi

# ncnn
cd ${SRC}
git clone https://github.com/Tencent/ncnn.git --depth=1
cd ncnn
mkdir -p build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=./install -DBUILD_SHARED_LIBS=OFF
make -j8
make install

mkdir -p ${DST}
if [ -d ${DST}/ncnn ]; then
  rm -rf ${DST}/ncnn
fi
mv install ${DST}/ncnn

