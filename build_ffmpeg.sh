#!/usr/bin/env bash
SRC=`pwd`/3rdlib_src
DST=`pwd`/3rdlibs
if ! [ -d ${SRC} ]; then
  mkdir -p ${SRC}
fi

# libx264
cd ${SRC}
git clone https://code.videolan.org/videolan/x264.git libx264 --depth=1
cd libx264
# cross compile
# ./configure --host=arm-linux --cross-prefix=${CCPREFIX} --enable-static --disable-asm
./configure --enable-static --enable-pic --disable-asm --prefix=./install
make -j8
make install
X264_PATH=`pwd`/install

# ffmpeg
cd ${SRC}
git clone git://source.ffmpeg.org/ffmpeg.git ffmpeg -b n4.2.9 --depth=1
cd ffmpeg
# cross compile
# ./configure --enable-cross-compile --cross-prefix=${CCPREFIX} --arch=armel --target-os=linux --enable-gpl --enable-libx264 --extra-cflags="-I${X264_PATH}" --extra-ldflags="-ldl -L${X264_PATH}"
./configure --target-os=linux --enable-static --disable-shared --enable-gpl --enable-libx264 --extra-cflags="-fPIC -I${X264_PATH}/include" --extra-ldflags="-ldl -L${X264_PATH}/lib -lx264" --disable-x86asm --enable-protocol=rtp --enable-network --prefix=./install
make -j8
make install

mkdir -p ${DST}
if [ -d ${DST}/ffmpeg ]; then
  rm -rf ${DST}/ffmpeg
fi
mv install ${DST}/ffmpeg

